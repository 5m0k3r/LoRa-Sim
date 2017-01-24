//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// The Aloha network consists of hosts which talk to the central "server" via
// the Aloha or Slotted Aloha protocol
//
network Aloha
{
    parameters:
        int numHosts;  // number of hosts
        //int numServers = 5; // numero de SF del GW
        double txRate @unit(bps);  // transmission rate
        double slotTime @unit(ms);  // zero means no slots (pure Aloha)
        double radioDelay @unit(s) = 100ms;
        double iaTime @unit(s)= 400ms;
        @display("bgi=background/terrain,s");
        int pkLenBits @unit(b) = 256b;
       
    submodules:
        
		//server[numServers]: Server {
		//    slotTime = slotTime;
		// 	pkLenBits = pkLenBits;
		// 	radioDelay = radioDelay;
		// 	
		//}
        host[numHosts]: Host {
            txRate = txRate;
            slotTime = slotTime;
            pkLenBits = pkLenBits; 
            radioDelay = radioDelay;  
            iaTime = iaTime;
            
                     
        };
    
        sf7: Server{
        	txRate = 5470bps;
        	iaTime = 56ms;
        	slotTime = slotTime;
		 	pkLenBits = pkLenBits;
		 	radioDelay = radioDelay;
       	};
       	
        sf8: Server{
       		txRate = 3125bps;
        	iaTime = 100ms;
	        slotTime = slotTime;
		 	pkLenBits = pkLenBits;
		 	radioDelay = radioDelay;
        };
        
        sf9: Server{
        	txRate = 1760bps;
        	iaTime = 200ms;
	        slotTime = slotTime;
		 	pkLenBits = pkLenBits;
		 	radioDelay = radioDelay;
        };
        
        sf10: Server{
        	txRate = 980bps;
       		iaTime = 370ms;
       		slotTime = slotTime;
		 	pkLenBits = pkLenBits;
		 	radioDelay = radioDelay;
        };
        
        sf11: Server{
        	txRate = 440bps;
        	iaTime = 740ms;
        	slotTime = slotTime;
		 	pkLenBits = pkLenBits;
		 	radioDelay = radioDelay;
        };
        
        sf12: Server{
        	txRate = 290bps;
        	iaTime = 1400ms;
        	slotTime = slotTime;
		 	pkLenBits = pkLenBits;
		 	radioDelay = radioDelay;
        };
}

