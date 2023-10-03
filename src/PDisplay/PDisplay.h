#pragma once

#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C 

#define SCLL 26
#define SDAA 27

class PDisplay{
    public:
        void init();
        void print(int x, int y, const char* message, ...);
        void printDegreeSysmbol();
    private:
        char printBuffer[1024];
        void scanForDisplay();
};

inline PDisplay pDisplay;