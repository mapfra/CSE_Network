#include "CSE.h"

void CSE::initialize()
{
    int URI = getId();  // this is the omnet id which is given when creating the module in the NED file (sequential numbering )
}
void CSE::handleMessage(cMessage *msg)
{
    // SWITCH ON THE 5 operational codes

    // if the message comes from the AE
    if (strcmp(msg->getSenderModule()->getName(),"AE")==0)
    {  EV<< "entering the AE part of the IF " <<"\n";
       AEMessage *aeMsg = check_and_cast<AEMessage *>(msg);
       // Create message object and set source and destination field.

       int op_code = aeMsg->getOp_code();  // op_code contains the type of message

       switch(op_code) {
           case REGISTRATION: {
               // if it's a registration message we just update the entry in CSE local database
               updateDatabase(aeMsg, op_code);

               break;
           }
           case CANCELLATION:{
               // if it is cancellation msg we cancel the entry from CSE local database
               updateDatabase(aeMsg, op_code);
               break;
           }
           case QUERY : {
               // if it is a query msg we create a discovery msg and we start ASDR
               generateDiscoveryMessage(aeMsg);
               break;
           }
           default:
               break;
      }
     }  else {
         EV<< "entering the CSE part  " <<"\n";
         // if the message comes from another resource that an AE
         discoveryMessage *discoveryMsg = check_and_cast<discoveryMessage *>(msg);
         EV<< "The Message is of type : " << discoveryMsg->getOp_code() << "\n";
         if (msg->isSelfMessage()){
             //the discovery message comes from the AE and should be forwarded
             EV<< "It is a self Message  " <<"\n";
         } else{
             EV<< "It is not a self Message  " ;
             if (discoveryMsg->getOp_code()==QUERY){
                 EV<< "of type query\n";
             std::vector<cGate *> tempGateVector;
             // You put on top of the list  the name of the gate to be used in the return path (getOtherHalf)
             tempGateVector=discoveryMsg->getGateVector();
             tempGateVector.push_back(msg->getArrivalGate()->getOtherHalf());
             discoveryMsg->setGateVector(tempGateVector);
             EV<< "A new  gate is added = " << tempGateVector.back()->getFullName() << "\n";
             }else{
                 EV<< "of type response so no new gate added\n";
             }
             }// end if self-message
             // switch on 2 possible opcodes between CSEs : QUERY or RESPONSE

             int op_code = discoveryMsg->getOp_code();

             switch(op_code){
             EV<< "Switch OPCODE  \n";

             case QUERY: {

                 std::map<int,int> res = DBLookup(discoveryMsg);
                 // If we find the index "NOT_FOUND" in the map, it means that
                 // the feature is not present in the database
                 if((res.find(NOT_FOUND) == res.end()))
                 {
                     EV<< "DB Lookup Successful"<< "\n";
                     discoveryMsg->setDbResult(res);
                     generateResponseMessage(discoveryMsg);

                }
                 // Otherwise it is present and we should reply back with the data extracted from the database
                else{
                     EV<< "The Message is a query \n";
                     EV<< "DB Lookup not Successful"<< "\n";
                     if  (discoveryMsg->getHopCount()<=0){
                         //Respond to the URI_init that the discovery ends
                         // TODO: DBLookup part to be added here
                         discoveryMsg->setOp_code(RESPONSE);
                         // TODO: set the message op_codes according to result from DBLookup
                         //You extract from the top of the list the gate that has to be used
                         EV<< "Hop count is 0 so we generate a self response message \n";
                         generateResponseMessage(discoveryMsg);
                     }
                     else {
                    // decrease the hop count
                    EV<< "we are in the else :  hop count is currently " << discoveryMsg->getHopCount() <<"\n";
                    discoveryMsg->setHopCount(discoveryMsg->getHopCount()-1);
                    EV<< "New HopCount=" << discoveryMsg->getHopCount() <<"\n";
                    // and we check the direction of discoveryMsg
                    int D = discoveryMsg->getDirection();
                    switch(D) {

                    case DOWN :
                    {
                        exploreDownstream(discoveryMsg);
                        exploreSidestream(discoveryMsg);
                       break;
                       }
                    case SIDE:
                    {
                        exploreSidestream(discoveryMsg);
                        exploreDownstream(discoveryMsg);
                        exploreUpstream(discoveryMsg);
                       break;
                    }
                    case UP:
                    {
                        exploreUpstream(discoveryMsg);
                        exploreDownstream(discoveryMsg);
                        exploreSidestream(discoveryMsg);
                        break;
                    }
                    default :
                       break;
                    }

               } //end of else
               break;
                }
             }// end of query case

             case RESPONSE:
             {
                 EV<<"The Message is a response \n";
                 int i= discoveryMsg->getGateVector().size();
                 if(i>=0){
                 EV<< "Size of Gate vector is " << i << "\n";
                 std::vector<cGate *> tempGateVector;
                 // You put on top of the list  the name of the gate to be used in the return path (getOtherHalf)
                 tempGateVector=discoveryMsg->getGateVector();
                 const char * returnGate=tempGateVector.back()->getName();
                 int returnIndex =tempGateVector.back()->getIndex();

                 tempGateVector.pop_back();
                 discoveryMsg->setGateVector(tempGateVector);
                 EV<< "gate removed = " << returnGate << "of index " << returnIndex <<"\n";
                 i= discoveryMsg->getGateVector().size();
                 EV<< "New Size of Gate vector is " << i << "\n";
                 EV<< "<Module Name"<< discoveryMsg->getName()<< "gate name"<< returnGate<<"\n"<<"gateIndex"<< returnIndex<<"\n";
                 send(discoveryMsg->dup(), returnGate , returnIndex);
                } else {
                 EV<< "We are in the last gate Message Delivered"<< "\n";
                    }
              break;
              } //end of response case
             default:
                 break;
           } // end of switch opcode
} // end of else AE or CSE
}// end of handle message

