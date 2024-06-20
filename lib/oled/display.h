
#ifndef display_h
#define display_h

#include <Arduino.h>
#include <I2CKeyPad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>
#include <iostream>

class Display
{   
    public:
    Display();
    void displaySetup(void);
    void displayConfig(void);
    void telaInicial(void);
    void digitarId(void);
    void displayPrint (String);
};

#endif
