#include "Biometria.h"
#include "Display.h"

#define pinRX_bio 16
#define pinTX_bio 17

Display displayBio;
// const uint32_t password = 0x0;

// HardwareSerial Biometria::HSerial(2);
// //Adafruit_Fingerprint fingerprintSensor = Adafruit_Fingerprint(&Serial2, password);

// Biometria::Biometria() : fingerprintSensor(&HSerial, password)
// {
//     Biometria::HSerial.begin(57600, SERIAL_8N1, pinRX_bio, pinTX_bio);
//     Biometria::setupFingerprintSensor();
// }
Biometria::Biometria()
{
}

String getCommand()
{
    // Espera até que haja dados disponíveis no buffer serial
    while (!Serial.available())
    {
        // Espera
    }

    // Lê a string do buffer serial
    return Serial.readStringUntil('\n'); // Lê até encontrar uma nova linha
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

void Biometria::criarDigital(int location)
{
    // Verifica se a posição é válida ou não
    if (location < 1 || location > 149)
    {
        // Se chegou aqui a posição digitada é inválida, então abortamos os próximos passos
        Serial.println(F("Posição inválida"));
        return;
    }

    
    Serial.println(F("Encoste o dedo no sensor"));
    displayBio.displayPrint("Encoste o dedo no sensor");

    // Espera até pegar uma imagem válida da digital
    while (fingerprintSensor.getImage() != FINGERPRINT_OK)
        ;

    // Converte a imagem para o primeiro padrão
    if (fingerprintSensor.image2Tz(1) != FINGERPRINT_OK)
    {
        // Se chegou aqui deu erro, então abortamos os próximos passos
        Serial.println(F("Erro image2Tz 1"));
        
        
        return;
    }

    Serial.println(F("Tire o dedo do sensor"));
    displayBio.displayPrint("Tire o dedo do sensor");

    delay(2000);

    // Espera até tirar o dedo
    while (fingerprintSensor.getImage() != FINGERPRINT_NOFINGER)
        ;

    // Antes de guardar precisamos de outra imagem da mesma digital
    Serial.println(F("Encoste o mesmo dedo no sensor"));
     displayBio.displayPrint("Encoste o mesmo dedo no sensor");

    // Espera até pegar uma imagem válida da digital
    while (fingerprintSensor.getImage() != FINGERPRINT_OK)
        ;

    // Converte a imagem para o segundo padrão
    if (fingerprintSensor.image2Tz(2) != FINGERPRINT_OK)
    {
        // Se chegou aqui deu erro, então abortamos os próximos passos
        Serial.println(F("Erro image2Tz 2"));
        return;
    }

    // Cria um modelo da digital a partir dos dois padrões
    if (fingerprintSensor.createModel() != FINGERPRINT_OK)
    {
        // Se chegou aqui deu erro, então abortamos os próximos passos
        Serial.println(F("Erro createModel"));
        return;
    }

    // Guarda o modelo da digital no sensor
    if (fingerprintSensor.storeModel(location) != FINGERPRINT_OK)
    {
        // Se chegou aqui deu erro, então abortamos os próximos passos
        Serial.println(F("Erro storeModel"));
        return;
    }

    // Se chegou aqui significa que todos os passos foram bem sucedidos
    Serial.println(F("Sucesso!!!"));
    displayBio.displayPrint("Digital salva com sucesso!");
}

void Biometria::verificarDigital(void)
{
    Serial.println(F("Encoste o dedo no sensor"));

    // Espera até pegar uma imagem válida da digital
    while (fingerprintSensor.getImage() != FINGERPRINT_OK)
        ;

    // Converte a imagem para o padrão que será utilizado para verificar com o banco de digitais
    if (fingerprintSensor.image2Tz() != FINGERPRINT_OK)
    {
        // Se chegou aqui deu erro, então abortamos os próximos passos
        Serial.println(F("Erro image2Tz"));
        return;
    }

    // Procura por este padrão no banco de digitais
    if (fingerprintSensor.fingerFastSearch() != FINGERPRINT_OK)
    {
        // Se chegou aqui significa que a digital não foi encontrada
        Serial.println(F("Digital não encontrada"));
        return;
    }

    // Se chegou aqui a digital foi encontrada
    // Mostramos a posição onde a digital estava salva e a confiança
    // Quanto mais alta a confiança melhor
    Serial.print(F("Digital encontrada com confiança de "));
    Serial.print(fingerprintSensor.confidence);
    Serial.print(F(" na posição "));
    Serial.println(fingerprintSensor.fingerID);
}

void Biometria::apagarDigital(int location)
{

    // Verifica se a posição é válida ou não
    if (location < 1 || location > 149)
    {
        // Se chegou aqui a posição digitada é inválida, então abortamos os próximos passos
        Serial.println(F("Posição inválida"));
        return;
    }

    // Apaga a digital nesta posição
    if (fingerprintSensor.deleteModel(location) != FINGERPRINT_OK)
    {
        Serial.println(F("Erro ao apagar digital"));
    }
    else
    {
        Serial.println(F("Digital apagada com sucesso!!!"));
    }
}
void Biometria::apagarTodasDigitais(void)
{
    Serial.println(F("Tem certeza? (s/N)"));

    // Lê o que foi digitado no monitor serial
    String command = getCommand();

    // Coloca tudo em maiúsculo para facilitar a comparação
    command.toUpperCase();

    // Verifica se foi digitado "S" ou "SIM"
    if (command == "S" || command == "SIM")
    {
        Serial.println(F("Apagando banco de digitais..."));

        // Apaga todas as digitais
        if (fingerprintSensor.emptyDatabase() != FINGERPRINT_OK)
        {
            Serial.println(F("Erro ao apagar banco de digitais"));
        }
        else
        {
            Serial.println(F("Banco de digitais apagado com sucesso!!!"));
        }
    }
    else
    {
        Serial.println(F("Cancelado"));
    }
}
