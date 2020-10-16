#include "AE.h"
#include "discoveryMessage_m.h"
#include "utils.h"

void AE::initialize()
{
    //do nothing
    EV <<"INIt AE of ID=" << getIndex() <<"\n";
    URI = getId();  // this is the omnet id which is given when creating the module in the NED file (sequential numbering )
    int random = intuniform(0, featureTypes.size()-1); // the type of the data randomly
    featureType = featureTypes[random];
    data = uniform(0,100);
    maxHop = 1;

    sendAEMessage(REGISTRATION, featureType );

    if (getIndex() == 9) // getIndex is an omnet function that select in  a vector of AE (in this case ) the index in the vector table. In our case only the AE of index 12 send a query
    {
        EV<< "id ==9";
        featureType="thermometer";
        sendAEMessage(QUERY, featureType);
    }
}

void AE::sendAEMessage(int flag, std::string featureType)
{
    switch(flag)
    {
        case REGISTRATION:
        {
            AEMessage *regMsg = new AEMessage("REGISTRATION");
            // set the message fields
            regMsg->setURI(URI);
            regMsg->setFeatureType(featureType.c_str());
            regMsg->setData(data);
            regMsg->setFlag(REGISTRATION);
            //send to the output gate of the AE $o as output and 0 is the number
            send(regMsg,"cse$o",0);
            break;
        }
        case QUERY:
        {
            AEMessage *queryMsg = new AEMessage("QUERY");
            // set the message fields
            queryMsg->setURI(URI);
            //set the type of the resource to be discovered
            //queryMsg->setFeatureType(featureTypes[1].c_str());
            queryMsg->setFeatureType(featureType.c_str());
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


    EV << "Resource of type" << responseMsg->getFeatureType() <<"found in"
            << responseMsg->getSenderModule()<<"\n" << "Data recorded" << "Uri"<< responseMsg->getURI();
    //EV << responseMsg->getDbResult().begin();
    //EV << " " <<responseMsg->getDbResult().begin();
    //for (auto it = responseMsg->getDbResult().begin(); it != responseMsg->getDbResult().end(); )
    //                    {
    //                           EV<< "valeur trouvee" << it->first <<"  " << it->second << "\n";
    //                           it++;
    //                    }






    /*
    AEMessage *queryMsg = new AEMessage("query");
    //int random = intuniform(0, feature_types.size()-1);
    //queryMsg->setFeature_type(feature_types[random]);
    queryMsg->setFeature_type("thermometer");
    send(queryMsg,"cse$o",0);
    */
}








