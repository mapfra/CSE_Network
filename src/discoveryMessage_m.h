//
// Generated file, do not edit! Created by nedtool 5.6 from discoveryMessage.msg.
//

#ifndef __DISCOVERYMESSAGE_M_H
#define __DISCOVERYMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    #include <map>
    #include <vector>
    //using namespace omnetpp;
    typedef std::map<int,int> DBresult;
    
    // this has a problem
    typedef std::vector<omnetpp::cGate *> GateVector;
// }}

/**
 * Class generated from <tt>discoveryMessage.msg:15</tt> by nedtool.
 * <pre>
 * message discoveryMessage
 * {
 *     int URI_init;// this is the identifer of the very first AE generating Discovery QUERY it will be used intensively (the initiator in oneM2M)
 *     // in the recommendation system because sender will be memorized in the BUCKETS[TYPE] during reverse path 
 *     // by each CSE Node
 * 
 *     int URI_route;// this is identifer of in route CSE, sending the message to another CSE
 *     string feature_type; // this is type of Resource CSE is looking for. Values can be "waterValve","thermometer","airStation","ATM","smartLock"
 * 
 *     int op_code; //this can be
 *         // Registration of a CSE (with all the subtree connected with) into another CSE  (0)
 *         // Update of a CSE routing table   (1)
 *         // Cancellation of a CSE (with all the subtree connected with) (2)
 *         // Querying a feature type (3)
 *         // Responding on Query with a (possibly empty) list of URI (7)
 *         // Notifying new AE (un)registration on the neighbors CSE (8)
 * 
 *     int hopCount; // this value  lists the number of remaining hopes before end of forwarding QUERY 
 * 
 *     int direction;// this will be UP (customer to provider) or DOWN (Provider to Customer)
 *     // or SIDE (Sibling to Sibling). direction where the message has been sent
 * 
 *     int initialGateIndex; // this is the index of the gate from which the request has been sent originator gate.
 * 
 *     DBresult dbResult;    //This is the result obtained when looking in the local database, Result is a (uri) To be checked The second value not mandatory ??to be checked
 *     GateVector gateVector; // this is the list that contains the discovery path (list of CSE that forward the query). It is used for the returning path 
 * 
 * 
 * 
 * }
 * </pre>
 */
class discoveryMessage : public ::omnetpp::cMessage
{
  protected:
    int URI_init;
    int URI_route;
    ::omnetpp::opp_string feature_type;
    int op_code;
    int hopCount;
    int direction;
    int initialGateIndex;
    DBresult dbResult;
    GateVector gateVector;

  private:
    void copy(const discoveryMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const discoveryMessage&);

  public:
    discoveryMessage(const char *name=nullptr, short kind=0);
    discoveryMessage(const discoveryMessage& other);
    virtual ~discoveryMessage();
    discoveryMessage& operator=(const discoveryMessage& other);
    virtual discoveryMessage *dup() const override {return new discoveryMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getURI_init() const;
    virtual void setURI_init(int URI_init);
    virtual int getURI_route() const;
    virtual void setURI_route(int URI_route);
    virtual const char * getFeature_type() const;
    virtual void setFeature_type(const char * feature_type);
    virtual int getOp_code() const;
    virtual void setOp_code(int op_code);
    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
    virtual int getDirection() const;
    virtual void setDirection(int direction);
    virtual int getInitialGateIndex() const;
    virtual void setInitialGateIndex(int initialGateIndex);
    virtual DBresult& getDbResult();
    virtual const DBresult& getDbResult() const {return const_cast<discoveryMessage*>(this)->getDbResult();}
    virtual void setDbResult(const DBresult& dbResult);
    virtual GateVector& getGateVector();
    virtual const GateVector& getGateVector() const {return const_cast<discoveryMessage*>(this)->getGateVector();}
    virtual void setGateVector(const GateVector& gateVector);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const discoveryMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, discoveryMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __DISCOVERYMESSAGE_M_H

