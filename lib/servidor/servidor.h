#ifndef servidor_h
#define servidor_h

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Classe que encapsula a lógica de conexão Wi-Fi e controle da fechadura
class Servidor {
private:
    const char* ssid;         // Nome da rede Wi-Fi
    const char* senhaWifi;     // Senha da rede Wi-Fi
    int pinTranca;            // Pino conectado ao relé fechadura

    void conectarWiFi();      // Método privado para conectar ao Wi-Fi
    void configurarRotas();   // Método privado para configurar as rotas HTTP

public:
    WebServer server;         // Servidor Web rodando na porta 80

    //Construtor que recebe os dados da rede e pino da fechadura
    Servidor(const char* ssid, const char* senhaWifi, int pinTranca);

    void iniciar();           // Inicializa o servidor e a conexão Wi-Fi
    void loop();              // Mantém o servidor respondendo aos clientes
    void destrancaFechadura(); // Handler para destrancar a fechadura
};

#endif
