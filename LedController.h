#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

class LedController {
private:
    uint8_t _pin;
    bool _blinkActive;
    bool _ledState;
    unsigned long _lastToggle;
    unsigned long _blinkInterval;

public:
    LedController(uint8_t pin);
    void begin();
    void update();               // Executa o blink assíncrono no loop
    
    void turnOn();               // Liga o LED fixo
    void turnOff();              // Desliga o LED fixo
    void setBlink(unsigned long interval); // Ativa o piscar contínuo
    void flashBlocking(int count, int delayMs); // Pisca de forma bloqueante (LED_PISCA)
};

#endif
