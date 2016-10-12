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
    pkCounter = 0;
    winCounter = 1;
    emit(receiveSignal, 0L);
    emit(receiveBeginSignal, 0L);
}

void Server::handleMessage(cMessage *msg)
{
    if (msg == endRxEvent || msg == endJoinEvent || msg == endJoinEvent2) {
        EV << "reception finished\n";
        channelBusy = false;
        emit(channelStateSignal, IDLE);


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
        receiveCounter = 0;
        emit(receiveBeginSignal, receiveCounter);
        cancelEvent(endJoinEvent);
        cancelEvent(endJoinEvent2);
        //delete pkt;
    }
    else {
        cPacket *pkt = check_and_cast<cPacket *>(msg);

        ASSERT(pkt->isReceptionStart());
        simtime_t endReceptionTime = simTime() + pkt->getDuration();


        emit(receiveBeginSignal, ++receiveCounter);

        if (!channelBusy) {
            EV << "started receiving\n";
            recvStartTime = simTime();
            channelBusy = true;
            emit(channelStateSignal, TRANSMISSION);
            emit(receiveBeginSignal, ++receiveCounter);
            //envío primer mensaje downlink window
            char pkname2[40];
            char pkname3[40];

            host = pkt->getSenderModule(); //obtencion de id de nodo que envió mensaje
            sprintf(pkname2, "downlink-1-to host %d-#%d", pkt->getSenderModuleId(), pkCounter++);
            EV << "generating packet 1 to host " << pkt->getSenderModuleId() << endl;
            cPacket *pk2 = new cPacket(pkname2);  //creación downlink window #1 luego de uplink desde nodo
            pk2->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
            randomnumber = ((rand() % 20)+1)/1000; // generación de tiempo aleatorio de diferencia con recepción
            simtime_t duration = (simTime() + SimTime(randomnumber, SIMTIME_US)).trunc(SIMTIME_MS); // asignacion de duracion de envio de paquete
            EV << "duration: "<<duration << endl;
            sendDirect(pk2, radioDelay, duration, host->gate("in")); // envío de mensaje a nodo
            //cancelEvent(endJoinEvent);
            scheduleAt(simTime()+duration, endJoinEvent);
            EV << "downlink window "<<winCounter<<" to host "<< pkt->getSenderModuleId() <<" is ended in "<< duration << endl;
            winCounter+=1;

            sprintf(pkname3, "downlink-2-to host %d-#%d", pkt->getSenderModuleId(), pkCounter++);
            EV << "generating packet 2 to host" << pkt->getSenderModuleId() << endl;
            cPacket *pk3 = new cPacket(pkname3);  //creación downlink window #1 luego de uplink desde nodo
            pk3->setBitLength(pkLenBits->longValue()); // asignación de largo de paquete (256 bytes)
            srand ( time(NULL) );
            randomnumber = ((rand() % 20)+1)/1000; // generación de tiempo aleatorio de diferencia con recepción
            simtime_t duration2 = (simTime() + SimTime(randomnumber, SIMTIME_US)).trunc(SIMTIME_MS); // asignacion de duracion de envio de paquete
            EV << "duration2: "<<duration2 << endl;
            sendDirect(pk3, radioDelay, duration2, host->gate("in")); // envío de mensaje a nodo
            //cancelEvent(endJoinEvent2);

            scheduleAt(simTime()+duration2, endJoinEvent2);
            EV << "downlink window "<<winCounter<<" to host "<< pkt->getSenderModuleId() <<" is ended in "<< duration2 << endl;
            winCounter = 1;

            //pkCounter++;
            //envío segundo mensaje downlink window
            //if (pkCounter ==2){
            /*
            cPacket *pk3 = new cPacket(pkname3);
            pk3->setBitLength(pkLenBits->longValue());
            randomnumber = (rand() % 20)+1;
            simtime_t duration2 = (simTime() + SimTime(randomnumber, SIMTIME_US)).trunc(SIMTIME_MS);
            EV << "duration2: "<< duration2 << endl;
            EV << "duration2 + simtime: "<< simTime()<<endl;
            sendDirect(pk2, radioDelay, duration2, host->gate("in"));
            scheduleAt(simTime()+duration2, endJoinEvent2);
            */
            //pkCounter = 0;
           // }
            //termino envio mensaje

            scheduleAt(endReceptionTime, endRxEvent);
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
