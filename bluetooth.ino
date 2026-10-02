#include "BluetoothSerial.h"
#include "LedController.h"
#include "DisplayController.h"
#include "CommandHandler.h"

#define LED_PIN  2
#define PIN_SDA 21
#define PIN_SCL 22

BluetoothSerial SerialBT;
LedController led(LED_PIN);
DisplayController oled(PIN_SDA, PIN_SCL); // Instancia o OLED nos pinos corretos

// Injeta as 3 instâncias necessárias no resolvedor de comandos
CommandHandler parser(led, SerialBT, oled);

void setup() {
  Serial.begin(115200);
  
  // Inicialização encapsulada de cada hardware
  led.begin();
  oled.begin(); 
  
  SerialBT.begin("ESP32_BT");
  
  oled.printLine("BT Ativo!");
  Serial.println("Sistema POO com OLED Inicializado.");
}

void loop() {
  // Escuta comandos Bluetooth
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    parser.execute(cmd);
  }

  // Atualiza tarefas assíncronas do LED (Blink)
  led.update();
}
