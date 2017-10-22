//
// Created by swood on 10/16/17.
//


#ifndef RpiRelays_h
#include "RpiRelays.h"
#endif

RpiRelays::RpiRelays() {};

String RpiRelays::getInfoJSON() {
    String output = "{";
    output += "count:" + String(countRelays()) + ",selected:" + String(selected) + ",set:\"" + String(_name) +"\"";

    output += ",relays:[";
    uint8_t _i;
    uint8_t _cnt = 0;
    uint8_t rSize = sizeof(relays);
    if(countRelays() > 0) {
        for (_i = 0; _i < rSize; _i++) {
            if (relays[_i].isDefined()) {
                _cnt++;
                String relInfo = (_cnt > 1) ? "," : "";
                output += relays[_i].getInfoJson();
            }
        }
    }

    output += "]}";
    return output;
}

void RpiRelays::clearRelay(uint8_t rId) {
    relays[rId].setUndefined();
    shiftRelays();
}

uint8_t RpiRelays::countRelays() {
    uint8_t _i;
    uint8_t cnt = 0;
    uint8_t rSize = sizeof(relays);
    for(_i = 0; _i<rSize; _i++) {
        if(relays[_i].isDefined()) {
            cnt++;
        }
    }
    return cnt;
}
void RpiRelays::shiftRelays() {
    uint8_t _i;
    int8_t last = -1;
    uint8_t rSize = sizeof(relays);
    for(_i=0;_i<rSize;_i++) {
        if(relays[_i].isDefined()) {
            if(last != -1) {
                relays[last] = relays[_i];
                relays[last] = RpiPower(0, 0, 0, "", "");
                relays[last].setUndefined();
                last++;
            }
        } else {
            last = _i;
        }
    }
}