//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

//
// The central computer in the ALOHAnet network.
//
simple Server
{
    parameters:
        @display("i=device/antennatower_l");
        @signal[receiveBegin](type="long");  // increases with each new frame arriving to the server and drops to 0 if the channel becomes finally idle
        @signal[receive](type="long");  // for successful receptions (non-collisions): 1 at the start of the reception, 0 at the end of the reception
        @signal[collision](type="long"); // the number of collided frames at the beginning of the collision period
        @signal[collisionLength](type="simtime_t");  // the length of the last collision period at the end of the collision period
        @signal[channelState](type="long");

        @statistic[serverChannelState](source="channelState";title="Channel state";enum="IDLE = 0, TRANSMIT = 1 , PRERECEIVE = 2, RECEIVE = 3, RECEIVE2=4, IDLE2 = 5, ACK=6, ACK2=7, SLEEP=8";record=vector);
        @statistic[receiveBegin](source="receiveBegin"; record=vector?; interpolationmode=sample-hold; title="receive begin");
        @statistic[channelUtilization](source="timeavg(receive)"; record=last; interpolationmode=linear; title="channel utilization");
        @statistic[collisionMultiplicity](source=collision; record=vector?,histogram; title="collision multiplicity");
        @statistic[collisionLength](record=vector?,histogram,mean,sum,max; title="collision length");
        @statistic[receivedFrames](source="sum(receive)"; record=last; title="received frames");
        @statistic[collidedFrames](source="count(collision)"; record=vector?,histogram; title="collided frames");
        @signal[errorpFrame](type="long");
     	@statistic[errorpCount](title="error count"; source="errorpFrame"; record=vector,stats; interpolationmode=none);
        volatile int pkLenBits @unit(b);
        volatile double iaTime @unit(s);
        double slotTime @unit(s);         // zero means no slots (pure Aloha)
        double txRate @unit(bps);         // transmission rate
        double radioDelay @unit(s);
        int per;
        
        //int sleeptime @unit(s);
    gates:
        input in @directIn;
}

