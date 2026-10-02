#include "CommandHandler.h"
#include <WiFi.h>

CommandHandler::CommandHandler(LedController& ledController, BluetoothSerial& bluetoothSerial, DisplayController& displayController) 
    : _led(ledController), _bt(bluetoothSerial), _display(displayController) {}

void CommandHandler::execute(String cmd) {
    cmd.trim();
    
    // Imprime na tela o comando capturado
    _display.printLine(cmd);

    if (cmd == "LED_ON") {
        _led.turnOn();
        _bt.print("LED ligado\n");
    }
    else if (cmd == "LED_OFF") {
        _led.turnOff();
        _bt.print("LED desligado\n");
    }
    else if (cmd.startsWith("LED_PISCA")) {
        processLedFlash(cmd);
    }
    else if (cmd.startsWith("LED_BLINK")) {
        processLedBlink(cmd);
    }
    else if (cmd == "TEMP") {
        _bt.printf("CPU Temp: %.2f\n", temperatureRead());
    }
    else if (cmd == "CPU") {
        sendCpuInfo();
    }
    else if (cmd == "RAM") {
        sendRamInfo();
    }
    else if (cmd == "FLASH") {
        sendFlashInfo();
    }
    else if (cmd == "INIT") {
        _bt.printf("Motivo reset: %d\n", esp_reset_reason());
    }
    else if (cmd == "UPTIME") {
        _bt.printf("Uptime: %lu ms\n", millis());
    }
    else if (cmd == "MAC") {
        _bt.printf("MAC: %s\n", WiFi.macAddress().c_str());
    }
    else if (cmd == "NET_INFO") {
        sendNetworkInfo();
    }
    else {
        _bt.print("Comando desconhecido\n");
    }
}

// Métodos internos mantidos... (idênticos à versão anterior)
void CommandHandler::processLedFlash(const String& cmd) {
    int piscadas = 10; int tempo = 250;
    int p1 = cmd.indexOf(':'); int p2 = cmd.indexOf(':', p1 + 1);
    if (p1 > 0 && p2 > 0) { piscadas = cmd.substring(p1 + 1, p2).toInt(); tempo = cmd.substring(p2 + 1).toInt(); }
    _led.flashBlocking(piscadas, tempo);
    _bt.printf("LED piscou %d vezes com %d ms\n", piscadas, tempo);
}
void CommandHandler::processLedBlink(const String& cmd) {
    unsigned long intervalo = 500; int p = cmd.indexOf(':');
    if (p > 0) { intervalo = cmd.substring(p + 1).toInt(); }
    _led.setBlink(intervalo);
    _bt.printf("Blink iniciado (%lu ms)\n", intervalo);
}
void CommandHandler::sendCpuInfo() {
    _bt.printf("Modelo: %s\n", ESP.getChipModel());
    _bt.printf("Revisão: %d\n", ESP.getChipRevision());
    _bt.printf("Núcleos: %d\n", ESP.getChipCores());
    _bt.printf("CPU: %d MHz\n", ESP.getCpuFreqMHz());
    _bt.printf("RAM livre: %u bytes\n", ESP.getFreeHeap());
}
void CommandHandler::sendRamInfo() {
    _bt.printf("Heap livre: %u\n", ESP.getFreeHeap());
    _bt.printf("Menor heap livre: %u\n", ESP.getMinFreeHeap());
    _bt.printf("Maior bloco livre: %u\n", ESP.getMaxAllocHeap());
}
void CommandHandler::sendFlashInfo() {
    _bt.printf("Flash total: %u\n", ESP.getFlashChipSize());
    _bt.printf("Velocidade Flash: %u\n", ESP.getFlashChipSpeed());
    _bt.printf("Tamanho Sketch: %u\n", ESP.getSketchSize());
    _bt.printf("Espaco livre: %u\n", ESP.getFreeSketchSpace());
}
void CommandHandler::sendNetworkInfo() {
    _bt.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    _bt.printf("Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    _bt.printf("Mascara de rede: %s\n", WiFi.subnetMask().toString().c_str());
    _bt.printf("RSSI: %d dbm\n", WiFi.RSSI());
    _bt.printf("Nome da Rede: %s\n", WiFi.SSID().c_str());
}
