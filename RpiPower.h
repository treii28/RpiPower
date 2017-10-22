/*
  RpiPower.h - Library defining a Raspberry Pi power connection
  Created by Scott Webster Wood, August 7, 2016
  Released into the public domain.
*/
#ifndef RpiPower_h
#define RpiPower_h

#if defined(ARDUINO) && ARDUINO >= 100

#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#define SEG_DIP_ON  0b01100011
#define SEG_DIP_OFF 0b01011100

class RpiPower {
public:
    RpiPower(uint8_t pin, uint8_t id1, uint8_t id2, const char * id, const char *name);

    void setPin(uint8_t p)      { _pin = p; };
    uint8_t getPin()            { return _pin; };

    void setId(const char *i)   { strncpy(_id, i, 2); };
    void copyId(char *i)        { strncpy(i, (char*)_id, 2); };

    void setIdc1(uint8_t d)     { _idc2 = d; };
    uint8_t getIdc1()           { return _idc1; };

    void setIdc2(uint8_t d)     { _idc2 = d; };
    uint8_t getIdc2()           { return _idc2; };

    void setName(const char *n) { strncpy(_name, n, 8); };
    void copyName(char *n)      { strncpy(n, _name, 8); };

    String getInfoJson();

    uint8_t getStatusDigit();

    void initRelay(uint8_t rPin, bool rState);

    void turnRelayOn();

    void turnRelayOff();

    void toggleRelay();

    bool getRelayState() { return _relayState; };
    bool relayIsOn() { return (_relayState == true); };
    bool relayIsOff() { return (_relayState == false); };
    void setDefined() { _def = true; };
    void setUndefined() { _def = false; };
    bool isDefined() { return _def; };
private:
    bool _def = true;
    uint8_t _pin;      // relay pin
    uint8_t _idc1;     // first character id to display
    uint8_t _idc2;     // second character id to display
    char _id[3];
    char _name[9];     // user friendly name
    bool _relayState;
};

#endif