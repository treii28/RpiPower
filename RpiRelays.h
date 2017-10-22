//
// Created by swood on 10/16/17.
//

#ifndef RpiRelays_h
#define RpiRelays_h

#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef RpiPower_h
#include <RpiPower.h>
#endif


class RpiRelays {
public:
    RpiRelays();

    RpiPower relays[];
    uint8_t selected = 0;

    String getInfoJSON();
    int setSelected(uint8_t _s) { selected = _s; return selected; };
    void defineRelay(uint8_t rId, uint8_t pin, uint8_t id1, uint8_t id2, const char *id, const char *name) {
        relays[rId] = RpiPower(pin, id1, id2, id, name);
    };
    void setRelay(uint8_t rId, RpiPower _rPwr) {
        relays[rId] = _rPwr;
        relays[rId].setDefined();
    };
    void clearRelay(uint8_t rId);
    uint8_t countRelays();

    void setName(const char *n) { strncpy(_name, n, 15); }
    void copyName(char *output) {strncpy(output, _name, 15); }
private:
    char _name[16];
    void shiftRelays();
};


#endif //RpiRelays_h
