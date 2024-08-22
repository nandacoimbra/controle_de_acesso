#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"
#include "Comandos.h"
#include "Teclado.h"
#include "MensagemUsuario.h"
#include "TelaSerial.h"
#include "RegistroUsuario.h"
// sd card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SDA_PIN 22
#define SCL_PIN 23

Display displayOled;
Biometria digital;
Teclado teclado;
Comandos comando(digital);
TelaSerial telaSerial(Serial);
MensagemUsuario msgUsuario(displayOled, telaSerial);
RegistroUsuario registroUsuario;

// define o estado atual do sistema, de acordo com o fluxograma
int estadoSistema = 1;

char ultimaTecla = '\0'; // Variável para armazenar a última tecla pressionada
String id;
String senha;

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  digital.setupFingerprintSensor();
  displayOled.displaySetup();
  teclado.setupKeypad();
  // teste sd card
  //  SCK MISO MOSI SS
  SPI.begin(18, 21, 19, 5);
  if (!SD.begin(5, SPI))
  {
    Serial.println("Card Mount Failed");
    return;
  }

  File file = SD.open("/registros.txt", "r");
  String stringEncontrada = registroUsuario.buscaIdNoArquivo(file, 2500);
  file.close();
  Serial.println(stringEncontrada);
  Usuario usuario = registroUsuario.transformaTextoEmUsuario(stringEncontrada);
  Serial.println("Nome: " + usuario.nome);
  Serial.printf("id: %d\n", usuario.id);
  Serial.printf("tipo: %d\n", usuario.tipo);
  Serial.println("senha: " + usuario.senha);
}

void loop()
{

  char teclaAtual = teclado.teclaPressionada();

  if (estadoSistema == 1)
  {
    // Executa toda hora
    msgUsuario.telaBemVindo();

    // Transiçoes
    if (digital.leitorTocado())
    {
      digital.identificaUsuario();
    }
    else if (Serial.available())
    {
      String comandoSerial = Serial.readString();
      comandoSerial.toUpperCase();
      // comando.executarComandos(comandoSerial);
      Serial.println("teste serial");
    }
    else if (teclaAtual != '\0')
    {
      estadoSistema = 2;
      teclaAtual = '\0';
    }
    //----------------------------
  }

  else if (estadoSistema == 2)
  {

    // Executa toda hora
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }
    msgUsuario.desenhaTelaDigiteId(teclado.digitosArmazenados);


    // Transições
    if (teclaAtual == '#')
    {
      id = teclado.digitosArmazenados;

      estadoSistema = 3;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }


  }

  if (estadoSistema == 3)
  {

    // Executa toda hora
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }
    msgUsuario.desenhaTelaDigiteSenha(teclado.digitosArmazenados);

    // Transições
    if (teclaAtual == '#')
    {
      senha = teclado.digitosArmazenados;
      estadoSistema = 4;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }
  }

  if (estadoSistema == 4)
  {
    File file = SD.open("/registros.txt", "r");
    Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), senha, TECLADO);
    file.close();
    if (user.id == -1)
    {
      msgUsuario.telaUsuarioNaoCadastrado();
    }
    else
      msgUsuario.telaUsuarioEncontrado(user.nome);
    // funcao buscaIdNoArquivo
    //  tranformaStringEmUsuario
  }
  // if(estadoSistema==2){
  //   display.digitarId();
  // }

  // // int id = digital.verificarDigital();
  // if (digital.verificarDigital()==true)
  //   Serial.println("Id = " + (String)digital.verificarDigital());

  // String getCommand()
  // {
  //     // Espera até que haja dados disponíveis no buffer serial
  //     while (!Serial.available())
  //     {
  //         // Espera
  //     }

  //     // Lê a string do buffer serial
  //     return Serial.readStringUntil('\n'); // Lê até encontrar uma nova linha
  // }

  ultimaTecla = teclaAtual;
}
