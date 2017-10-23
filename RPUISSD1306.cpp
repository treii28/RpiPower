//
// Created by swood on 10/15/17.
//

#ifndef Arduino_h
#include <Arduino.h>
#endif

#ifndef _CPP_U8GLIB
#include <U8glib.h>
#endif

#ifndef RpiPower_h
#include <RpiPower.h>
#endif

#ifndef RPUISSD1306_H
#include "RPUISSD1306.h"
#endif

// constructors
RPUISSD1306::RPUISSD1306(U8GLIB_SSD1306_128X64 *_u) {
    _u8g = _u;
    u8gDef();
    _u8g->begin();
}

// public methods

void RPUISSD1306::clearRelay(uint8_t rId) {
    rpRelays[rId] = new RpiPower();
    shiftRelays();
}

uint8_t RPUISSD1306::countRelays() {
    uint8_t _i;
    uint8_t cnt = 0;
    for(_i = 0; _i<6; _i++) {
        if(rpRelays[_i]->isDefined()) {
            cnt++;
        }
    }
    return cnt;
}
void RPUISSD1306::shiftRelays() {
    uint8_t _i;
    int8_t last = -1;
    for(_i=0;_i<6;_i++) {
        if(rpRelays[_i]->isDefined()) {
            if(last != -1) {
                rpRelays[last] = rpRelays[_i];
                rpRelays[last] = new RpiPower();
                last++;
            }
        } else {
            last = _i;
        }
    }
}

void RPUISSD1306::changeSelected(uint8_t _s) {
    // remove any current selection indicator
    renderSwitchSelected(_selected, 0);
    renderSelectedInfo(_selected, 0);
    setSelected(_s);
    renderSwitchSelected(_selected, 1);
    renderSelectedInfo(_selected, 1);
}

String RPUISSD1306::getInfoJSON() {
    String output = "{";
    output += "count:" + String(countRelays()) + ",selected:" + String(_selected);

    output += ",relays:[";
    uint8_t _i;
    uint8_t _cnt = 0;
    for (_i = 0; _i < 6; _i++) {
        if (rpRelays[_i]->isDefined()) {
            _cnt++;
            output += (_cnt > 1) ? "," : "";
            output += rpRelays[_i]->getInfoJSON();
        }
    }

    output += "]}";
    return output;
}


void RPUISSD1306::drawTest() {
    _u8g->firstPage();
    do {
        _u8g->setColorIndex(1);
        _u8g->setFont(u8g_font_timB10);
        _u8g->drawStr(5, 32, "starting...");

        _u8g->drawLine(5, 28, 123, 28);
    } while( _u8g->nextPage() );

}

void RPUISSD1306::drawScreen() {
    _u8g->firstPage();
    do {
        renderSelectedInfo(_selected, 1);
        renderDivider(1);
        renderButtonOuter(1);
        renderButtonInner(false, 1);
        drawSwitches();
        renderButtonOuter(1);
        renderButtonInner(false, 1);
    } while( _u8g->nextPage() );
};

void RPUISSD1306::drawSwitches() {
    clearLower();
    uint8_t i;
    for(i=0; i<6; i++) {
        if(rpRelays[i]->isDefined()) {
            renderSwitchFrame(i, 1);
            renderSwitchToggle(i, rpRelays[i]->getRelayState(), 1);
            renderSwitchLabel(i, 1);
            if(i == _selected) {
                renderSwitchSelected(i, 1);
            }
        } else {
            Serial.println("Relay " + String(i) + " undefined");
        }
    }
}

void RPUISSD1306::buttonPressed() {
    renderButtonInner(true, 1);
}

void RPUISSD1306::buttonReleased() {
    renderButtonInner(false, 1);
}

void RPUISSD1306::toggleRelay(uint8_t _r) {
    rpRelays[_r]->toggleRelay();
    renderSelectedInfo(_selected, 0);
    renderSwitchToggle(_r, rpRelays[_r]->getRelayState(), 1);
    renderSelectedInfo(_selected, 1);
}
void RPUISSD1306::clearUpper() {
    clearRectangle(0,0,127,27);
}
void RPUISSD1306::clearLower() {
    clearRectangle(0,29,127,63);
}
void RPUISSD1306::clearSwitchArea(uint8_t sNum) {
    clearRectangle(UI_VOFFSET - 1, getSwHOffset(sNum) -1, UI_SWWID + 2, UI_SWHIG + 2);
}

