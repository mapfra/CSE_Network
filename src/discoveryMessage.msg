cplusplus{{
    #include <map>
    #include <vector>
    //using namespace omnetpp;
    typedef std::map<int,int> DBresult;
    
    // this has a problem
    typedef std::vector<omnetpp::cGate *> GateVector;
}}

class noncobject DBresult;
class noncobject GateVector;

message discoveryMessage
{
    int URI_init;// this is the identifer of the very first AE generating Discovery QUERY it will be used intensively (the initiator in oneM2M)
    // in the recommendation system because sender will be memorized in the BUCKETS[TYPE] during reverse path 
    // by each CSE Node
    
    int URI_route;// this is identifer of in route CSE, sending the message to another CSE
    string feature_type; // this is type of Resource CSE is looking for. Values can be "waterValve","thermometer","airStation","ATM","smartLock"
      
    int op_code; //this can be
        // Registration of a CSE (with all the subtree connected with) into another CSE  (0)
        // Update of a CSE routing table   (1)
        // Cancellation of a CSE (with all the subtree connected with) (2)
        // Querying a feature type (3)
        // Responding on Query with a (possibly empty) list of URI (7)
        // Notifying new AE (un)registration on the neighbors CSE (8)
  
    int hopCount; // this value  lists the number of remaining hopes before end of forwarding QUERY 
    
    int direction;// this will be UP (customer to provider) or DOWN (Provider to Customer)
    // or SIDE (Sibling to Sibling). direction where the message has been sent
    
    int initialGateIndex; // this is the index of the gate from which the request has been sent originator gate.
    
    DBresult dbResult;    //This is the result obtained when looking in the local database, Result is a (uri) To be checked The second value not mandatory ??to be checked
    GateVector gateVector; // this is the list that contains the discovery path (list of CSE that forward the query). It is used for the returning path 
    

  
}