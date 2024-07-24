
#ifndef biometria_h
#define biometria_h

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include "Display.h"



const uint32_t password = 0x0;

class Biometria
{
    private:
        // const uint32_t password;
        // static HardwareSerial HSerial;
        //Adafruit_Fingerprint fingerprintSensor = Adafruit_Fingerprint(&Serial2, password);
        Adafruit_Fingerprint fingerprintSensor;
        Display displayBio;
    public:
        //métodos

        // Biometria();
        int confiancaBiometria;
        Biometria(Display D);
        void setupFingerprintSensor();
        void criarDigital(int id);
        int identificaUsuario(int confianca);
        bool leitorTocado();
        bool verificarDigital(void);
        void apagarDigital(int id);
        void apagarTodasDigitais(void);

};

#endif
