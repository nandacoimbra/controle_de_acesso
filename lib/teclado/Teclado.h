#ifndef teclado_h
#define teclado_h

#include <Arduino.h>
#include "I2CKeyPad.h"
#include "Wire.h"
#include "Display.h"


class Teclado
{
private:
public:
    static const char keys[];
    String digitosArmazenados;
    // endereco padrao PCF8574
    const uint8_t KEYPAD_ADDRESS = 0x20;
    I2CKeyPad keyPad;
    byte lastKeyPressed;
    Teclado();
    void setupKeypad(void);
    char teclaPressionada(void);
    void armazenaDigito(char tecla);
    void limpaDigitosArmazenados(void);
};

#endif
