#include "LedController.h"

LedController::LedController(uint8_t pin) {
    _pin = pin;
    _blinkActive = false;
    _ledState = false;
    _lastToggle = 0;
    _blinkInterval = 500;
}

void LedController::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void LedController::update() {
    if (!_blinkActive) return;

    unsigned long currentMillis = millis();
    if (currentMillis - _lastToggle >= _blinkInterval) {
        _lastToggle = currentMillis;
        _ledState = !_ledState;
        digitalWrite(_pin, _ledState);
    }
}

void LedController::turnOn() {
    _blinkActive = false;
    _ledState = true;
    digitalWrite(_pin, HIGH);
}

void LedController::turnOff() {
    _blinkActive = false;
    _ledState = false;
    digitalWrite(_pin, LOW);
}

void LedController::setBlink(unsigned long interval) {
    _blinkInterval = interval;
    _blinkActive = true;
}

void LedController::flashBlocking(int count, int delayMs) {
    _blinkActive = false; // Interrompe o assíncrono para priorizar este
    for (int i = 0; i < count; i++) {
        digitalWrite(_pin, HIGH); delay(delayMs);
        digitalWrite(_pin, LOW);  delay(delayMs);
    }
}
