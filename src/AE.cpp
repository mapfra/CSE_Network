#include "AE.h"
#include "discoveryMessage_m.h"
#include "utils.h"

void AE::initialize()
{
    // get the id of the AE
    URI = getId();  // this is the omnet id which is given when creating the module in the NED file (sequential numbering )
    // selecting randomly the feature type
    int random = intuniform(0, feature_types.size()-1); // the type of the data randomly
    feature_type = feature_types[random];
    // assigning randomly the data
    data = uniform(0,100);
    // maxhop set to 10
    maxHop = 10;
    // send an AE message with op_code Registration
    sendAEMessage(REGISTRATION);

    // getIndex is an omnet function that select in  a vector of AE (in this case ) the index in the vector table.
    if (getIndex() == 5)

    {
        // send the AE message with op_code QUERY
        sendAEMessage(QUERY);
    }
}// end of initialize

void AE::sendAEMessage(int op_code)
{
    // this function we set the fields of an AEMessage with respect to op_code
    switch(op_code)
    {
        case REGISTRATION:
        {
            AEMessage *regMsg = new AEMessage("REGISTRATION");
            // set the message fields
            regMsg->setURI(URI);
            regMsg->setFeature_type(feature_type.c_str());
            regMsg->setData(data);
            regMsg->setOp_code(REGISTRATION);
            //send to the output gate of the AE $o as output and 0 is the number
            send(regMsg,"cse$o",0);
            break;
        }// end of REGISTRATION case
        case QUERY:
        {
            AEMessage *queryMsg = new AEMessage("QUERY");
            queryMsg->setURI(URI);
            queryMsg->setFeature_type(feature_types[1].c_str());
            queryMsg->setOp_code(QUERY);
            queryMsg->setMaxHop(maxHop);
            send(queryMsg,"cse$o",0);
            break;
        }// end of QUERY case

        default:
            break;
    }// end of switch

}// end of function sendAEMessage

void AE::handleMessage(cMessage *msg)
{
    //AE will receive the response
    //AEMessage *responseMsg = check_and_cast<AEMessage *>(msg);
    discoveryMessage *responseMsg = check_and_cast<discoveryMessage *>(msg);
    EV <<"AE receives a response" << "\n";
    EV << "Resource of type" << responseMsg->getFeature_type() <<"found in" << responseMsg->getSenderModule()<<"\n"
            << "Data recorded"<< "Uri"<< responseMsg->getURI_route();
}