void CSE::generateResponseMessage(discoveryMessage* responseMsg)
{
    EV<< "inside generateResponseMessage Procedure"<<"\n";
    // we set op_code to RESPONSE
    responseMsg->setOp_code(RESPONSE);
    //These data may change during the routing of the query
    // we set the direction to NODIR
    responseMsg->setDirection(NODIR);

    cancelEvent(responseMsg);
    scheduleAt(simTime(), responseMsg);
}

// this method forward the initial query to CSE
// void CSE::parseRouting(AEMessage *msg) {
// this function is transforming a query message to a discovery message

void CSE::generateDiscoveryMessage(AEMessage *msg) {
// this function transforms a query message to a discovery message
     // these data should not change during the routing between CSEs
    // TODO lets consider if the URI parameter  is useful ??

    // we created a discovery message
    discoveryMessage *queryMsg = new discoveryMessage ("QUERY");
    // we extract the URI from the AE URI_init of the message
    queryMsg->setURI_init(msg->getURI());
    // we extract the msg feature_type from AEmessage and we set it in the discovery Message
    queryMsg->setFeature_type(msg->getFeature_type());

    // we set op_code to QUERY
    queryMsg->setOp_code(QUERY);


//These data may change during the routing of the query

    // set the hop count
     queryMsg->setHopCount(msg->getMaxHop());

    // we set the direction DOWN
    queryMsg->setDirection(DOWN);

    // create a omnet vector of type cGate* named gateVector
    std::vector<cGate *> gateVector = queryMsg->getGateVector();
    //You update the discoveryMessage with this object
    queryMsg->setGateVector(gateVector);
    // You put on top of the list  the name of the gate to be used in the return path (getOtherHalf)
    gateVector.push_back(msg->getArrivalGate()->getOtherHalf());

            EV<< "back cse event7  " << gateVector.back()->getFullName();
            EV<< "front  " << gateVector.front()->getFullName();

    // We  update the query msg with this vector
    queryMsg->setGateVector(gateVector);
            EV<< "back cse event7  " << queryMsg->getGateVector().back()->getFullName();
            EV<< "front  " << queryMsg->getGateVector().front()->getFullName();

    // we schedule this query message to be sent asap in the simulation schedule
    scheduleAt(simTime(),queryMsg);

    // delete the AE message
    delete msg;
}

// used for exploring in customers
void CSE::exploreDownstream(discoveryMessage *discoveryMsg)
{
    // checking the size of gate
    int t= gateSize("customer");
    //if it is greater than zero means if we have customer
    if (t>0)
    {
        EV<< "inside Downstream"<<"\n";
        // it detects the size of the customer gates
        int Uri_customer = gate("provider$o",0)->getId();
        EV<< "uri of customer"<< Uri_customer<< "\n";
        int vectSize = gate("customer$o",0)->getVectorSize();
        // it register in the scheduler map the UR of the CSE and the parameters of the gate
        // we will forward through the vectSize of customer gate which have all the customer
        for(int i=0; i<vectSize; i++)
        {
            // TODO: should be modify
            send(discoveryMsg->dup(),"customer$o",i);
        }
        }
    else{
         generateResponseMessage(discoveryMsg);
        }
}

// Used for exploring resources in Siblings
void CSE::exploreSidestream(discoveryMessage *discoveryMsg)
{
    // checking the gate size of sibling gate
    int t= gateSize("sibling");
    // if value is greater than zero means if we have siblings
    if (t>0)
    {
        // we detect the size of the gate
        int vectSize = gate("sibling$o",0)->getVectorSize();
        EV <<"NO of siblings: "<< vectSize<< "\n";
        // we forward the Discovery Message to all the siblings
        for(int i=0; i<vectSize; i++)
        {
            EV<< "arrival gate id ="<< discoveryMsg->getInitialGateIndex();


                send(discoveryMsg->dup(),"sibling$o",i);
       }
    }
    else
    {
        generateResponseMessage(discoveryMsg);

    }
    }

