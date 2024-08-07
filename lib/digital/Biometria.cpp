#include "Biometria.h"

Biometria::Biometria() : fingerprintSensor(&Serial2, password)
{
    
}

void Biometria::setupFingerprintSensor()
{
    // Inicializa o sensor
    fingerprintSensor.begin(57600);

    // Verifica se a senha está correta
    if (!fingerprintSensor.verifyPassword())
    {
        // Se chegou aqui significa que a senha está errada ou o sensor está problemas de conexão
        Serial.println(F("Não foi possível conectar ao sensor. Verifique a senha ou a conexão"));
        while (true)
            ;
    }
    Serial.println(F("Sensor de Biometria Inicializado!"));
}

bool Biometria::iniciaCriacaoDigital()
{
    // Considera que a digital foi tocada e imagem existe
    // Converte a imagem para o primeiro padrão
    return fingerprintSensor.image2Tz(1) == FINGERPRINT_OK;
}

bool Biometria::finalizaCriacaoDigital(int id)
{
    // Verifica se a posição é válida ou não
    if (id < 1 || id > 149)
        return false;
    // solicita o dedo
    //  Converte a imagem para o segundo padrão
    if (fingerprintSensor.image2Tz(2) != FINGERPRINT_OK)
        return false;

    // Cria um modelo da digital a partir dos dois padrões
    if (fingerprintSensor.createModel() != FINGERPRINT_OK)
        return false;

    // Guarda o modelo da digital no sensor
    return fingerprintSensor.storeModel(id) == FINGERPRINT_OK;
}

int Biometria::identificaUsuario()
{
    if (fingerprintSensor.image2Tz() != FINGERPRINT_OK)
        return -1;
    if (fingerprintSensor.fingerFastSearch() != FINGERPRINT_OK)
        return -1;
    if (fingerprintSensor.confidence < limiarConfianca)
        return -1;
    return fingerprintSensor.fingerID;
}

bool Biometria::leitorTocado()
{
    return fingerprintSensor.getImage() == FINGERPRINT_OK;
}


bool Biometria::apagarDigital(int id)
{
    // Verifica se a posição é válida ou não
    if (id < 1 || id > 149)
    {
        return false;
    }
    // retorna se conseguiu ou nao apagar a digital
    return fingerprintSensor.deleteModel(id) == FINGERPRINT_OK;
}
bool Biometria::apagarTodasDigitais()
{
    return fingerprintSensor.emptyDatabase() == FINGERPRINT_OK;
}
