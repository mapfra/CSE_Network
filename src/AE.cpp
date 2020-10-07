#include "AE.h"
#include "discoveryMessage_m.h"
#include "utils.h"

void AE::initialize()
{
    //do nothing
    URI = getId();  // this is the omnet id which is given when creating the module in the NED file (sequential numbering )
    int random = intuniform(0, feature_types.size()-1); // the type of the data randomly
    feature_type = feature_types[random];
    data = uniform(0,100);
    maxHop = 3;
    sendAEMessage(REGISTRATION);

    if (getIndex() == 16) // getIndex is an omnet function that select in  a vector of AE (in this case ) the index in the vector table. In our case only the AE of index 12 send a query
    {
        sendAEMessage(QUERY);
    }
}

void AE::sendAEMessage(int flag)
{
    switch(flag)
    {
        case REGISTRATION:
        {
            AEMessage *regMsg = new AEMessage("REGISTRATION");
            // set the message fields
            regMsg->setURI(URI);
            regMsg->setFeature_type(feature_type.c_str());
            regMsg->setData(data);
            regMsg->setFlag(REGISTRATION);
            //send to the output gate of the AE $o as output and 0 is the number
            send(regMsg,"cse$o",0);
            break;
        }
        case QUERY:
        {
            AEMessage *queryMsg = new AEMessage("QUERY");
            queryMsg->setURI(URI);
            //int random = intuniform(0, feature_types.size()-1);
            //set the type of the resource to be discovered
            queryMsg->setFeature_type(feature_types[1].c_str());
            queryMsg->setFlag(QUERY);
            queryMsg->setMaxHop(maxHop);
            send(queryMsg,"cse$o",0);
            break;
        }

        default:
            break;
    }

}

void AE::handleMessage(cMessage *msg)
{
    //MAP AE will receive the response
    //AEMessage *responseMsg = check_and_cast<AEMessage *>(msg);
    discoveryMessage *responseMsg = check_and_cast<discoveryMessage *>(msg);
    EV <<"AE receives a response" << "\n";
    EV << "Resource of type" << responseMsg->getFeature_type() <<"found in" << responseMsg->getSenderModule()<<"\n"
            << "Data recorded"<< responseMsg->getData()<< "\n"<< "Uri"<< responseMsg->getURI();





    /*
    AEMessage *queryMsg = new AEMessage("query");
    //int random = intuniform(0, feature_types.size()-1);
    //queryMsg->setFeature_type(feature_types[random]);
    queryMsg->setFeature_type("thermometer");
    send(queryMsg,"cse$o",0);
    */
}








