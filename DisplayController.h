#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayController {
private:
    static const uint8_t SCREEN_WIDTH = 128;
    static const uint8_t SCREEN_HEIGHT = 64;
    static const uint8_t MAX_LINHAS = 8;

    Adafruit_SSD1306 _display;
    uint8_t _sdaPin;
    uint8_t _sclPin;
    bool _initialized;
    
    String _history[MAX_LINHAS];
    int _totalLines;

public:
    DisplayController(uint8_t sdaPin = 21, uint8_t sclPin = 22);
    
    void begin();
    void printLine(String text);
    void clear();
};

#endif
