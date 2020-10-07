message AEMessage
{
    int URI; // this is the unique identifer of the AE, sending the message
    string feature_type; // this is the feature type of the resource we are looking for; in this version of protocol 
    // we can just query one feature type per QUERY
    int data; // this is the value concerning the resource we are looking for; Actually used in REGISTRATION.
    // TO DO it will be used in UPDATE, PUT, GET, REPLY. 
    int flag; // this can be 
     // REGISTRATION (when AE register to CSE), value =0
     // UPDATE (when AE update every x minutes the CSE about the update in its value in local Database); value =1
     // CANCELLATION (when AE wants to cancel the Resource inside CSE local Database) value =2 
     // QUERY (when AE wants to ask for resource), value =3 
     // PUT (CSE to AE: CSE gives some directive to the corresponding AE) value =4 
     // GET (CSE to AE: CSE ask some value from the corresponding AE) value =5
     // REPLY (AE to CSE: AE reply to the CSE with a value normally in data) value =6 
     // RESPONSE (AE to CSE: AE reply to the CSE with a value normally in data) value =7 
     // NOTIFY between CSE to notify modifcations in route tables  value =8 
     // still to be asked from oneM2M experts, whether or not add the following flag
     
     int maxHop; // used for a discovery query. Number of hops for the search  

     // NOTE:: the packet dose not need destination URI because in our implementation it will be forwarded using Gates.
     // this will affect almost all the operational flags.  
}