// private methods
void RPUISSD1306::u8gDef() {
    // assign default color value
    if (_u8g->getMode() == U8G_MODE_R3G3B2) {
        _u8g->setColorIndex(255);     // white
    } else if (_u8g->getMode() == U8G_MODE_GRAY2BIT) {
        _u8g->setColorIndex(3);         // max intensity
    } else if (_u8g->getMode() == U8G_MODE_BW) {
        _u8g->setColorIndex(1);         // pixel on
    } else if (_u8g->getMode() == U8G_MODE_HICOLOR) {
        _u8g->setHiColorByRGB(255, 255, 255);
    }
}

uint8_t RPUISSD1306::getSwHOffset(uint8_t sNum) {
    return UI_HSPACING + (sNum * (UI_SWWID + UI_HSPACING));
}

void RPUISSD1306::renderSelectedInfo(uint8_t sNum, uint8_t ci) {
    _u8g->setColorIndex((ci == 1) ? 1 : 0);
    _u8g->setFont(u8g_font_timB10);

    delay(1000);

    String statusMessage = rpRelays[sNum]->getStatus();
    _u8g->drawStr(4, 18, statusMessage.c_str());
}

void RPUISSD1306::renderDivider(uint8_t ci) {
    _u8g->setColorIndex((ci == 1) ? 1 : 0);
    _u8g->drawLine(5, 28, 123, 28);
}

void RPUISSD1306::renderSwitchSelected(uint8_t sNum, uint8_t ci) {
    _u8g->setColorIndex((ci == 1) ? 1 : 0);
    _u8g->drawFrame(getSwHOffset(sNum) - 2, UI_VOFFSET - 2, UI_SWWID + 4, 10);
}

void RPUISSD1306::renderSwitchLabel(uint8_t sNum, uint8_t ci) {
    _u8g->setColorIndex((ci == 1) ? 1 : 0);
    _u8g->setFont(u8g_font_5x7);
    char id[3];
    rpRelays[sNum]->copyId(id);
    _u8g->drawStr(getSwHOffset(sNum), UI_VOFFSET + 6, id);
}

void RPUISSD1306::renderSwitchFrame(uint8_t sNum, uint8_t ci) {
    _u8g->setColorIndex((ci == 1) ? 1 : 0);
    _u8g->drawFrame(getSwHOffset(sNum), UI_VOFFSET + 10, UI_SWWID, UI_SWHIG);
}

void RPUISSD1306::renderSwitchToggle(uint8_t sNum, bool state, uint8_t ci) {
    // render top
    _u8g->setColorIndex((state && (ci == 1)) ? 1 : 0);
    _u8g->drawBox(getSwHOffset(sNum) + 2, UI_VOFFSET + 12, UI_SWWID - 4, UI_SWWID - 4);

    // render bottom
    _u8g->setColorIndex((!state && (ci == 1)) ? 1 : 0);
    _u8g->drawFrame(getSwHOffset(sNum) + 2, UI_VOFFSET + (2 + UI_SWHIG), UI_SWWID - 4, UI_SWWID - 4);
}

void RPUISSD1306::renderButtonInner(bool pressed, uint8_t ci) {
    // clear area of disc if drawing un-pressed state
    if(!pressed && (ci == 1)) {
        _u8g->setColorIndex(0);
        _u8g->drawDisc(114, 52, 7, U8G_DRAW_ALL);
    }

    _u8g->setColorIndex((ci == 1) ? 1 : 0);

    if(pressed) {
        _u8g->drawDisc(114, 52, 7, U8G_DRAW_ALL);
    } else {
        _u8g->drawCircle(114, 52, 7, U8G_DRAW_ALL);
    }
}
void RPUISSD1306::renderButtonOuter(uint8_t ci) {
    _u8g->setColorIndex((ci == 1) ? 1 : 0);
    _u8g->drawCircle(114, 52, 10, U8G_DRAW_ALL);
}

void RPUISSD1306::clearRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    _u8g->setColorIndex(0);
    _u8g->drawBox(x, y, w, h);
}