// used to explore the Providers
void CSE::exploreUpstream(discoveryMessage *discoveryMsg)
{
    // checking the size of gate
    int t= gateSize("provider");
    // if there are providers
    if (t>0)
    {
        // we detect the size of vector gate Provider
        int vectSize = gate("provider$o",0)->getVectorSize();
        EV << "inside upstream...Vector Size " << vectSize << "\n";
        // we forward to all Providers
        for(int i=0; i<vectSize; i++)
        {
            send(discoveryMsg->dup(),"provider$o", i);
        }
        }
    else{
        generateResponseMessage(discoveryMsg);
    }
}

// this function is used to update the database
void CSE::updateDatabase(AEMessage *msg, int op_code)
{
    // we get the op_code of the message and we apply the switch_case
    switch(op_code)
    {
    // the message is sent to be stored in the Database
        case REGISTRATION:
        {
            // we extract the feature_type; URI_route; data from the AEmessage
            std::string feature_type = msg->getFeature_type();
            int URI_route = msg->getURI();
            int data = msg->getData();
            //std::map<int, int> customerlist;
            //std::map<int, int> providerlist;
            //std::map<int, int> siblinglist;

           //customerlist[URI]=

            //std::map<std::string , struct RoutingTable> SemanticRoutingTable;
            //SemanticRoutingTable[msg->getFeature_type()]= RoutingTable;

            //customerlist[discoveryMsg->getURI]= t;


            //RoutingTable = (RoutingTablePattern){.SemanticResourcefeature_type= msg->getFeature_type()};
           // RoutingTable.CSECustomer=
            //RoutingTable.SemanticResourcefeature_type(msg->getfeature_type());
            //RoutingTable.LocalResource(msg->getURI_route());

            // we create an internal map
            std::map<URI,int> internalMap;
            // we create an Iterator on the database
            std::map< std::string, std::map<URI,int>>::iterator it;
            // we search for the feature_type in the database
            it = database.find(feature_type);
            // if we don't find it
            if (it == database.end())
            {
                // putting data in the internal map as a new entry
                internalMap[URI_route]=data;
            }
            // if we find the feature_type
            else
            {
               internalMap = database[feature_type];// we put the internal map inside the DataBase map next to the feature_type
               internalMap[URI_route]=data;
            }
            database[feature_type]=internalMap;

            EV<< "feature type added in Database"<< msg->getFeature_type() << "\n";

            delete msg;

            break;
        }
        case CANCELLATION:
        {
            // drop the entry from the CSE local database lookup the key= feature_type of AE and get list of uri ; scan the entries against the URI_route of request cancellation
            EV<< "to do" ;
            std::string feature_type = msg->getFeature_type();
            // we create an internal map
             std::map<int,int> internalMap;
            // we create an Iterator on the database
             std::map< std::string, std::map<int,int>>::iterator it;
            // we search for the feature_type in the database
            //it = database.find(feature_type);
            if(it== database.end())
            {


            }
            else
            {
                database.erase(it);
            }
            break;
        }
            default:
                break;
    }

}

// this is used to lookup in to local database of CSE
std::map<int,int> CSE::DBLookup(discoveryMessage *msg) //this function is used to lookup in the local database
{

   //getting the feature_type from the discovery message
   std::string feature_type= msg->getFeature_type();
   //EV<< "Feature type " << feature_type.c_str() << "\n";
   // creating an iterator for the database
   std::map<std::string, std::map<int,int>>::iterator it;
   // extracting the feature_type
   it = database.find(feature_type);
   // if we find the data correspond to the feature_type
   if(!(it == database.end()))
   {
       return it->second; // because we need will send the values corresponds to the feature_type
   }
   else
   {
       return std::map<int,int> { {NOT_FOUND,NOT_FOUND} };
   }
}


// this is used to create a new Discovery Message
discoveryMessage * CSE::generateMessage(int op_code)
{
    switch(op_code)  {
    case QUERY:
    {
        // Produce source and destination addresses.
        int URI_route = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI_route);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setOp_code(QUERY);
        msg->setURI_route(URI_route);
        return msg;
        break;
    }
    case RESPONSE:
    {
        int URI_route = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI_route);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setOp_code(RESPONSE);
        msg->setURI_route(URI_route);
        return msg;
        break;
    }
    case NOTIFY:
    {
        int URI_route = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI_route);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setOp_code(NOTIFY);
        msg->setURI_route(URI_route);
        return msg;
        break;
    }
    case REGISTRATION:
    {
        int URI_route = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI_route);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setOp_code(REGISTRATION);
        msg->setURI_route(URI_route);
        return msg;
        break;
    }
    case CANCELLATION:
    {
        int URI_route = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI_route);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setOp_code(REGISTRATION);
        msg->setURI_route(URI_route);
        return msg;
        break;
    }
    default:
        break;

}
}

void CSE::orderingMap(std::map<int,int>)
{
    return;
}




