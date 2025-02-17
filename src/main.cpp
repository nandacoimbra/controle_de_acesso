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
  INVALIDO,
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
  BIOMETRIA_CADASTRADA_COM_SUCESSO,

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
int estadoAnteriorSistema = INVALIDO;

char ultimaTecla = '\0'; // Variável para armazenar a última tecla pressionada
String id;               // id usado na autenticacao
int idGerado;
int idBiometria; // id gerado para um novo usuario
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
    if (estadoAnteriorSistema != estadoAtualSistema)
    {
      //
      msgUsuario.telaBemVindo();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    // Executa toda hora

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
    estadoAnteriorSistema = estadoAtualSistema;
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
    estadoAnteriorSistema = estadoAtualSistema;
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
    estadoAnteriorSistema = estadoAtualSistema;
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
    estadoAnteriorSistema = estadoAtualSistema;
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
    else if (teclaAtual == '4')
    {
      teclaAtual = '\0';
      estadoAtualSistema = INICIO;
    }
  }

  else if (estadoAtualSistema == CADASTRANDO_USUARIO)
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      File file = SD.open("/registros.txt", "r");
      idGerado = registroUsuario.buscaProximoIdDisponivel(file);
      file.close();
      // atualiza o estado
      estadoAnteriorSistema = estadoAtualSistema;
    }

    // Executa toda hora
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }

    msgUsuario.telaCadastroDigiteSenha(teclado.digitosArmazenados, idGerado);

    // Transições
    if (teclaAtual == '#')
    {
      // armazena senha digitada
      senha = teclado.digitosArmazenados;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
      // muda para o estado de confirmacao de senha
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
      // armazena senha digitada pela 2 vez
      confirmaSenha = teclado.digitosArmazenados;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
      // compara as senhas digitadas
      if (senha == confirmaSenha)
      {
        // se as senhas estiveram iguais, segue p/ o cadastro da biometria
        estadoAtualSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO;
      }
      else
      {
        // se as senhas estiveram diferentes, retorna ao inicio do cadastro
        estadoAtualSistema = CADASTRO_SENHA_INCORRETA;
      }
    }
  }
  else if (estadoAtualSistema == CADASTRO_SENHA_INCORRETA)
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      timer = millis();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    // exibe mensagem de senha incorreta e retorno ao cadastro
    msgUsuario.telaSenhaIncorreta();
    // aguarda 3s para retornar
    if (millis() - timer > 3000)
    {
      // retorna ao inicio do cadastro
      estadoAtualSistema = CADASTRANDO_USUARIO;
    }
  }
  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO)
  {
    estadoAnteriorSistema = estadoAtualSistema;
    msgUsuario.telaCadastroBiometriaEncosteDedo();
    if (digital.leitorTocado())
    {
      bool primeiraImagemOk = digital.iniciaCriacaoDigital();
      if (primeiraImagemOk)
      {
        estadoAtualSistema = CADASTRO_BIOMETRIA_RETIRE_DEDO;
      }
    }
  }

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_RETIRE_DEDO)
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      timer = millis();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    msgUsuario.telaCadastroBiometriaRetireDedo();
    if (!digital.leitorTocado() && (millis() - timer > 5000))
    {
      estadoAtualSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE;
    }
  }

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE)
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaCadastroBiometriaEncosteDedoNovamente();
    }
    if (digital.leitorTocado())
    {
      File file = SD.open("/registros.txt", "r");
      idBiometria = registroUsuario.buscaIdBiometriaDisponivel(file);
      file.close();
      if (digital.finalizaCriacaoDigital(idBiometria))
      {
        Serial.println("Biometria cadastrada com sucesso na posicao: " + idBiometria);
        estadoAtualSistema = BIOMETRIA_CADASTRADA_COM_SUCESSO;
      }
    }
  }

  else if (estadoAtualSistema == BIOMETRIA_CADASTRADA_COM_SUCESSO)
  {
    
    estadoAnteriorSistema = estadoAtualSistema;
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
