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
    pair = 0;
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
    ack1 = new cMessage("ack-downlink-1");
    ack2 = new cMessage("ack-downlink-2");
    uplink = new cMessage("uplink");
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
    const char *msgtxt = msg->getFullName();
    if (state == IDLE && pair == 0) {
        // generate packet and schedule timer when it ends
        char pkname[40];
        sprintf(pkname, "pair-request", getId(), pkCounter++);
        EV << "generating packet " << pkname << endl;

        state = TRANSMIT;
        emit(stateSignal, state);

        cPacket *pk = new cPacket(pkname);
        pk->setBitLength(pkLenBits->longValue());
        simtime_t duration = pk->getBitLength() / txRate;
        // is set for default the SF7 for the first communication so, i put the gate 6 for that use in the simulator.
        sendDirect(pk, radioDelay, duration, server->gate("in7"));
        scheduleAt(simTime()+duration, endTxEvent);
        cancelEvent(endJoinEvent);
        cancelEvent(endJoinEvent2);

        gate("in")->setDeliverOnReceptionStart(true);
    }
    else if (state == TRANSMIT && strcmp(msgtxt,"downlink-1") == 0 ) {
        // endTxEvent indicates end of transmission
        state = RECEIVE;

        emit(stateSignal, state);
        char pkname[40];
        sprintf(pkname, "ack-downlink-1", getId(), pkCounter++);
        // schedule next sending
        simtime_t endReceptionTime = simTime() + msg->getSendingTime();
        // start of reception at recvStartTime
        cTimestampedValue tmp(recvStartTime, 1l);
        emit(receiveSignal, &tmp);
        emit(receiveSignal, 0);
        emit(receiveBeginSignal, receiveCounter);
        // end of reception now
        cPacket *pk = new cPacket(pkname);
        pk->setBitLength(pkLenBits->longValue());
        simtime_t duration = pk->getBitLength() / txRate;
        // is set for default the SF7 for the first communication so, i put the gate 6 for that use in the simulator.
        cancelEvent(ack1);
        sendDirect(pk, radioDelay+10, duration+0.000020, server->gate("in7"));
        scheduleAt(simTime()+duration, ack1);

        EV << "downlink-window-1 received"<<endl;
        //cancelEvent(endJoinEvent);
        //scheduleAt(endReceptionTime+0.000020, endJoinEvent);
    }
    else if (state == RECEIVE && strcmp(msgtxt,"downlink-2") == 0 ){

        state = RECEIVE2;
        emit(stateSignal, state);
        char pkname[40];
        sprintf(pkname, "ack-downlink-2", getId(), pkCounter++);
        // schedule next sending
        simtime_t endReceptionTime = simTime() + msg->getSendingTime() + 0.000020;
        // start of reception at recvStartTime
        cTimestampedValue tmp(recvStartTime, 1l);
        emit(receiveSignal, &tmp);
        emit(receiveSignal, 0);
        emit(receiveBeginSignal, receiveCounter);
        // end of reception now
        cPacket *pk = new cPacket(pkname);
        pk->setBitLength(pkLenBits->longValue());
        simtime_t duration = pk->getBitLength() / txRate;
        // is set for default the SF7 for the first communication so, i put the gate 6 for that use in the simulator.
        cancelEvent(ack2);
        sendDirect(pk, radioDelay+10, duration+0.000020, server->gate("in7"));
        scheduleAt(simTime()+duration, ack2);

        EV << "downlink-window-2 received"<<endl;
        //cancelEvent(endJoinEvent2);
        //scheduleAt(endReceptionTime, endJoinEvent2);
    }
    else if (state == RECEIVE2 ){
        state = IDLE2;
        emit(stateSignal, state);
        // schedule next sending
        cancelEvent(endRxEvent);
        scheduleAt(getNextTransmissionTime(), endRxEvent);
        this->setpair(1);
    }
    else if (state == IDLE2 && this->getpair() == 1 ){
        //inicio de fase de transmisión
        char pkname4[40];
        sprintf(pkname4, "uplink-req");
        EV << "generating uplink-request to gateway" << endl;
        cPacket *pk4 = new cPacket(pkname4);  //creación downlink window #1 luego de uplink desde nodo
        pk4->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
        simtime_t duration = pk4->getBitLength() / txRate; // asignacion de duracion de envio de paquete
        cancelEvent(uplink);
        sendDirect(pk4, radioDelay, duration, server->gate("in7"));
        if (simTime() >= duration){
            cancelEvent(uplink);
            sendDirect(pk4, radioDelay, duration, server->gate("in7"));
            EV << "packet retransmited" << endl;
        }
        scheduleAt(simTime()+duration, uplink);
    }
    else {
        EV <<"state final: "<< state << endl;
        throw cRuntimeError("invalid state");
    }
}

int Host::getpair()
{
    return this->pair;
}
void Host::setpair(int a){
    this->pair = a;
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
