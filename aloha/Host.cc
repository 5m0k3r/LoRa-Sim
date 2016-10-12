//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Host.h"
namespace aloha {

Define_Module(Host);

Host::Host()
{
    endTxEvent = nullptr;
    endJoinEvent = nullptr;
    endJoinEvent2 = nullptr;
}

Host::~Host()
{
    cancelAndDelete(endTxEvent);
    cancelAndDelete(endJoinEvent);
    cancelAndDelete(endJoinEvent2);

}

void Host::initialize()
{
    stateSignal = registerSignal("state");
    receiveSignal = registerSignal("receive");
    receiveBeginSignal = registerSignal("receiveBegin");
    channelStateSignal = registerSignal("channelState");

    server = getModuleByPath("server");
    if (!server)
        throw cRuntimeError("server not found");
    endRxEvent = new cMessage("end-reception");
    endJoinEvent = new cMessage("end-downlink-window1");
    endJoinEvent2 = new cMessage("end-downlink-window2");
    endTxEvent = new cMessage("send/endTx");

    txRate = par("txRate");
    radioDelay = par("radioDelay");
    iaTime = &par("iaTime");
    pkLenBits = &par("pkLenBits");
    receiveCounter = 0;
    slotTime = par("slotTime");
    isSlotted = slotTime > 0;
    WATCH(slotTime);
    WATCH(isSlotted);
    WATCH(txRate);
    WATCH(radioDelay);
    state = IDLE;
    emit(stateSignal, state);
    pkCounter = 0;
    WATCH((int&)state);
    WATCH(pkCounter);
    gate("in")->setDeliverOnReceptionStart(true);
    scheduleAt(getNextTransmissionTime(), endTxEvent);
}

void Host::handleMessage(cMessage *msg)
{

    //ASSERT(msg == endTxEvent );
    if (state == IDLE) {
        // generate packet and schedule timer when it ends
        char pkname[40];
        sprintf(pkname, "pk-%d-#%d", getId(), pkCounter++);
        EV << "generating packet " << pkname << endl;
        //EV << "generating packet to" << server << endl;

        state = TRANSMIT;
        emit(stateSignal, state);

        cPacket *pk = new cPacket(pkname);
        pk->setBitLength(pkLenBits->longValue());
        simtime_t duration = pk->getBitLength() / txRate;
        sendDirect(pk, radioDelay, duration, server->gate("in"));
        scheduleAt(simTime()+duration, endTxEvent);
        cancelEvent(endJoinEvent);
        cancelEvent(endJoinEvent2);

        gate("in")->setDeliverOnReceptionStart(true);

    }
    else if (state == TRANSMIT) {
        // endTxEvent indicates end of transmission
        state = RECEIVE;

        emit(stateSignal, state);

        // schedule next sending
        simtime_t endReceptionTime = simTime() + msg->getSendingTime();
        // start of reception at recvStartTime
        cTimestampedValue tmp(recvStartTime, 1l);
        emit(receiveSignal, &tmp);
        // end of reception now
        emit(receiveSignal, 0);
        emit(receiveBeginSignal, receiveCounter);
        EV << "downlink-window-1 received"<<endl;
        //scheduleAt(endReceptionTime, endJoinEvent);
    }
    else if (state == RECEIVE){
        state = RECEIVE2;
        emit(stateSignal, state);
        // schedule next sending
        simtime_t endReceptionTime = simTime() + msg->getSendingTime();
        // start of reception at recvStartTime
        cTimestampedValue tmp(recvStartTime, 1l);
        emit(receiveSignal, &tmp);
        // end of reception now
        emit(receiveSignal, 0);
        emit(receiveBeginSignal, receiveCounter);
        EV << "downlink-window-2 received"<<endl;
        //scheduleAt(endReceptionTime, endJoinEvent2);
    }
    else if (state == RECEIVE2){
        state = IDLE;
        emit(stateSignal, state);
        // schedule next sending
        scheduleAt(getNextTransmissionTime(), endRxEvent);
    }
    else {
        throw cRuntimeError("invalid state");
    }
}

simtime_t Host::getNextTransmissionTime()
{
    simtime_t t = simTime() + iaTime->doubleValue();

    if (!isSlotted)
        return t;
    else
        // align time of next transmission to a slot boundary
        return slotTime * ceil(t/slotTime);
}

void Host::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 2, "#808000");
    if (state == IDLE) {
        getDisplayString().setTagArg("i", 1, "");
        getDisplayString().setTagArg("t", 0, "");
    }
    else if (state == TRANSMIT) {
        getDisplayString().setTagArg("i", 1, "yellow");
        getDisplayString().setTagArg("t", 0, "TRANSMIT");
    }
}


}; //namespace
