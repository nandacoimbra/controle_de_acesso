#include "servidor.h"

// Construtor: inicializa os atributos e o servidor na porta 80
Servidor::Servidor(const char *ssid, const char *senhaWifi, int pinTranca)
    : ssid(ssid), senhaWifi(senhaWifi), pinTranca(pinTranca), server(80) {}

// Método responsável por conectar à rede Wi-Fi
void Servidor::conectarWiFi()
{
    Serial.print("Conectando-se a ");
    Serial.println(ssid);

    WiFi.begin(ssid, senhaWifi); // Inicia conexão com Wi-Fi

    // Aguarda conexão
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // Exibe o IP local após conexão
    Serial.println();
    Serial.print("Conectado no IP: ");
    Serial.println(WiFi.localIP());
}

// Método para configurar rotas HTTP do servidor
void Servidor::configurarRotas()
{
    // Associa a rota "/destrancar" à função destrancaFechadura()
    server.on("/destrancar", [this]()
              { this->destrancaFechadura(); });
}

// Inicializa pino da fechadura, conecta Wi-Fi, configura rotas e inicia o servidor
void Servidor::iniciar()
{
    pinMode(pinTranca, OUTPUT);   // Define o pino da fechadura como saída
    // digitalWrite(pinTranca, LOW); // Garante que a fechadura comece trancada

    conectarWiFi();    // Conecta ao Wi-Fi
    configurarRotas(); // Define as rotas do servidor

    server.begin(); // Inicia o servidor
    Serial.println("Servidor iniciado");
}

// Loop que mantém o servidor em funcionamento
void Servidor::loop()
{
    server.handleClient(); // Verifica se há clientes HTTP e responde
}

// Função chamada quando a rota /destrancar é acessada
void Servidor::destrancaFechadura()
{
    //   digitalWrite(pinTranca, HIGH);   // Aciona o relé/motor
    //   delay(3000);                     // Mantém aberto por 3 segundos
    //   digitalWrite(pinTranca, LOW);    // Tranca novamente
    Serial.println("Fechadura destrancada");                 // Log no console
    server.send(200, "text/plain", "Fechadura destrancada"); // Responde ao cliente
}
