
#ifndef biometria_h
#define biometria_h

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

class Biometria
{
    private:
        // const uint32_t password;
        void setupFingerprintSensor();
        static HardwareSerial HSerial;
        Adafruit_Fingerprint fingerprintSensor;
    public:
        //métodos

        Biometria();
        void criarDigital(int id);
        int verificarDigital(void);
        void apagarDigital(int id);
        void apagarTodasDigitais(void);

};

#endif
