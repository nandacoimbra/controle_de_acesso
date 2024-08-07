#pragma once

#include <Arduino.h>

class TelaSaida
{
public:
    // Pure Virtual Function
    virtual void desenhaTexto(String texto) = 0;
};
