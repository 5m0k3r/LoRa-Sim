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
        int numServers = 5; // numero de SF del GW
        double txRate @unit(bps);  // transmission rate
        double slotTime @unit(ms);  // zero means no slots (pure Aloha)
        double radioDelay @unit(s) = 100ms;
        @display("bgi=background/terrain,s");
        int pkLenBits @unit(b) = 256b;
    submodules:
        server[0]: Server{
        	txRate = 5470bps;
        	slotTime = slotTime;
        	iaTime = 56ms;
        	pkLenBits = pkLenBits;
        	radioDelay = radioDelay;
        }
        server[1]: Server{
        	txRate = 3125bps;
        	slotTime = slotTime;
        	iaTime = 100ms;
        	pkLenBits = pkLenBits;
        	radioDelay = radioDelay;
        }
        server[2]: Server{
        	txRate = 1760bps;
        	slotTime = slotTime;
        	iaTime = 200ms;
        	pkLenBits = pkLenBits;
        	radioDelay = radioDelay;
        }
        server[3]: Server{
        	txRate = 980bps;
        	slotTime = slotTime;
        	iaTime = 370ms;
        	pkLenBits = pkLenBits;
        	radioDelay = radioDelay;
        }
        server[4]: Server{
        	txRate = 440bps;
        	slotTime = slotTime;
        	iaTime = 740ms;
        	pkLenBits = pkLenBits;
        	radioDelay = radioDelay;
        }
        server[5]: Server{
        	txRate = 290bps;
        	slotTime = slotTime;
        	iaTime = 1400ms;
        	pkLenBits = pkLenBits;
        	radioDelay = radioDelay;
        }
        host[numHosts]: Host {
            txRate = txRate;
            slotTime = slotTime;
            pkLenBits = pkLenBits; 
            radioDelay = radioDelay;           
        }
}

