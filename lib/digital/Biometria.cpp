#include "Biometria.h"
#define pinRX_bio 16
#define pinTX_bio 17


// HardwareSerial Biometria::HSerial(2);
// //Adafruit_Fingerprint fingerprintSensor = Adafruit_Fingerprint(&Serial2, password);


// Biometria::Biometria() : fingerprintSensor(&HSerial)    
// {
//     Biometria::HSerial.begin(57600, SERIAL_8N1, pinRX_bio, pinTX_bio);
//     Biometria::setupFingerprintSensor();
// }

// void Biometria::setupFingerprintSensor()
// {
//     // Inicializa o sensor
//     fingerprintSensor.begin(57600);

//     // Verifica se a senha está correta
//     if (!fingerprintSensor.verifyPassword())
//     {
//         // Se chegou aqui significa que a senha está errada ou o sensor está problemas de conexão
//         Serial.println(F("Não foi possível conectar ao sensor. Verifique a senha ou a conexão"));
//         while (true)
//             ;
//     }
// }

// void Biometria::criarDigital(int id)
// {
    
// }
// int Biometria::verificarDigital(void)
// {
// }

// void Biometria::apagarDigital(int id)
// {
// }
// void Biometria::apagarTodasDigitais(void)
// {
// }
