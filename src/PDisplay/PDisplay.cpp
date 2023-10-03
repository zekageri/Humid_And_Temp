#include "PDisplay.h"

#include <Wire.h>
#include <U8x8lib.h>

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void PDisplay::init() {
    Wire.begin(SDAA, SCLL);
    u8x8.begin();
    u8x8.setPowerSave(0);
    u8x8.setFont(u8x8_font_8x13_1x2_f);
}

void PDisplay::print(int x, int y, const char* message, ...){
    va_list args;
    va_start(args, message);
    vsprintf(printBuffer, message, args);
    va_end(args);

    u8x8.drawString(x,y,printBuffer);
}

void PDisplay::printDegreeSysmbol(){
    u8x8.drawUTF8(15,5,"°");
}

void PDisplay::scanForDisplay() {
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknow error at address 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
    } else {
        Serial.println("done\n");
    }
}