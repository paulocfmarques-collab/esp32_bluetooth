#include "BluetoothSerial.h"
#include <WiFi.h>

BluetoothSerial SerialBT;

#define LED 2

bool blinkAtivo = false;
bool estadoLed = false;

unsigned long ultimoToggle = 0;
unsigned long intervaloBlink = 500; // ms

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  SerialBT.begin("ESP32_BT");
}

void loop() {

  if (SerialBT.available()) {

    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    executa_comando(cmd);
  }

    // Controle do Blink assíncrono
  if (blinkAtivo) 
  {
    unsigned long agora = millis();
    if (agora - ultimoToggle >= intervaloBlink) 
    {
      ultimoToggle = agora;
      estadoLed = !estadoLed;
      digitalWrite(LED, estadoLed);
    }
  }  
}

void executa_comando(String cmd)
{

 if (cmd == "LED_ON") // Comando para ligar o LED
  {
    blinkAtivo = false;
    estadoLed = true;
    digitalWrite(LED, HIGH);

    SerialBT.print("LED ligado\n");
  }
  else if (cmd == "LED_OFF") // Comando para desligar o LED
  {
    blinkAtivo = false;
    estadoLed = false;
    digitalWrite(LED, LOW);

    SerialBT.print("LED desligado\n");
  }
  else if (cmd.startsWith("LED_PISCA")) // Comando para piscar o LED uma quantidade de vezes
  {
    int piscadas = 10;
    int tempo = 250;

    int p1 = cmd.indexOf(':');
    int p2 = cmd.indexOf(':', p1 + 1);

    if (p1 > 0 && p2 > 0) 
    {
      piscadas = cmd.substring(p1 + 1, p2).toInt();
      tempo = cmd.substring(p2 + 1).toInt();
    }

    for (int i = 0; i < piscadas; i++) 
    {
      digitalWrite(LED, HIGH);  delay(tempo);
      digitalWrite(LED, LOW);   delay(tempo);
    }

    SerialBT.printf("LED piscou %d vezes com %d ms\n", piscadas, tempo);
  }
  else if (cmd.startsWith("LED_BLINK")) // Comando para piscar led com tempo
  {
    int p = cmd.indexOf(':');

    if (p > 0) 
    {
      intervaloBlink = cmd.substring(p + 1).toInt();
    }

    blinkAtivo = true;

    SerialBT.printf("Blink iniciado (%lu ms)\n", intervaloBlink);
  }
  else if (cmd == "TEMP") // Comando ler a temperatura
  {
    SerialBT.printf("CPU Temp: %.2f\n", temperatureRead());
  }
  else if (cmd == "CPU") // Informações sobre a CPU
  {
    SerialBT.printf("Modelo: %s\n", ESP.getChipModel());
    SerialBT.printf("Revisão: %d\n", ESP.getChipRevision());
    SerialBT.printf("Núcleos: %d\n", ESP.getChipCores());
    SerialBT.printf("CPU: %d MHz\n", ESP.getCpuFreqMHz());
    SerialBT.printf("RAM livre: %u bytes\n", ESP.getFreeHeap());
  }
  else if (cmd == "RAM") // Informações sobre a RAM
  {
    SerialBT.printf("Heap livre: %u\n", ESP.getFreeHeap());
    SerialBT.printf("Menor heap livre: %u\n", ESP.getMinFreeHeap());
    SerialBT.printf("Maior bloco livre: %u\n", ESP.getMaxAllocHeap());
  }
  else if (cmd == "FLASH") // Informações sobre a flash
  {
    SerialBT.printf("Flash total: %u\n", ESP.getFlashChipSize());
    SerialBT.printf("Velocidade Flash: %u\n", ESP.getFlashChipSpeed());
    SerialBT.printf("Tamanho Sketch: %u\n", ESP.getSketchSize());
    SerialBT.printf("Espaco livre: %u\n", ESP.getFreeSketchSpace());
  }
  else if (cmd == "INIT") // Motivo do reset
  {
    SerialBT.printf("Motivo reset: %d\n", esp_reset_reason());    
  }
  else if (cmd == "UPTIME") // Tempo ligado
  {
    SerialBT.printf("Uptime: %lu ms\n", millis());    
  }
  else if (cmd == "MAC") // MAC address
  {
    SerialBT.printf("MAC: ");
    SerialBT.println(WiFi.macAddress());
  }
  else if (cmd == "NET_INFO") // MAC address
  {
    SerialBT.printf("IP: ");
    SerialBT.println(WiFi.localIP());
    SerialBT.printf("Gateway: ");
    SerialBT.println(WiFi.gatewayIP());
    SerialBT.printf("Mascara de rede: ");
    SerialBT.println(WiFi.subnetMask());
    SerialBT.printf("RSSI: %d dbm\n", WiFi.RSSI());
    SerialBT.printf("Nome da Rede: %s\n", WiFi.SSID());
  }
  else // Comando invalido
  {
    SerialBT.print("Comando desconhecido\n");
  }
}
