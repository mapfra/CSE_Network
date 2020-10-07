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
        CSE_mn[6]: CSE;
        
        AE [18]: AE;
    connections allowunconnected :  
 // LINKS BETWEEN IN  
        // IN peers are fully connected  
        for i=0..2 {
            CSE_in[i].sibling++ <--> CSE_in[(i+1)%3].sibling++;
        }
 // LINKS BETWEEN MN and IN  
        // MN 0 and 1 are customers of IN0  
        for j=0..0 {
            CSE_mn[j].provider++ <--> CSE_in[0].customer++;
        }
        // MN 2 and 3 are customers of IN1  
        for j=2..3 {
            CSE_mn[j].provider++ <--> CSE_in[1].customer++;
        }
        // MN 4 and 5 are customers of IN2  
        for j=4..5 {
            CSE_mn[j].provider++ <--> CSE_in[2].customer++;
        }
// LINKS BETWEEN MN and AEs 
        // MN 0 has AE 0 `na 1  on it  
        for j=0..1 {
            CSE_mn[0].ae++ <--> AE[j].cse++;
        }
        //...
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
        
 // LINKS BETWEEN IN and AEs        
        // IN 0 has AE 12 and 13  on it  
        for j=12..13 {
            CSE_in[0].ae++ <--> AE[j].cse++;
        }
        for j=14..15 {
            CSE_in[1].ae++ <--> AE[j].cse++;
        }
        for j=16..17 {
            CSE_in[2].ae++ <--> AE[j].cse++;
        }


//ADDED BY MAP 
// LINKS BETWEEN MNs  
               
       // MN 5 is customer of MN4 
       
            CSE_mn[5].provider++ <--> CSE_mn[4].customer++;
            CSE_mn[1].provider++ <--> CSE_mn[5].customer++;
}      