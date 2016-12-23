//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <time.h>
namespace aloha {

Define_Module(Server);

Server::Server()
{

    endRxEvent = nullptr;
    counter = 0;
    errorcounter = 0;
    errorpcounter2 = 0;
}

Server::~Server()
{
    cancelAndDelete(endRxEvent);
}



void Server::initialize()
{

    channelStateSignal = registerSignal("channelState");
    endJoinEvent = new cMessage("end-downlink-window1");
    endJoinEvent2 = new cMessage("end-downlink-window2");
    endRxEvent = new cMessage("end-reception");
    startRxEvent = new cMessage("downlink-1");
    startRx2Event = new cMessage("downlink-2");
    ack0 = new cMessage("ack-join-request");
    ack1 = new cMessage("ack-downlink-1");
    ack2 = new cMessage("ack-downlink-2");
    uplink = new cMessage("uplink-req");
    payload = new cMessage("uplink-payload");
    pair = new cMessage("pair-request");
    ackpayload = new cMessage("ack-payload");
    ackpayload2 = new cMessage("ack-payload-2");
    waits = new cMessage("waittime");
    errors = new cMessage("error-package");
    channelBusy = false;
    emit(channelStateSignal, IDLE);
    gate("in")->setDeliverOnReceptionStart(true);
    currentCollisionNumFrames = 0;
    receiveCounter = 0;
    WATCH(currentCollisionNumFrames);
    pkLenBits = &par("pkLenBits");
    iaTime = &par("iaTime");
    txRate = par("txRate");
    slotTime = par("slotTime");
    receiveBeginSignal = registerSignal("receiveBegin");
    receiveSignal = registerSignal("receive");
    collisionSignal = registerSignal("collision");
    collisionLengthSignal = registerSignal("collisionLength");
    errorpSignal = registerSignal("errorpFrame");
    pkCounter = 0;
    emit(receiveSignal, 0L);
    emit(receiveBeginSignal, 0L);
    per= par("per");
}

void Server::handleMessage(cMessage *msg)
{
    EV<<"error: "<<this->errorcounter/this->receiveCounter*100<<endl;
    EV << "receive counter: "<<this->receiveCounter<<endl;
    if ( (this->errorcounter/this->receiveCounter*100 < this->per || this->errorcounter == 0) && this->per != 0 ){
            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            emit(receiveSignal, 0);
            this->errorcounter++;
            this->errorpcounter2+=1;
            emit(errorpSignal, errorpcounter2);
            cancelEvent(errors);
            scheduleAt(simTime()+1, errors);


        }
    const char *msgtxt = msg->getFullName();
    EV << "msg-server-inicial: "<< msgtxt <<endl;
    if (counter ==  20){
                msgtxt = "ack-downlink-2";
                EV<<"retransmisión de ACK"<<endl;
                channelBusy = false;

            }
    if (counter > 20){
        msgtxt = "waittime";
        cTimestampedValue tmp(recvStartTime, 1l);
        emit(receiveSignal, &tmp);
        emit(receiveSignal, 0);
        //emit(receiveBeginSignal, receiveCounter);
        EV<<"retransmisión de ACK"<<endl;
        this->counter = 0;
    }

    if (strcmp(msgtxt, "end-reception") == 0 || strcmp(msgtxt, "ack-payload-2")==0 || strcmp(msgtxt, "ack-payload")==0 || strcmp(msgtxt, "downlink-1" ) == 0 || strcmp(msgtxt, "end-downlink-window1") == 0 || strcmp(msgtxt, "end-downlink-window2") == 0|| strcmp(msgtxt, "ack-downlink-1") == 0 || strcmp(msgtxt, "ack-downlink-2") == 0 || strcmp(msgtxt, "uplink-req") == 0 || strcmp(msgtxt, "uplink-payload") == 0 || strcmp(msgtxt, "waittime") == 0 || strcmp(msgtxt, "error-package")==0)
    {
        EV << "reception finished\n";

        if ( strcmp(msgtxt, "ack-downlink-1")==0 ){

            // start of reception at recvStartTime
            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            emit(receiveBeginSignal, ++receiveCounter);
            cancelEvent(ack1);
            scheduleAt(simTime()+5, ack1);
            counter ++;
            msgtxt = "waittime";
            msg->setName("waittime");
        }
        if (strcmp(msgtxt, "ack-downlink-2") == 0){

            // start of reception at recvStartTime
            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            emit(receiveBeginSignal, ++receiveCounter);
            cancelEvent(ack2);
            scheduleAt(simTime()+10, ack2);
            counter ++;
            msgtxt = "waittime";
            msg->setName("waittime");
        }

        channelBusy = false;
        emit(channelStateSignal, IDLE);
        //envío de los disintos ack que componen la comunicación con lora
        if ( strcmp(msgtxt,"uplink-req")==0){
            EV << "generating uplink-ack " << endl;

            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            emit(receiveBeginSignal, ++receiveCounter);
            cPacket *pkt = check_and_cast<cPacket *>(msg);
            ASSERT(pkt->isReceptionStart());

            char pkname6[40];
            sprintf(pkname6, "ack-uplink");
            host = pkt->getSenderModule(); //obtencion de id de nodo que envió mensaje
            EV << "generating uplink-ack to host "<< (pkt->getSenderModuleId()-3) << endl;
            cPacket *pk6 = new cPacket(pkname6);  //creación downlink window #1 luego de uplink desde nodo
            pk6->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
            simtime_t duration = simTime()+pk6->getBitLength()/txRate; // asignacion de duracion de envio de paquete
            cancelEvent(ackpayload);
            sendDirect(pk6, radioDelay, duration, host->gate("in"));
            scheduleAt(simTime()+duration, ackpayload);

        }
        if ( strcmp(msgtxt,"uplink-payload")==0){
            EV << "generating uplink-payload-ack " << endl;

            cTimestampedValue tmp(recvStartTime, 1l);

            emit(receiveSignal, &tmp);
            emit(receiveBeginSignal, ++receiveCounter);
            char pkname7[40];
            sprintf(pkname7, "ack-payload-2");
            cPacket *pkt = check_and_cast<cPacket *>(msg);
            ASSERT(pkt->isReceptionStart());
            host = pkt->getSenderModule(); //obtencion de id de nodo que envió mensaje
            EV << "generating payload-ack to host "<< (pkt->getSenderModuleId()-3) << endl;
            cPacket *pk7 = new cPacket(pkname7);  //creación downlink window #1 luego de uplink desde nodo
            pk7->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
            simtime_t duration = simTime()+pk7->getBitLength()/txRate; // asignacion de duracion de envio de paquete
            cancelEvent(ackpayload2);
            sendDirect(pk7, radioDelay, duration, host->gate("in"));
            scheduleAt(simTime()+duration, ackpayload2);
            msgtxt = "wait";

        }
        if ( strcmp(msgtxt, "waittime")==0 || strcmp(msgtxt, "ack-payload")==0 || strcmp(msgtxt, "ack-payload-2")==0){
            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            emit(receiveBeginSignal, ++receiveCounter);
            cancelEvent(waits);
            scheduleAt(simTime()+1, waits);
            EV << "en tiempo de espera"<<endl;

        }
        // update statistics
        simtime_t dt = simTime() - recvStartTime;
        if (currentCollisionNumFrames == 0) {
            // start of reception at recvStartTime
            cTimestampedValue tmp(recvStartTime, 1l);
            emit(receiveSignal, &tmp);
            // end of reception now
            emit(receiveSignal, 0);
        }
        else {
            // start of collision at recvStartTime
            cTimestampedValue tmp(recvStartTime, currentCollisionNumFrames);
            emit(collisionSignal, &tmp);

            emit(collisionLengthSignal, dt);
        }

        currentCollisionNumFrames = 0;
        //receiveCounter = 0;
        emit(receiveBeginSignal, receiveCounter);
        cancelEvent(startRxEvent);
        cancelEvent(startRx2Event);
        cancelEvent(ack0);
        cancelEvent(ack1);
        cancelEvent(uplink);
        cancelEvent(payload);
        //delete pkt;
    }
    else{

        cPacket *pkt = check_and_cast<cPacket *>(msg);

        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime = simTime() + pkt->getDuration();


        emit(receiveBeginSignal, ++receiveCounter);

        if (!channelBusy) {
                EV << "started receiving\n";
                recvStartTime = simTime();

                emit(channelStateSignal, TRANSMISSION);
                emit(receiveBeginSignal, ++receiveCounter);
                //envio de join-accept
                char pkname1[40];
                host = pkt->getSenderModule(); //obtencion de id de nodo que envió mensaje
                sprintf(pkname1, "join-accept-ack");
                EV << "generating join-accept-ack to host " << pkt->getSenderModuleId()-3 << endl;
                cPacket *pk1 = new cPacket(pkname1);  //creación downlink window #1 luego de uplink desde nodo
                pk1->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
                simtime_t duration1 = simTime() + pk1->getBitLength() / txRate; // asignacion de duracion de envio de paquete
                EV << "duration: "<<duration1 << endl;
                sendDirect(pk1, radioDelay, duration1, host->gate("in")); // envío de mensaje a nodo
                cancelEvent(ack0);
                scheduleAt(simTime()+duration1, ack0);

                //envío primer mensaje downlink window
                char pkname2[40];
                host = pkt->getSenderModule(); //obtencion de id de nodo que envió mensaje
                sprintf(pkname2, "downlink-1");
                EV << "generating packet 1 to host " << pkt->getSenderModuleId() << endl;
                cPacket *pk2 = new cPacket(pkname2);  //creación downlink window #1 luego de uplink desde nodo
                pk2->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
                randomnumber = ((rand() % 20)+1)/1000; // generación de tiempo aleatorio de diferencia con recepción
                simtime_t duration = (simTime() + SimTime(randomnumber, SIMTIME_US)).trunc(SIMTIME_MS); // asignacion de duracion de envio de paquete
                EV << "duration: "<<duration << endl;
                sendDirect(pk2, radioDelay, duration, host->gate("in")); // envío de mensaje a nodo
                cancelEvent(startRxEvent);
                scheduleAt(simTime()+duration, startRxEvent);
                EV << "downlink window "<<winCounter<<" to host "<< pkt->getSenderModuleId() <<" is ended in "<< duration << endl;
                winCounter+=1;


                char pkname3[40];
                sprintf(pkname3, "downlink-2");
                EV << "generating packet 2 to host" << pkt->getSenderModuleId() << endl;
                cPacket *pk3 = new cPacket(pkname3);  //creación downlink window #1 luego de uplink desde nodo
                pk3->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
                srand ( time(NULL) );
                randomnumber = ((rand() % 20)+1)/1000; // generación de tiempo aleatorio de diferencia con recepción
                simtime_t duration2 = (simTime() + SimTime(randomnumber, SIMTIME_US)).trunc(SIMTIME_MS); // asignacion de duracion de envio de paquete
                EV << "duration2: "<<duration2 << endl;
                sendDirect(pk3, radioDelay, duration2, host->gate("in")); // envío de mensaje a nodo
                cancelEvent(startRx2Event);
                scheduleAt(simTime()+duration2, startRx2Event);
                EV << "downlink window "<<winCounter<<" to host "<< pkt->getSenderModuleId() <<" is ended in "<< duration2 << endl;
                winCounter = 1;
                cancelEvent(endRxEvent);
                scheduleAt(endReceptionTime, endRxEvent);
                channelBusy = true;


        }
        else {
            EV << "another frame arrived while receiving -- collision!\n";
            emit(channelStateSignal, COLLISION);

            if (currentCollisionNumFrames == 0)
                currentCollisionNumFrames = 2;
            else
                currentCollisionNumFrames++;

            if (endReceptionTime > endRxEvent->getArrivalTime()) {
                cancelEvent(endRxEvent);
                scheduleAt(endReceptionTime, endRxEvent);
            }

            // update network graphics
            if (hasGUI()) {
                char buf[32];
                sprintf(buf, "Collision! (%ld frames)", currentCollisionNumFrames);
                bubble(buf);
            }
        }
        channelBusy = true;
        delete pkt;
    }
}

simtime_t Server::getNextTransmissionTime()
{
    simtime_t t = simTime() + iaTime->doubleValue();

    if (!isSlotted)
        return t;
    else
        // align time of next transmission to a slot boundary
        return slotTime * ceil(t/slotTime);
}

void Server::refreshDisplay() const
{
    if (!channelBusy) {
        getDisplayString().setTagArg("i2", 0, "status/off");
        getDisplayString().setTagArg("t", 0, "");
    }
    else if (currentCollisionNumFrames == 0) {
        getDisplayString().setTagArg("i2", 0, "status/yellow");
        getDisplayString().setTagArg("t", 0, "RECEIVE");
        getDisplayString().setTagArg("t", 2, "#808000");
    }
    else {
        getDisplayString().setTagArg("i2", 0, "status/red");
        getDisplayString().setTagArg("t", 0, "COLLISION");
        getDisplayString().setTagArg("t", 2, "#800000");
    }
}

void Server::finish()
{
    EV << "duration: " << simTime() << endl;

    recordScalar("duration", simTime());
}

}; //namespace
