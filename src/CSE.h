#ifndef CSE_H_
#define CSE_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "discoveryMessage_m.h"
#include "AEMessage_m.h"
#include <list>
#include "utils.h"

#define UP      0
#define DOWN    1
#define SIDE    2

#define EXPIRATION_TIME  300

using namespace omnetpp;
//using namespace std;
class CSE : public cSimpleModule
{
  protected:
    // by default in omnet methods
      virtual void initialize() override;
      virtual void handleMessage(cMessage *msg) override;
      // application specific  method
      virtual discoveryMessage *generateMessage(int flag);
  private:
      // application specific for progagating query according to CSE roles (directions)
      void exploreDownstream(discoveryMessage *msg);
      void exploreUpstream(discoveryMessage *msg);
      void exploreSidestream(discoveryMessage *msg);

      // application specific for exploring and updating  the local database
      std::map<int,int> DBLookup(discoveryMessage *msg);
      void updateDatabase(AEMessage *msg, int op_code);
      // this method forward the initial query to CSEs
      // MAPchg void parseRouting(AEMessage *msg);
      void generateDiscoveryMessage(AEMessage *msg);
      void generateResponseMessage(discoveryMessage *responseMsg) ;
      // not yet used
      // TODO this function organize the map by value-ordering
      void orderingMap(std::map<int,int>);


/*     WE ARE IN THE NOTIFY SWITCH CASE
     before orderingMap
<(1,30),(2,40),(5,33),(6,12)>
      call orderingMap
<(2,40),(5,33),(1,30),(6,12)>
notify +5 1
<(2,40),(5,33),(1,35),(6,12)>
      call orderingMap
<(2,40),(1,35),(5,33),(6,12)>
notify +22 6
<(2,40),(5,33),(1,35),(6,34)>
      call orderingMap (will take log(n)
<(6,34),(2,40),(5,33),(1,35)>*/

/// DATA structures definitions
       int URI;

       std::map<int,std::tuple<int,simtime_t,int>> schedulerMap;
       // this is the omnet++ ledger
       // that collect some data useful for measuring experiments
       // and is also useful for replying the query.
       // this is composed as follow
       // <id or URI of the CSE ,<gateIndex,simTime,direction>>


       std::map<std::string,std::map<int,int>> database;


      //Routing Table
      struct RoutingTablePattern
      {
         std::map<int,int> database;
         // key is data (used for filter criteria purposes)
         // value is AE_URI of that feature_type
         // e.g. (37,URI_AE1),...
         std::map<int,int> CSECustomer;
         // key is the CSE_URI Customer
         // value is the number of AE feature type
         // e.g. (URI_CSE,#23),...
         std::map<int,int> CSEProvider;
         // the same but CSE_URI Providers
         std::map<int,int> CSESibling;
         // the same but CSE_URI Sibling
         std::map<int,int> CSEPeer;
         // the same but CSE_URI Peer
         std::map<int,int> CSEBucket;
         // key is the CSE_URI
         // value is the % of successful query result
      } RoutingTable;




};
Define_Module(CSE);

#endif /* CSE_H_ */




