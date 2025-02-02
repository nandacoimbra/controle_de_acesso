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
  USUARIO_ENCONTRADO,
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
int estadoAtualSistema = INICIO;
int estadoAnteriorSistema = INICIO;

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

  if (estadoAtualSistema == INICIO)
  {
    estadoAnteriorSistema = estadoAtualSistema;
    // Executa toda hora
    msgUsuario.telaBemVindo();

    // Transiçoes
    if (digital.leitorTocado())
    {
      if (digital.identificaUsuario() != -1)
      {
        estadoAtualSistema = USUARIO_ENCONTRADO;
      }
      else
      {
        estadoAtualSistema = USUARIO_NAO_CADASTRADO;
      }
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
      estadoAtualSistema = INSERCAO_ID_USUARIO;
      teclaAtual = '\0';
    }
    //----------------------------
  }

  else if (estadoAtualSistema == INSERCAO_ID_USUARIO)
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

      estadoAtualSistema = INSERCAO_SENHA_USUARIO;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }
  }

  else if (estadoAtualSistema == INSERCAO_SENHA_USUARIO)
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
      estadoAtualSistema = AUTENTICACAO;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }
  }

  else if (estadoAtualSistema == AUTENTICACAO)
  {
    File file = SD.open("/registros.txt", "r");
    Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), senha, TECLADO);
    file.close();
    if (user.id == -1)
    {
      estadoAtualSistema = USUARIO_NAO_CADASTRADO;
      timer = millis();
    }
    else if (user.tipo == MASTER)
    {
      estadoAtualSistema = MENU_USUARIO_MASTER;

      // funcao buscaIdNoArquivo
      //  tranformaStringEmUsuario
    }
    else
    {
      msgUsuario.telaUsuarioEncontrado(user.nome);
    }
  }

  else if (estadoAtualSistema == MENU_USUARIO_MASTER)
  {
    msgUsuario.telaMenuMaster();

    if (teclaAtual == '1')
    {
    }
    else if (teclaAtual == '2')
    {
      teclaAtual = '\0';
      estadoAtualSistema = CADASTRANDO_USUARIO;
    }
    else if (teclaAtual == '3')
    {
    }
  }

  else if (estadoAtualSistema == CADASTRANDO_USUARIO)
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
      estadoAtualSistema = CADASTRO_CONFIRMANDO_SENHA_USUARIO;
    }
  }

  else if (estadoAtualSistema == CADASTRO_CONFIRMANDO_SENHA_USUARIO)
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
        estadoAtualSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO;
      }
      else
      {
        // senha incorreta
      }
    }
  }

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO)
  {
    msgUsuario.telaCadastroBiometriaEncosteDedo();
    if (digital.leitorTocado())
    {
      bool aux = digital.iniciaCriacaoDigital();
      Serial.println(aux);
      if (aux)
      {
        estadoAtualSistema = CADASTRO_BIOMETRIA_RETIRE_DEDO;
        timer = millis();
      }
    }
  }

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_RETIRE_DEDO)
  {
    msgUsuario.telaCadastroBiometriaRetireDedo();
    if (!digital.leitorTocado() && (millis() - timer > 5000))
    {
      estadoAtualSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE;
    }
  }

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE)
  {
    msgUsuario.telaCadastroBiometriaEncosteDedoNovamente();
    if (digital.leitorTocado())
    {
      if (digital.finalizaCriacaoDigital(111))
      {
        estadoAtualSistema = USUARIO_CADASTRADO_COM_SUCESSO;
      }
    }
  }

  else if (estadoAtualSistema == USUARIO_CADASTRADO_COM_SUCESSO)
  {
    msgUsuario.telaBiometriaCadastradaSucesso();
    if (teclado.teclaPressionada() == '#')
      estadoAtualSistema = MENU_USUARIO_MASTER;
  }

  else if (estadoAtualSistema == USUARIO_NAO_CADASTRADO)
  {
    // Executa só na entrada
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      timer = millis();
      msgUsuario.telaUsuarioNaoCadastrado();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    if (millis() - timer > 3000)
    {
      estadoAtualSistema = INICIO;
    }
  }

  else if (estadoAtualSistema == USUARIO_ENCONTRADO)
  {
    // Executa só na entrada
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      timer = millis();
      msgUsuario.telaMsgUsuarioEncontrado();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    if (millis() - timer > 3000)
    {
      estadoAtualSistema = INICIO;
    }
  }

  ultimaTecla = teclaAtual;
  // estadoAnteriorSistema = estadoAtualSistema;
}
