//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 




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
network NetworkV2
{
int nb_CseIn= 3;
int nb_AE =18;
int nb_CseMn = 6;

    submodules:
        CSE_in[nb_CseIn]: CSE;
        CSE_mn[nb_CseMn]: CSE;
        
        AE [nb_AE]: AE;
    connections allowunconnected : 
        // full interconnection between CSE_IN     
        for i=0..nb_CseIn-2 {
            CSE_in[i].sibling++ <--> CSE_in[(i+1)%nb_CseIn].sibling++;
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
}

    

