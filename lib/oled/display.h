
#ifndef display_h
#define display_h

#include <Arduino.h>
#include <I2CKeyPad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>
#include <iostream>
#include <TelaSaida.h>

// configurações oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class Display : public TelaSaida
{   
    private:
    Adafruit_SSD1306 display;
    
    public:
    Display();
    void displaySetup(void);
    void displayConfig(void);
    void telaInicial(void);
    void digitarId(void);
    void digitarSenha(void);
    void displayPrint (String);
    void menuTeclado(void);
    void desenhaTexto(String texto);
};

#endif
