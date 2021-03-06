/*
  RpiPower.cpp - Library defining a Raspberry Pi power connection
  Created by Scott Webster Wood, August 7, 2016
  Released into the public domain.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else

#include "WProgram.h"

#endif

#include "RpiPower.h"
RpiPower::RpiPower() {
    initRelay(0, false);
    _idc1 = 0b00000000;
    _idc2 = 0b00000000;
    setUndefined();
}

RpiPower::RpiPower(uint8_t pin, uint8_t id1, uint8_t id2, const char *id, const char *name) {
    initRelay(pin, false);
    _idc1 = id1;
    _idc2 = id2;
    strncpy(_id, id, 2);
    _id[2] = 0;
    strncpy(_name, name, 8);
    _name[8] = 0;
    setDefined();
}

String RpiPower::getInfoJSON() {
    String relInfo = "{pin:" + String(_pin) + ",idc1:" + String(_idc1) + ",idc2:" + String(_idc2) +
            ",id:\"" + String(_id) + "\",name:\"" + String(_name) + "\",state:" + String(_relayState) + "}";

    return relInfo;
};

String RpiPower::getStatus() {
    String _status = String(_name) + " is ";
    _status += (getRelayState()) ? "'on'" : "'off'";
    return _status;
}

uint8_t RpiPower::getStatusDigit() {
    return 0b01100011;
    if (_relayState == true) {
        return SEG_DIP_ON;
    } else {
        return SEG_DIP_OFF;
    }
}

// relay related methods
void RpiPower::initRelay(uint8_t rPin, bool rState) {
    _pin = rPin;
    pinMode(rPin, OUTPUT);
    if (rState != 0) {
        turnRelayOn();
    } else {
        turnRelayOff();
    }
}

void RpiPower::turnRelayOn() {
    digitalWrite(_pin, LOW);
    _relayState = true;
}

void RpiPower::turnRelayOff() {
    digitalWrite(_pin, HIGH);
    _relayState = false;
}

void RpiPower::toggleRelay() {
    if (_relayState == true) {
        turnRelayOff();
    } else {
        turnRelayOn();
    }
}

