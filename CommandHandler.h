#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "LedController.h"
#include "DisplayController.h" // Inclui o novo controlador

class CommandHandler {
private:
    LedController& _led;
    BluetoothSerial& _bt;
    DisplayController& _display; // Armazena a referência do display

    void processLedFlash(const String& cmd);
    void processLedBlink(const String& cmd);
    void sendCpuInfo();
    void sendRamInfo();
    void sendFlashInfo();
    void sendNetworkInfo();

public:
    // Construtor atualizado recebendo o display
    CommandHandler(LedController& ledController, BluetoothSerial& bluetoothSerial, DisplayController& displayController);

    void execute(String cmd);
};

#endif
