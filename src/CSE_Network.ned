simple CSE
{
    parameters:
        @display("i=abstract/router,blue,9");
    gates:
        inout provider[] @loose;
        inout customer[] @loose;
        inout sibling[] @loose;
        inout ae[] @loose;
}
simple AE
{
    parameters:
        @display("i=misc/node,#0080FF,23;is=s");
    gates:
        inout cse[] @loose;
}
network CSE_Network
{
    submodules:
        CSE_in[3]: CSE;
        CSE_mn[18]: CSE;
        AE [42]: AE;
    connections allowunconnected :     
        for i=0..2 {
            CSE_in[i].sibling++ <--> CSE_in[(i+1)%3].sibling++;
        }
        for j=0..1 {
            CSE_mn[j].provider++ <--> CSE_in[0].customer++;
        }
        for j=2..3 {
            CSE_mn[j].provider++ <--> CSE_in[1].customer++;
        }
        for j=4..5 {
            CSE_mn[j].provider++ <--> CSE_in[2].customer++;
        }
        for j=6..7 {
            CSE_mn[j].provider++ <--> CSE_mn[0].customer++;
        }
        for j=8..9 {
            CSE_mn[j].provider++ <--> CSE_mn[1].customer++;
        }
        for j=10..11 {
            CSE_mn[j].provider++ <--> CSE_mn[2].customer++;
        }
        for j=12..13 {
            CSE_mn[j].provider++ <--> CSE_mn[3].customer++;
        }
        for j=14..15 {
            CSE_mn[j].provider++ <--> CSE_mn[4].customer++;
        }
        for j=16..17 {
            CSE_mn[j].provider++ <--> CSE_mn[5].customer++;
        }
        for j=0..1 {
            CSE_mn[0].ae++ <--> AE[j].cse++;
        }
        for j=2..3 {
            CSE_mn[1].ae++ <--> AE[j].cse++;
        }
        for j=4..5 {
            CSE_mn[2].ae++ <--> AE[j].cse++;
        }
        for j=6..7 {
            CSE_mn[3].ae++ <--> AE[j].cse++;
        }
        for j=8..9 {
            CSE_mn[4].ae++ <--> AE[j].cse++;
        }
        for j=10..11 {
            CSE_mn[5].ae++ <--> AE[j].cse++;
        }
        for j=12..13 {
            CSE_in[0].ae++ <--> AE[j].cse++;
        }
        for j=14..15 {
            CSE_in[1].ae++ <--> AE[j].cse++;
        }
        for j=16..17 {
            CSE_in[2].ae++ <--> AE[j].cse++;
        }
        for j=18..19{
            CSE_mn[6].ae++ <--> AE[j].cse++;
        }
        for j=20..21{
            CSE_mn[7].ae++ <--> AE[j].cse++;
        }
        for j=22..23{
            CSE_mn[8].ae++ <--> AE[j].cse++;
        }
        for j=24..25{
            CSE_mn[9].ae++ <--> AE[j].cse++;
        }
        for j=26..27{
            CSE_mn[10].ae++ <--> AE[j].cse++;
        }
        for j=28..29{
            CSE_mn[11].ae++ <--> AE[j].cse++;
        }
        for j=30..31{
            CSE_mn[12].ae++ <--> AE[j].cse++;
        }
        for j=32..33{
            CSE_mn[13].ae++ <--> AE[j].cse++;
        }
        for j=34..35{
            CSE_mn[14].ae++ <--> AE[j].cse++;
        }
        for j=36..37{
            CSE_mn[15].ae++ <--> AE[j].cse++;
        }
        for j=38..39{
            CSE_mn[16].ae++ <--> AE[j].cse++;
        }
        for j=40..41{
            CSE_mn[17].ae++ <--> AE[j].cse++;
        }
        
        
        
        
        
        
 
}