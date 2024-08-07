
#ifndef biometria_h
#define biometria_h

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "Display.h"

const uint32_t password = 0x0;

class Biometria
{
private:
    Adafruit_Fingerprint fingerprintSensor;

public:
    // métodos
    Biometria();
    int limiarConfianca;
    void setupFingerprintSensor();
    bool iniciaCriacaoDigital();
    bool finalizaCriacaoDigital(int id);
    int identificaUsuario();
    bool leitorTocado();
    bool apagarDigital(int id);
    bool apagarTodasDigitais();
};

#endif
