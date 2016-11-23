//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __ALOHA_SERVER_H_
#define __ALOHA_SERVER_H_

#include <omnetpp.h>
int winCounter = 1;
using namespace omnetpp;

namespace aloha {

/**
 * Aloha server; see NED file for more info.
 */
class Server : public cSimpleModule
{
  private:
    // state variables, event pointers
    bool channelBusy;
    cMessage *endRxEvent;
    cMessage *endJoinEvent;
    cMessage *endJoinEvent2;
    cMessage *startRxEvent;
    cMessage *startRx2Event;
    cMessage *ack1;
    cMessage *ack2;
    cMessage *pair;
    cMessage *uplink;
    cMessage *payload;
    cMessage *ackpayload;
    cMessage *ackuplink;
    cMessage *ack0;
    cMessage *ackpayload2;
    long currentCollisionNumFrames;
    long receiveCounter;
    simtime_t recvStartTime;
    enum { IDLE = 0, TRANSMISSION = 1, COLLISION = 2 };
    simsignal_t channelStateSignal;
    // statistics
    simsignal_t receiveBeginSignal;
    simsignal_t receiveSignal;
    simsignal_t collisionLengthSignal;
    simsignal_t collisionSignal;

    simtime_t radioDelay;
    double txRate;
    cPar *iaTime;
    cPar *pkLenBits;
    cModule *host;
    simtime_t slotTime;
    simtime_t delay;
    bool isSlotted;
    int pkCounter;
    int counter;
    int randomnumber;

  public:
    Server();
    virtual ~Server();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    simtime_t getNextTransmissionTime();
    virtual void finish() override;
    virtual void refreshDisplay() const override;
};

}; //namespace

#endif

