//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#ifndef __ALOHA_HOST_H_
#define __ALOHA_HOST_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace aloha {

/**
 * Aloha host; see NED file for more info.
 */
class Host : public cSimpleModule
{
  private:
    // parameters
    simtime_t radioDelay;
    double txRate;
    cPar *iaTime;
    cPar *pkLenBits;
    simtime_t recvStartTime;
    simtime_t slotTime;
    simtime_t delay;
    bool isSlotted;
    long receiveCounter;


    // state variables, event pointers etc
    cModule *server;
    cMessage *endTxEvent;
    cMessage *endRxEvent;
    cMessage *endJoinEvent;
    cMessage *endJoinEvent2;
    enum { IDLE = 0, TRANSMIT = 1 , RECEIVE = 2, RECEIVE2=3} state;
    simsignal_t channelStateSignal;
    simsignal_t receiveBeginSignal;
    simsignal_t receiveSignal;
    simsignal_t stateSignal;
    int pkCounter;

  public:
    Host();
    virtual ~Host();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
    simtime_t getNextTransmissionTime();
};

}; //namespace

#endif

