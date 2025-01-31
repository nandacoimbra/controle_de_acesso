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

enum Estado
{
  INICIO,
  INSERCAO_ID_USUARIO,
  INSERCAO_SENHA_USUARIO,
  AUTENTICACAO,
  PORTA_ABERTA,
  USUARIO_NAO_CADASTRADO,
  MENU_USUARIO_MASTER,
  REMOVENDO_USUARIO,
  CADASTRANDO_USUARIO,
  CADASTRO_CONFIRMANDO_SENHA_USUARIO,
  CADASTRO_SENHA_INCORRETA,
  CADASTRO_BIOMETRIA_ENCOSTE_DEDO,
  CADASTRO_BIOMETRIA_RETIRE_DEDO,
  SALVA_USUARIO_SD_CARD,
  CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE,
  USUARIO_CADASTRADO_COM_SUCESSO,

};

Display displayOled;
Biometria digital;
Teclado teclado;
Comandos comando(digital);
TelaSerial telaSerial(Serial);
MensagemUsuario msgUsuario(displayOled, telaSerial);
RegistroUsuario registroUsuario;

// define o estado atual do sistema, de acordo com o fluxograma
int estadoSistema = INICIO;

char ultimaTecla = '\0'; // Variável para armazenar a última tecla pressionada
String id;
String senha;
String confirmaSenha;

long timer = 0;

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

  if (estadoSistema == INICIO)
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
      estadoSistema = INSERCAO_ID_USUARIO;
      teclaAtual = '\0';
    }
    //----------------------------
  }

  else if (estadoSistema == INSERCAO_ID_USUARIO)
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

      estadoSistema = INSERCAO_SENHA_USUARIO;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }
  }

  if (estadoSistema == INSERCAO_SENHA_USUARIO)
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
      estadoSistema = AUTENTICACAO;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }
  }

  if (estadoSistema == AUTENTICACAO)
  {
    File file = SD.open("/registros.txt", "r");
    Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), senha, TECLADO);
    file.close();
    if (user.id == -1)
    {
      msgUsuario.telaUsuarioNaoCadastrado();
      // executar alguma ação, tipo voltar para o início ou digitar a senha novamente
    }
    else if (user.tipo == MASTER)
    {
      estadoSistema = MENU_USUARIO_MASTER;

      // funcao buscaIdNoArquivo
      //  tranformaStringEmUsuario
    }
    else
    {
      msgUsuario.telaUsuarioEncontrado(user.nome);
    }
  }

  if (estadoSistema == MENU_USUARIO_MASTER)
  {
    msgUsuario.telaMenuMaster();

    if (teclaAtual == '1')
    {
    }
    else if (teclaAtual == '2')
    {
      teclaAtual = '\0';
      estadoSistema = CADASTRANDO_USUARIO;
    }
    else if (teclaAtual == '3')
    {
    }
  }

  if (estadoSistema == CADASTRANDO_USUARIO)
  {
    // Executa toda hora
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }

    msgUsuario.telaCadastroDigiteSenha(teclado.digitosArmazenados);

    // Transições
    if (teclaAtual == '#')
    {
      senha = teclado.digitosArmazenados;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
      estadoSistema = CADASTRO_CONFIRMANDO_SENHA_USUARIO;
    }
  }
  if (estadoSistema == CADASTRO_CONFIRMANDO_SENHA_USUARIO)
  {
    // Executa toda hora
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }

    msgUsuario.telaCadastroDigiteNovamenteSenha(teclado.digitosArmazenados);

    // Transições
    if (teclaAtual == '#')
    {
      confirmaSenha = teclado.digitosArmazenados;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
      if (senha == confirmaSenha)
      {
        estadoSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO;
      }
      else
      {
        // senha incorreta
      }
    }
  }

  if (estadoSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO)
  {
    msgUsuario.telaCadastroBiometriaEncosteDedo();
    if (digital.leitorTocado())
    {
      bool aux = digital.iniciaCriacaoDigital();
      Serial.println(aux);
      if (aux)
      {
        estadoSistema = CADASTRO_BIOMETRIA_RETIRE_DEDO;
        timer = millis();
      }
    }
  }

  if (estadoSistema == CADASTRO_BIOMETRIA_RETIRE_DEDO)
  {
    msgUsuario.telaCadastroBiometriaRetireDedo();
    if (!digital.leitorTocado() && (millis() - timer > 5000))
    {
      estadoSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE;
    }
  }

  if (estadoSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE)
  {
    msgUsuario.telaCadastroBiometriaEncosteDedoNovamente();
    if (digital.leitorTocado())
    {
      if (digital.finalizaCriacaoDigital(102))
      {
        estadoSistema = USUARIO_CADASTRADO_COM_SUCESSO;
      }
    }
  }
  if (estadoSistema == USUARIO_CADASTRADO_COM_SUCESSO)
  {
    msgUsuario.telaBiometriaCadastradaSucesso();
    if (teclado.teclaPressionada() == '#')
      estadoSistema = MENU_USUARIO_MASTER;
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
