//
// Created by swood on 10/15/17.
//

#ifndef RPUISSD1306_H
#define RPUISSD1306_H

#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef _CPP_U8GLIB
#include <U8glib.h>
#endif

#ifndef RpiPower_h
#include <RpiPower.h>
#endif

#ifndef RpiRelays_h
#include "RpiRelays.h"
#endif

#define UI_VOFFSET 36
#define UI_HSPACING 5
#define UI_SWWID 10
#define UI_SWHIG 16

class RPUISSD1306 {
public:
    // constructors
    RPUISSD1306(U8GLIB_SSD1306_128X64 *_u, RpiRelays *_r);

    // public methods
    int setSelected(uint8_t _s);
    void changeSelected(uint8_t _s);

    void drawSwitchNum(uint8_t swNum, int8_t state);

    void drawTest();

    void drawScreen();
    void drawSwitches();

    void buttonPressed();
    void buttonReleased();

    void clearUpper();
    void clearLower();
    void clearSwitchArea(uint8_t sNum);

private:
    // private variables
    U8GLIB_SSD1306_128X64 *_u8g;
    RpiRelays *_rpRelays;

    // private methods
    void u8gDef();

    uint8_t getSwHOffset(uint8_t sNum);

    void renderSelectedInfo(uint8_t sNum, uint8_t ci);

    void renderDivider(uint8_t ci);

    void renderSwitchSelected(uint8_t sNum, uint8_t ci);

    void renderSwitchLabel(uint8_t sNum, uint8_t ci);

    void renderSwitchFrame(uint8_t sNum, uint8_t ci);

    void renderSwitchToggle(uint8_t sNum, bool state, uint8_t ci);

    void renderButtonInner(bool pressed, uint8_t ci);

    void renderButtonOuter(uint8_t ci);

    void clearRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

};


#endif //RPUISSD1306_H
