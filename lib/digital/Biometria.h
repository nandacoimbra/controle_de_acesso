
#ifndef biometria_h
#define biometria_h

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

const uint32_t password = 0x0;

class Biometria
{
    private:
        // const uint32_t password;
        // static HardwareSerial HSerial;
        Adafruit_Fingerprint fingerprintSensor = Adafruit_Fingerprint(&Serial2, password);
    public:
        //métodos

        Biometria();
        void setupFingerprintSensor();
        void criarDigital(int id);
        void verificarDigital(void);
        void apagarDigital(int id);
        void apagarTodasDigitais(void);

};

#endif
