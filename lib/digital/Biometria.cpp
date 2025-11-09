#include "Biometria.h"

Biometria::Biometria() : fingerprintSensor(&Serial2, password)
{
    
}

bool Biometria::setupFingerprintSensor()
{
    fingerprintSensor.begin(57600);

    const unsigned long START = millis();
    const unsigned long TIMEOUT_MS = 5000; // tempo máximo para tentar conectar
    const unsigned long RETRY_DELAY_MS = 200;

    while (millis() - START < TIMEOUT_MS)
    {
        if (fingerprintSensor.verifyPassword())
        {
            Serial.println(F("Sensor de Biometria Inicializado!"));
            return true;
        }
        delay(RETRY_DELAY_MS);
    }

    // Se chegou aqui, não conseguiu inicializar — não travar o sistema
    Serial.println(F("Não foi possível conectar ao sensor de Biometria. Verifique a senha ou a conexão"));
    // Opcional: setar uma flag interna para indicar indisponibilidade
    return false;
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
    if (id < 1 || id > 162)
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
