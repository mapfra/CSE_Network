#include "CSE.h"

void CSE::initialize()
{
    URI = getId();  // this is the omnet id which is given when creating the module in the NED file (sequential numbering )
}
void CSE::handleMessage(cMessage *msg)
{
    // SWITCH ON THE 5 operational codes

    // if the message comes from the AE
    if (strcmp(msg->getSenderModule()->getName(),"AE")==0)
    {  EV<< "entering the AE part of the IF " <<"\n";
       AEMessage *aeMsg = check_and_cast<AEMessage *>(msg);
       // Create message object and set source and destination field.

       int op_code = aeMsg->getFlag();  // flag contains the type of message

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
               // mapchg parseRouting(aeMsg);
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
         EV<< "The Message is of type : " << discoveryMsg->getFlag() << "\n";
         if (msg->isSelfMessage()){
             //the discovery message comes from the AE and should be forwarded
             EV<< "It is a self Message  " <<"\n";
         } else{
             EV<< "It is not a self Message  " ;
             if (discoveryMsg->getFlag()==QUERY){
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
         } // end ifselfmessage
             // switch on 2 possible opcodes between CSEs : QUERY or RESPONSE

             int op_code = discoveryMsg->getFlag();

             switch(op_code){
             EV<< "Switch OPCODE  \n";

             case QUERY: {
               EV<< "The Message is a query \n";
               if  (discoveryMsg->getHopCount()<=0){
                   //EV<< "HopCount=" << discoveryMsg->getHopCount() <<"\n";
                   //Respond to the initiator that the discovery ends
                   // DBLookup part to be added here
                    std::map<int,int> result;
                    std::map<int,int>::iterator it;


                    EV<< "Before call to DBlookup \n";
                    result = DBLookup(discoveryMsg);
                    //Et on itère sur la map result
                    for (it = result.begin(); it != result.end(); it++) {
                          EV<< "valeur trouvee " << it->first <<"  " << it->second << "\n";
                    }
                    discoveryMsg->setDbResult(result);
                    discoveryMsg->setFlag(RESPONSE);
                   // TODO: set the message flags according to result from DBLookup
                   //You extract from the top of the list the gate that has to be used
                    //MAP change
                    EV<< "The Hopcount is 0 so we generate a self response message \n";
                    generateResponseMessage(discoveryMsg);
                   //send(discoveryMsg->dup(), discoveryMsg->getGateVector().back()->getName() , discoveryMsg->getGateVector().back()->getIndex());

                   //send(discoveryMsg->dup(), discoveryMsg->getGateVector().back()->getName() , discoveryMsg->getGateVector().back()->getIndex());
                   //discoveryMsg->getGateVector().pop_back();
                              //EV<< "back cse event7  " << discoveryMsg->getGateVector().back()->getFullName();
                              //EV<< "back name " << discoveryMsg->getGateVector().back()->getBaseName()<< "\n";
                             // EV<< "back index " << discoveryMsg->getGateVector().back()->getIndex() << "\n";
               } else {
                    // decrease the hop count
                    EV<< "we are in the else :  hopcount is currently " << discoveryMsg->getHopCount() <<"\n";
                    discoveryMsg->setHopCount(discoveryMsg->getHopCount()-1);
                    EV<< "New HopCount=" << discoveryMsg->getHopCount() <<"\n";
                    // and we check the direction of discoveryMsg
                    int D = discoveryMsg->getDirection();
                    switch(D) {

                    case DOWN :
                    {
                       EV<< "We go down \n";
                       int t= gateSize("customer");
                       if (t>0){
                           // if there are customers
                           int vectSize = gate("customer$o",0)->getVectorSize();
                           for(int i=0; i<vectSize; i++)
                           {
                            send(discoveryMsg->dup(),"customer$o", i);
                           }
                       }else {
                           // if there are no customers
                          generateResponseMessage(discoveryMsg);
                       }
                       //propagate the discovery according to the direction and the algo
                       //exploreDownstream(discoveryMsg);
                       break;
                    }
                    default :
                       break;
                    }

               } //end of else
               break;
             }// end of query case

               case RESPONSE: {
                   EV<<"The Message is a response \n";
                   int i= discoveryMsg->getGateVector().size();
                   if(i>=0){
                     EV<< "Size of Gatevector is " << i << "\n";
                     std::vector<cGate *> tempGateVector;
                     // You put on top of the list  the name of the gate to be used in the return path (getOtherHalf)
                     tempGateVector=discoveryMsg->getGateVector();
                     const char * returnGate=tempGateVector.back()->getName();
                     int returnIndex =tempGateVector.back()->getIndex();

                     tempGateVector.pop_back();
                     discoveryMsg->setGateVector(tempGateVector);
                     EV<< "gate removed = " << returnGate << "of index " << returnIndex <<"\n";
                     i= discoveryMsg->getGateVector().size();
                     EV<< "New Size of Gatevector is " << i << "\n";

                     send(discoveryMsg->dup(), returnGate , returnIndex);
                    } else {
                     EV<< "We are in the last gate Message Delivered"<< "\n";
                    }
              break;
              } //end of response case
           } // end of switch opcode
} // end of else AE or CSE
}// end of handle message

// this method forward the initial query to CSE
// void CSE::parseRouting(AEMessage *msg) {
// this function is transforming a query message to a discovery message

void CSE::generateDiscoveryMessage(AEMessage *msg) {
// this function transforms a query message to a discovery message
     // these data should not change during the routing between CSEs
    // TODO lets consider if the URI parameter  is useful ??

    // we created a discovery message
    discoveryMessage *queryMsg = new discoveryMessage ("QUERY");
    // we extract the URI from the AE initiator of the message
    queryMsg->setInitiator(msg->getURI());
    // we extract the msg featureType from AEmessage and we set it in the discovery Message
    queryMsg->setFeatureType(msg->getFeatureType());

    // we set flag to QUERY
    queryMsg->setFlag(QUERY);


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

void CSE::generateResponseMessage(discoveryMessage *responseMsg) {
// this function transforms a discovery message to a response message
     // these data should not change during the routing between CSEs
    // TODO lets consider if the URI parameter  is useful ??
    EV<< "inside generateResponseMessage Procedure"<<"\n";
    // we set flag to RESPONSE
    responseMsg->setFlag(RESPONSE);

   //These data may change during the routing of the query


    // we set the direction to NODIR
    responseMsg->setDirection(NODIR);


    // we schedule this query message to be sent asap in the simulation schedule
    scheduleAt(simTime(),responseMsg);

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
        int vectSize = gate("customer$o",0)->getVectorSize();
        // it register in the scheduler map the UR of the CSE and the parameters of the gate
        // TODO this things should be probably removed
       // std::tuple <int,simtime_t,int> values = schedulerMap[discoveryMsg->getId()];
       // std::tuple <int,simtime_t,int> mapEntry;
       // mapEntry =  std::make_tuple( std::get<0>(values), std::get<1>(values), std::get<2>(values));
       // schedulerMap[discoveryMsg->getURI()]=mapEntry;

        // TODO we need changes in here

        // we will forward through the vectSize of customer gate which have all the customer
        for(int i=0; i<vectSize; i++)
        {
               // if (!(discoveryMsg->getInitialGateIndex()==i))
                //{
                    send(discoveryMsg->dup(),"customer$o",i);
                //}
                //else
                //{
                   // discoveryMsg->setDirection(SIDE);
                   // exploreSidestream(discoveryMsg);

               // }

            }
        }
    else{
        if  (discoveryMsg->getHopCount()==0){
            return;
    }
        else{
            discoveryMsg->setHopCount(discoveryMsg->getHopCount()-1);
            EV<< "New HopCount=" << discoveryMsg->getHopCount() <<"\n";
            return;
        }
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
        // TODO we need changes in here
        // we detect the size of the gate
        int vectSize = gate("sibling$o",0)->getVectorSize();
        EV <<"NO of siblings: "<< vectSize<< "\n";
        //std::tuple<int,simtime_t,int> values = schedulerMap[discoveryMsg->getURI()];
        //EV << "Value of numForward: " << numForward << " ,  myIndex is:" << getIndex() << "\n";
        //std::tuple <int,simtime_t,int> mapEntry;
       // mapEntry =  std::make_tuple( std::get<0>(values), std::get<1>(values), std::get<2>(values));
        //schedulerMap[discoveryMsg->getURI()]=mapEntry;
        // we forward the Discovery Message to all the siblings
        for(int i=0; i<vectSize; i++)
        {
            //if (!(discoveryMsg->getInitialGateIndex()==i))
               //{

                   send(discoveryMsg->dup(),"sibling$o",i);
               //}
               //else
               //{
                  /* std::vector<cGate *> gateVect = discoveryMsg->getGateVector();
                    // You put on top of the list  the name of the gate to be used in the return path (getOtherHalf)
                    gateVect.push_back(discoveryMsg->getArrivalGate()->getOtherHalf());
                    // We  update the query msg with this vector
                    discoveryMsg->setGateVector(gateVect);*/
                     // increase the hop count
                                    //}


           }
       }
    else
    {
        if  (discoveryMsg->getHopCount()==0){
                    return;
            }
                else{
                    discoveryMsg->setHopCount(discoveryMsg->getHopCount()-1);
                    EV<< "New HopCount=" << discoveryMsg->getHopCount() <<"\n";
                    return;
                }
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

       // std::tuple<int,simtime_t,int> values = schedulerMap[discoveryMsg->getURI()];

        EV << "inside upstream...Vector Size " << vectSize << "\n";

       // std::tuple <int,simtime_t,int> mapEntry;
       // mapEntry =  std::make_tuple( std::get<0>(values), std::get<1>(values), std::get<2>(values));

       // schedulerMap[discoveryMsg->getURI()]=mapEntry;

        // we forward to all Providers
        for(int i=0; i<vectSize; i++)
        {
            // TODO we need changes in here
           // if (!(discoveryMsg->getInitialGateIndex()==i))
            //{
                send(discoveryMsg->dup(),"provider$o", i);
             //}else{
                //  std::vector<cGate *> gateVe = discoveryMsg->getGateVector();
                  // You put on top of the list  the name of the gate to be used in the return path (getOtherHalf)
                //  gateVe.push_back(discoveryMsg->getArrivalGate()->getOtherHalf());
                  // We  update the query msg with this vector
                //  discoveryMsg->setGateVector(gateVe);
                  // increase the hop count
             //}


            }
        }

    else{
        EV <<"Discovery Finished "<< "\n";
        }
}
// this function is used to update the database
void CSE::updateDatabase(AEMessage *msg, int op_code)
{
    // we get the flag of the message and we apply the switch_case
    switch(op_code)
    {
    // the message is sent to be stored in the Database
        case REGISTRATION:
        {
            // we extract the featureType; URI; data from the AEmessage
            std::string featureType = msg->getFeatureType();
            int URI = msg->getURI();
            int data = msg->getData();
            EV << "updateDB due to a REGISTRATION : FType="<< featureType << " URI=" << URI << " data=" << data <<"\n";
            // RoutingTable = (RoutingTableItem){.SemanticResourcefeatureType= msg->getFeatureType()};
            // RoutingTable.LocalResource [0]= msg->getURI();
            // RoutingTable.SemanticResourcefeature_type(msg->getfeatureType());
            // RoutingTable.LocalResource(msg->getURI());

            // we create an internal map
            std::map<int,int> internalMap;
            // we create an Iterator on the database
            std::map< std::string, std::map<int,int>>::iterator it;
            EV << "le nbre d'elements de  database est " << database.size() <<"\n";
            // we search for the featureType in the database
            it = database.find(featureType);
            // if we don't find it
            if (it == database.end()){
                EV << "FeatureType not yet in DB\n";
                // putting data in the internal map as a new entry
                internalMap[data]=URI;
                database[featureType]=internalMap;
                //database.insert(featureType, internalMap)
                EV << "le nbre d'elements de  database est " << database.size() <<"\n";
                EV << "le nbre d'elements de  internaldatabase est " << internalMap.size() <<"\n";

            } else {   // if we find the featureType
               EV << "FeatureType already exist\n";
               internalMap = database[featureType];// we put the internal map inside the DataBase map next to the featureType
               internalMap[data]=URI;
               database[featureType]=internalMap;
               EV << "le nbre d'elements de  database est " << database.size() <<"\n";
               EV << "le nbre d'elements de  internaldatabase est " << internalMap.size() <<"\n";
            }
            delete msg;
            break;
        }
        case CANCELLATION:
        {
            // drop the entry from the CSE local database lookup the key= featureType of AE and get list of uri ; scan the entries against the URI of request cancellation

            std::string featureType = msg->getFeatureType();
            int URI = msg->getURI();
            int data = msg->getData();
            EV << "updateDB due to a CANCELLATION : FType="<< featureType << " URI=" << URI << " data=" << data <<"\n";
            // we create an internal map
             std::map<int,int> internalMap;
            // we create an Iterator on the database
             std::map< std::string, std::map<int,int>>::iterator it;
            // we search for the featureType in the database
            it = database.find(featureType);
            if(it== database.end())
            {
                EV << "FeatureType to be cancelled not found in DB \n";
            }
            else
            {
                EV << "FeatureType to be cancelled  founded in DB \n";
                internalMap = database[featureType];
                internalMap.erase(URI);
                database[featureType]=internalMap;
                // here we erace the all liste of feature type
                database.erase(it);
            }
            break;
        }
        default:
                break;
    }// end switch opcode

}

// this is used to lookup in to local database of CSE
std::map<int,int> CSE::DBLookup(discoveryMessage *msg) //this function is used to lookup in the local database
{

   //getting the featureType from the discovery message
   std::string featureType= msg->getFeatureType();

   //EV<< "Feature type " << featureType.c_str() << "\n";
   // creating an iterator for the database
   std::map<std::string, std::map<int,int>>::iterator it;
   // extracting the featureType
   it = database.find(featureType);
   if(!(it == database.end()))
   {
       EV << "Resource of type " << featureType << "founded in URI \n";
       return it->second;
   }
   else
   {    EV << "No Resource of type "<< featureType << "founded the local databease \n";
       return std::map<int,int> { {NOT_FOUND,NOT_FOUND} };
   }
}


// this is used to create a new Discovery Message
discoveryMessage * CSE::generateMessage(int flag)
{
    switch(flag)  {
    case QUERY:
    {
        // Produce source and destination addresses.
        int URI = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setFlag(QUERY);
        msg->setURI(URI);
        return msg;
        break;
    }
    case RESPONSE:
    {
        int URI = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI);
        // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setFlag(RESPONSE);
        msg->setURI(URI);
        return msg;
        break;
    }
    case NOTIFY:
    {
        int URI = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setFlag(NOTIFY);
        msg->setURI(URI);
        return msg;
        break;
    }
    case REGISTRATION:
    {
        int URI = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI);
        // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setFlag(REGISTRATION);
        msg->setURI(URI);
        return msg;
        break;
    }
    case CANCELLATION:
    {
        int URI = getId();
        char msgname[20];
        sprintf(msgname, "bonjour-%d", URI);
    // Create message object and set source and destination field.
        discoveryMessage *msg = new discoveryMessage(msgname);
        //msg->setPayload("thermometer");
        msg->setDirection(DOWN);
        msg->setFlag(REGISTRATION);
        msg->setURI(URI);
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




