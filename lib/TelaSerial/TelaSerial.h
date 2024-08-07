#pragma once

#include <Arduino.h>
#include <TelaSaida.h>

class TelaSerial : public TelaSaida
{
private:
    //exemplo Serial, Serial2, SoftwareSerial, etc
    HardwareSerial& serialUsado;

public:
    // métodos
    TelaSerial(HardwareSerial serial);
    void desenhaTexto(String texto);
};
