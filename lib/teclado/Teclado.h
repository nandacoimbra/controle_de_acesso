#ifndef teclado_h
#define teclado_h

#include <Arduino.h>
#include "I2CKeyPad.h"
#include "Wire.h"
#include "Display.h"

// teclado e oled usam esses pinos
#define SDA_PIN 22
#define SCL_PIN 23

class Teclado
{
private:
public:
    static const char keys[];
    // endereco padrao PCF8574
    const uint8_t KEYPAD_ADDRESS = 0x20;
    I2CKeyPad keyPad;
    byte lastKeyPressed;
    Display displayTeclado;
    Teclado();
    Teclado(Display &display);
    void setupKeypad(void);
    char teclaPresionada(void);
    void armazenaDigitos(void);
};

#endif
