#include "DisplayController.h"

DisplayController::DisplayController(uint8_t sdaPin, uint8_t sclPin) 
    : _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1), 
      _sdaPin(sdaPin), _sclPin(sclPin), _initialized(false), _totalLines(0) {}

void DisplayController::begin() {
    Wire.begin(_sdaPin, _sclPin); 
    if (_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        _initialized = true;
        _display.clearDisplay();
        _display.display();
        printLine("OLED Pronto!");
    } else {
        Serial.println("[OLED] Erro: Falha ao encontrar o Display!");
    }  
}

void DisplayController::printLine(String text) {
    Serial.println("[OLED] " + text); // Espelha no monitor serial

    if (!_initialized) return;

    // Gerencia o scroll do histórico de linhas
    if (_totalLines >= MAX_LINHAS) {
        for (int i = 0; i < MAX_LINHAS - 1; i++) {
            _history[i] = _history[i + 1];
        }
        _history[MAX_LINHAS - 1] = text;
    } else {
        _history[_totalLines] = text;
        _totalLines++;
    }

    // Renderiza na tela física
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    
    for (int i = 0; i < _totalLines; i++) {
        _display.setCursor(0, i * 8); 
        _display.println(_history[i]);
    }
    _display.display();
}

void DisplayController::clear() {
    if (!_initialized) return;
    _totalLines = 0;
    _display.clearDisplay();
    _display.display();
}
