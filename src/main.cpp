#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"
#include "Comandos.h"
#include "Teclado.h"
#include "MensagemUsuario.h"
#include "TelaSerial.h"
#include "RegistroUsuario.h"
#include "ComunicacaoSerial.h"
// sd card
#include "FS.h"
#include "SPIFFS.h"
#include "SD.h"
#include "SPI.h"
// servidor
#include "Servidor.h"
#include "servidor.h"

#include <Backup.h>
#include <WiFi.h>

#define SDA_PIN 22
#define SCL_PIN 23

enum Estado
{
  INVALIDO,
  INICIO,
  RECONHECIMENTO_FACIAL_AGUARDANDO,
  RECONHECIMENTO_FACIAL_PROCESSANDO,
  INSERCAO_ID_USUARIO,
  ERRO_INSERCAO_ID_USUARIO,
  INSERCAO_SENHA_USUARIO,
  ERRO_INSERCAO_SENHA_USUARIO,
  AUTENTICACAO,
  ABRINDO_PORTA,
  PORTA_ABERTA,
  USUARIO_NAO_CADASTRADO,
  USUARIO_ENCONTRADO,
  MENU_USUARIO_MASTER,
  CADASTRO_DIGITANDO_SENHA,
  CADASTRO_DIGITANDO_NOME,
  CADASTRO_CONFIRMANDO_SENHA_USUARIO,
  CADASTRO_SENHA_INCORRETA,
  CADASTRO_BIOMETRIA_ENCOSTE_DEDO,
  CADASTRO_BIOMETRIA_JA_CADASTRADA_ERRO,
  CADASTRO_BIOMETRIA_RETIRE_DEDO,
  CADASTRO_INFORMA_TIPO_USUARIO,
  SALVA_USUARIO_SD_CARD,
  CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE,
  BIOMETRIA_CADASTRADA_COM_SUCESSO,
  REMOVE_USUARIO_INFORME_ID,
  REMOVE_USUARIO_ID_NAO_ENCONTRADO,
  REMOVE_USUARIO_CONFIRMA_ID,
  REMOVENDO_USUARIO,
  USUARIO_REMOVIDO_COM_SUCESSO,
  ERRO_REMOVER_USUARIO,
  FACE_DETECTADA,
  CADASTRO_PREPARAR_PARA_FOTO,
  CADASTRO_TIRANDO_FOTO,
  REGISTRA_LOG_USUARIO

};

Display displayOled;
Biometria digital;
Teclado teclado;
Comandos comando(digital);
TelaSerial telaSerial(Serial);
MensagemUsuario msgUsuario(displayOled, telaSerial);
RegistroUsuario registroUsuario;
ComunicacaoSerial comunicacaoSerial;

// define o estado atual do sistema, de acordo com o fluxograma
int estadoAtualSistema = INICIO;
int estadoAnteriorSistema = INVALIDO;
int contaArquivosRemovidos = 0;

char ultimaTecla = '\0';        // Variável para armazenar a última tecla pressionada
char ultimaTeclaNaoNula = '\0'; // Variável para armazenar a última tecla pressionada que não é nula
char letraAtual = '\0';         // Variável para armazenar a letra atual para digitacao do nome
String nomeUsuario;             // nome do usuario
int contadorLetras = 0;         // contador de letras digitadas
String id;                      // id usado na autenticacao
int idGerado;                   // id gerado para um novo usuario
int idBiometria;                // idBiometria gerado para um novo usuario
String senha;
String confirmaSenha;
TipoUsuario tipoUsuario;
String stringEncontrada;
Backup backup;
bool usuarioRemovido = false;
bool emCadastro = false;
bool logRegistrado = false;

long timer = 0;

const char *ssid = "House";         // SSID da sua rede Wi-Fi
const char *senhaWifi = "12345678"; // Senha da rede Wi-Fi
// const char *ssid = "Fernanda ";
// const char *senha_wifi = "02072024api";
const int pinoTranca = 25; // Pino ligado à fechadura

// Criação do objeto servidor com os dados necessários
// Servidor servidor(ssid, senhaWifi, pinoTranca);

void resetaValoresGlobais()
{
  ultimaTecla = '\0';        // Variável para armazenar a última tecla pressionada
  ultimaTeclaNaoNula = '\0'; // Variável para armazenar a última tecla pressionada que não é nula
  letraAtual = '\0';         // Variável para armazenar a letra atual para digitacao do nome
  nomeUsuario = "";          // nome do usuario
  contadorLetras = 0;        // contador de letras digitadas
  id = "";                   // id usado na autenticacao
  idGerado = 0;              // id gerado para um novo usuario
  idBiometria = 0;           // idBiometria gerado para um novo usuario
  senha = "";
  confirmaSenha = "";
  tipoUsuario = COMUM;
  stringEncontrada = "";
  usuarioRemovido = false;
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, senhaWifi);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

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

  if (!SPIFFS.begin(true))
  {
    Serial.println("Erro ao montar o SPIFFS!");
    return;
  }
  Serial.println("SPIFFS montado com sucesso!");

  // servidor.iniciar(); // Inicia o servidor
  //                     // Aguarda conexão

  configTime(-3 * 3600, 0, "pool.ntp.org"); // GMT-3 para Brasil
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S");
  }
  // File file = SD.open("/registros.txt", "r");
  // String stringEncontrada = registroUsuario.buscaIdNoArquivo(file, 2500);
  // file.close();
  // Serial.println(stringEncontrada);
  // Usuario usuario = registroUsuario.transformaTextoEmUsuario(stringEncontrada);
  // Serial.println("Nome: " + usuario.nome);
  // Serial.printf("id: %d\n", usuario.id);
  // Serial.printf("tipo: %d\n", usuario.tipo);
  // Serial.println("senha: " + usuario.senha);
  pinMode(pinoTranca, OUTPUT);

  // Faz o backup
  backup.backupUsuarios();

  // Mostra o que foi salvo no SD card
  backup.imprimirBackup();

  File logFile = SPIFFS.open("/logs.txt", "r");
  while (logFile.available())
  {
    Serial.write(logFile.read());
  }
  logFile.close();
}

void loop()
{
  // servidor.loop(); // Mantém o servidor respondendo aos clientes

  char teclaAtual = teclado.teclaPressionada();

  if (estadoAtualSistema == INICIO)
  {
    if (estadoAnteriorSistema != estadoAtualSistema)
    {
      resetaValoresGlobais();
      msgUsuario.telaBemVindo();
      estadoAnteriorSistema = estadoAtualSistema;
    }

    else if (teclaAtual == 'C') // aperte a tecla C para reconhecimento facial
    {
      estadoAnteriorSistema = estadoAtualSistema;
      estadoAtualSistema = RECONHECIMENTO_FACIAL_AGUARDANDO;
      teclaAtual = '\0';
    }

    else if (teclaAtual == '#') // aperte a tecla # para login com teclado
    {
      estadoAnteriorSistema = estadoAtualSistema;
      estadoAtualSistema = INSERCAO_ID_USUARIO;
      teclaAtual = '\0';
    }

    // Transiçoes
    if (digital.leitorTocado())
    {
      int id = digital.identificaUsuario();
      if (id != -1)
      {
        Serial.println("ID lido: " + String(id));
        File file = SPIFFS.open("/usuarios.txt", "r");
        Usuario userEncontrado;
        bool encontrado = false;
        while (file.available())
        {
          String linha = file.readStringUntil('\n');
          Usuario user = registroUsuario.transformaTextoEmUsuario(linha);
          if (user.idBiometria == id)
          {
            userEncontrado = user;
            encontrado = true;
            break;
          }
        }
        file.close();

        if (encontrado)
        {
          Serial.println("Usuário encontrado: " + userEncontrado.nome);
          msgUsuario.telaUsuarioEncontrado(userEncontrado.nome);
          // Registrar log de entrada
          File logFile = SPIFFS.open("/logs.txt", "a");
          if (logFile)
          {
            registroUsuario.registrarLogEntrada(logFile, userEncontrado.id, userEncontrado.nome, "BIOMETRIA");
            logFile.close();
          }
          estadoAtualSistema = USUARIO_ENCONTRADO;
        }
        else
        {
          estadoAtualSistema = USUARIO_NAO_CADASTRADO;
        }
      }
      }
      else if (Serial.available())
      {
        String comandoSerial = Serial.readStringUntil('\n');
        comandoSerial.trim();

        if (comandoSerial == "cadastro_iniciado")
        {
          emCadastro = true;
          estadoAtualSistema = CADASTRO_TIRANDO_FOTO;
        }
        else if (comandoSerial == "cadastro_finalizado")
        {
          emCadastro = false;
          estadoAtualSistema = CADASTRO_INFORMA_TIPO_USUARIO;
        }
      }
    }

    else if (estadoAtualSistema == RECONHECIMENTO_FACIAL_AGUARDANDO)
    {
      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        msgUsuario.telaAguardandoReconhecimentoFacial();
        // Comando para app em Python p/ iniciar reconhecimento facial
        comunicacaoSerial.iniciarReconhecimentoFacial();
        timer = millis();
        estadoAnteriorSistema = estadoAtualSistema;
      }

      // Aguarda resposta da serial
      if (Serial.available())
      {
        String resposta = Serial.readStringUntil('\n');
        resposta.trim();
        if (resposta.startsWith("face_detectada:")) // Exemplo: "face_detectada:1234"
        {
          id = resposta.substring(resposta.indexOf(':') + 1);
          // id.trim();
          // Serial.println("Usuario detectado: " + id);
          Serial.print("ID recebido: ");
          Serial.println(id);
          Serial.print("ID convertido: ");
          Serial.println(id.toInt());
          estadoAtualSistema = RECONHECIMENTO_FACIAL_PROCESSANDO;
        }
        else if (resposta == "face_nao_reconhecida")
        {
          timer = millis();
          msgUsuario.telaFaceNaoReconhecida();
          Serial.println("Usuario não detectado");
          // delay(2000);
          if (millis() - timer > 3000)
          {
            estadoAtualSistema = INICIO;
          }
        }
      }
    }

    else if (estadoAtualSistema == RECONHECIMENTO_FACIAL_PROCESSANDO)
    {
      // Aqui você pode buscar o usuário pelo ID recebido e abrir a porta, etc.

      // File file = SD.open("/registros.txt", "r");
      File file = SPIFFS.open("/usuarios.txt", "r");
      Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), "", RECONHECIMENTO_FACIAL);
      file.close();
      Serial.print("Usuário encontrado: ");
      Serial.println(user.nome);
      Serial.print("ID encontrado: ");
      Serial.println(user.id);
      if (user.id != -1)
      {
        msgUsuario.telaUsuarioEncontrado(user.nome);
        // Aqui você pode acionar a abertura da porta
        // pinMode(pinoTranca, HIGH); // Abre a porta

        File logFile = SPIFFS.open("/logs.txt", "a");
        if (logFile)
        {
          registroUsuario.registrarLogEntrada(logFile, user.id, user.nome, "RECONHECIMENTO_FACIAL");
          logFile.close();
        }
        delay(3000);

        estadoAtualSistema = INICIO;
        // pinMode(pinoTranca, LOW); // FECHA a porta
      }
      else
      {
        msgUsuario.telaUsuarioNaoCadastrado();
        delay(3000);
        estadoAtualSistema = INICIO;
      }
    }

    else if (estadoAtualSistema == INSERCAO_ID_USUARIO)
    {
      estadoAnteriorSistema = estadoAtualSistema;

      if (teclaAtual != '\0' && teclaAtual != '#')
      {
        teclado.armazenaDigito(teclaAtual);
      }
      msgUsuario.desenhaTelaDigiteId(teclado.digitosArmazenados);

      if (teclaAtual == '#')
      {
        id = teclado.digitosArmazenados;

        if (id.length() > 0)
        {
          estadoAtualSistema = INSERCAO_SENHA_USUARIO;
          teclaAtual = '\0';
          teclado.limpaDigitosArmazenados();
        }
        else
        {
          estadoAtualSistema = ERRO_INSERCAO_ID_USUARIO;
          teclaAtual = '\0';
          teclado.limpaDigitosArmazenados();
        }
      }
      // else if (teclaAtual == '*')
      // {
      //   estadoAtualSistema = INICIO;
      //   teclado.limpaDigitosArmazenados();
      //   teclaAtual = '\0';
      // }
    }

    else if (estadoAtualSistema == ERRO_INSERCAO_ID_USUARIO)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaIdNaoInformado(); // Mensagem avisando para pressionar * ou tentar novamente
      // timer = millis();
      if (teclaAtual == '*')
      {
        estadoAtualSistema = INICIO;
        teclado.limpaDigitosArmazenados();
        teclaAtual = '\0';
      }
      else if (teclaAtual == '#')
      {
        estadoAtualSistema = INSERCAO_ID_USUARIO;
        teclado.limpaDigitosArmazenados();
        teclaAtual = '\0';
      }
    }
    else if (estadoAtualSistema == INSERCAO_SENHA_USUARIO)
    {
      estadoAnteriorSistema = estadoAtualSistema;

      if (teclaAtual != '\0' && teclaAtual != '#')
      {
        teclado.armazenaDigito(teclaAtual);
      }
      msgUsuario.desenhaTelaDigiteSenha(teclado.digitosArmazenados);

      if (teclaAtual == '#')
      {
        senha = teclado.digitosArmazenados;

        if (senha.length() > 0)
        {
          estadoAtualSistema = AUTENTICACAO;
          teclaAtual = '\0';
          teclado.limpaDigitosArmazenados();
        }
        else
        {
          estadoAtualSistema = ERRO_INSERCAO_SENHA_USUARIO;
          teclaAtual = '\0';
          teclado.limpaDigitosArmazenados();
        }
      }
      // else if (teclaAtual == '*')
      // {
      //   estadoAtualSistema = INICIO;
      //   teclado.limpaDigitosArmazenados();
      //   teclaAtual = '\0';
      // }
    }

    else if (estadoAtualSistema == ERRO_INSERCAO_SENHA_USUARIO)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaSenhaNaoInformada(); // Mensagem avisando para pressionar * ou tentar novamente
      // timer = millis();
      if (teclaAtual == '*')
      {
        estadoAtualSistema = INICIO;
        teclado.limpaDigitosArmazenados();
        teclaAtual = '\0';
      }
      else if (teclaAtual == '#')
      {
        estadoAtualSistema = INSERCAO_SENHA_USUARIO;
        teclado.limpaDigitosArmazenados();
        teclaAtual = '\0';
      }
    }
    // mudar armazenamento
    else if (estadoAtualSistema == AUTENTICACAO)
    {
      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        timer = millis();
        estadoAnteriorSistema = estadoAtualSistema;
      }
      // File file = SD.open("/registros.txt", "r");
      File file = SPIFFS.open("/usuarios.txt", "r");
      Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), senha, TECLADO);
      nomeUsuario= user.nome;
      file.close();
      if (user.id == -1)
      {
        estadoAtualSistema = USUARIO_NAO_CADASTRADO;
      }
      else
      {
        // REGISTRA O LOG DE ENTRADA
        File logFile = SPIFFS.open("/logs.txt", "a");
        if (logFile && !logRegistrado)
        {
          registroUsuario.registrarLogEntrada(logFile, user.id, user.nome, "teclado");
          logFile.close();
          logRegistrado = true;
        }
        if (user.tipo == MASTER)
        {
          estadoAtualSistema = MENU_USUARIO_MASTER;
        }
        else
        {
          msgUsuario.telaUsuarioEncontrado(user.nome);
          if (millis() - timer > 3000)
          {
            // colocar estado para abrir a porta
            estadoAtualSistema = INICIO;
          }
        }
      }
    }

    else if (estadoAtualSistema == MENU_USUARIO_MASTER)
    {
      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        estadoAnteriorSistema = estadoAtualSistema;
        msgUsuario.telaMenuMaster();
        resetaValoresGlobais();
      }

      if (teclaAtual == '1')
      {
        // abre porta
      }
      else if (teclaAtual == '2')
      {
        estadoAtualSistema = CADASTRO_DIGITANDO_NOME;
        teclaAtual = '\0';
      }
      else if (teclaAtual == '3')
      {
        estadoAtualSistema = REMOVE_USUARIO_INFORME_ID;
        teclaAtual = '\0';
      }
      else if (teclaAtual == '4')
      {
        teclaAtual = '\0';
        estadoAtualSistema = INICIO;
      }
      else if (teclaAtual == '5')
      {
        estadoAtualSistema = INICIO;
        teclaAtual = '\0';
        digital.apagarTodasDigitais();
      }
      else if (teclaAtual == '6')
      {
        estadoAtualSistema = CADASTRO_PREPARAR_PARA_FOTO;
        teclaAtual = '\0';
      }
    }

    // else if (estadoAtualSistema == REMOVE_USUARIO_CONFIRMA_ID)
    // {
    //   estadoAnteriorSistema = estadoAtualSistema;
    //   // Executa toda hora
    //   if (teclaAtual != '\0' && teclaAtual != '#')
    //   {
    //     teclado.armazenaDigito(teclaAtual);
    //   }
    //   msgUsuario.telaConfirmaRemocaoUsuario(teclado.digitosArmazenados);

    //   // Transições
    //   if (teclaAtual == '#')
    //   {
    //     id = teclado.digitosArmazenados;
    //     estadoAtualSistema = REMOVENDO_USUARIO;
    //     teclaAtual = '\0';
    //     teclado.limpaDigitosArmazenados();
    //   }
    // }

    // else if (estadoAtualSistema == REMOVENDO_USUARIO)
    // {
    //   estadoAnteriorSistema = estadoAtualSistema;
    //   File file = SD.open("/registros.txt", "r");
    //   File fileTemp = SD.open("/temp.txt", "w");
    //   Usuario usuarioRemovido = registroUsuario.recuperaUsuario(file, id.toInt(), senha, TECLADO);

    //   if (usuarioRemovido.id != -1)
    //   {
    //     usuarioRemovido.idBiometria = -1; // remove a biometria do usuario removido
    //     registroUsuario.removeUsuarioSdCard(file, fileTemp, usuarioRemovido);
    //   }
    // }
    else if (estadoAtualSistema == CADASTRO_DIGITANDO_NOME)
    {
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        msgUsuario.telaCadastroDigiteNome("");
        ultimaTeclaNaoNula = '\0';
        timer = millis();
        estadoAnteriorSistema = estadoAtualSistema;
      }

      if (letraAtual == '\0')
      {
        if (teclaAtual >= '0' && teclaAtual <= '9')
        {
          letraAtual = teclado.TECLAS_ALFABETO[teclaAtual - '0'].charAt(0);
          msgUsuario.telaCadastroDigiteNome(nomeUsuario + letraAtual);
          timer = millis();
        }
      }
      else
      {
        if (millis() - timer > 1000)
        {
          nomeUsuario += letraAtual;
          msgUsuario.telaCadastroDigiteNome(nomeUsuario);
          letraAtual = '\0';
          contadorLetras = 0;
        }
        else if (teclaAtual >= '0' && teclaAtual <= '9')
        {
          if (teclaAtual != ultimaTeclaNaoNula)
          {
            nomeUsuario += letraAtual;
            letraAtual = teclado.TECLAS_ALFABETO[teclaAtual - '0'].charAt(0);
            msgUsuario.telaCadastroDigiteNome(nomeUsuario + letraAtual);
            contadorLetras = 0;
            timer = millis();
          }
          else
          {
            contadorLetras++;
            String grupoLetras = teclado.TECLAS_ALFABETO[teclaAtual - '0'];
            letraAtual = grupoLetras.charAt(contadorLetras % grupoLetras.length());
            msgUsuario.telaCadastroDigiteNome(nomeUsuario + letraAtual);
            timer = millis();
          }
        }
      }
      if (teclaAtual == 'B') // backspace
      {
        // remove a letra ativa
        if (letraAtual != '\0')
        {
          letraAtual = '\0';
          msgUsuario.telaCadastroDigiteNome(nomeUsuario);
        }
        else if (nomeUsuario.length() > 0)
        {
          nomeUsuario.remove(nomeUsuario.length() - 1);
          msgUsuario.telaCadastroDigiteNome(nomeUsuario);
        }
      }
      if (teclaAtual == '#')
      {
        estadoAtualSistema = CADASTRO_DIGITANDO_SENHA;
      }
    }

    else if (estadoAtualSistema == CADASTRO_DIGITANDO_SENHA)
    {
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        // File file = SD.open("/registros.txt", "r");
        File file = SPIFFS.open("/usuarios.txt", "r");
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
      estadoAnteriorSistema = estadoAtualSistema;

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
        estadoAtualSistema = CADASTRO_DIGITANDO_SENHA;
      }
    }
    else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaCadastroBiometriaEncosteDedo();
      if (digital.leitorTocado())
      {
        // if (digital.identificaUsuario() != -1)
        // {
        //   estadoAtualSistema = CADASTRO_BIOMETRIA_JA_CADASTRADA_ERRO;
        // }
        // else
        // {
        //   bool primeiraImagemOk = digital.iniciaCriacaoDigital();
        //   if (primeiraImagemOk)
        //   {
        //     estadoAtualSistema = CADASTRO_BIOMETRIA_RETIRE_DEDO;
        //   }
        //   // else erro
        // }
        bool primeiraImagemOk = digital.iniciaCriacaoDigital();
        if (primeiraImagemOk)
        {
          estadoAtualSistema = CADASTRO_BIOMETRIA_RETIRE_DEDO;
        }
      }
    }
    else if (estadoAtualSistema == CADASTRO_BIOMETRIA_JA_CADASTRADA_ERRO)
    {
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        timer = millis();
        estadoAnteriorSistema = estadoAtualSistema;
      }
      msgUsuario.telaCadastroBiometriaJaCadastradaErro();
      if (millis() - timer > 3000)
      {
        estadoAtualSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO;
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
        // File file = SD.open("/registros.txt", "r");
        File file = SPIFFS.open("/usuarios.txt", "r");
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
      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        timer = millis();
        estadoAnteriorSistema = estadoAtualSistema;
      }
      msgUsuario.telaBiometriaCadastradaSucesso();

      if (millis() - timer > 3000)
      {
        estadoAtualSistema = CADASTRO_PREPARAR_PARA_FOTO;
        // tirar foto usuário
      }
    }

    else if (estadoAtualSistema == CADASTRO_PREPARAR_PARA_FOTO)
    {
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        estadoAnteriorSistema = estadoAtualSistema;
        timer = millis();
        msgUsuario.telaCadastroPrepararParaFoto();
      }
      if (millis() - timer > 5000)
      {
        estadoAtualSistema = CADASTRO_TIRANDO_FOTO;
      }
    }

    else if (estadoAtualSistema == CADASTRO_TIRANDO_FOTO)
    {
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        msgUsuario.telaCadastroTirandoFoto();
        estadoAnteriorSistema = estadoAtualSistema;
        comunicacaoSerial.tirarFotos(String(idGerado), nomeUsuario);
      }

      if (Serial.available())
      {
        String resposta = Serial.readStringUntil('\n'); // Lê até o \n
        Serial.print("Recebido pela serial: ");
        Serial.println(resposta); // debug
        resposta.trim();
        if (resposta == "cadastro_finalizado")
        {
          estadoAtualSistema = CADASTRO_INFORMA_TIPO_USUARIO;
        }
      }
    }

    else if (estadoAtualSistema == CADASTRO_INFORMA_TIPO_USUARIO)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaCadastroInformeTipoUsuario();
      if (teclaAtual == '0')
      {
        tipoUsuario = COMUM;
        estadoAtualSistema = SALVA_USUARIO_SD_CARD;
      }
      else if (teclaAtual == '1')
      {
        tipoUsuario = MASTER;
        estadoAtualSistema = SALVA_USUARIO_SD_CARD;
      }
    }
    else if (estadoAtualSistema == SALVA_USUARIO_SD_CARD)
    {
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        // abre o arquivo no modo de escrita "append", que escreve no final do arquivo e mantem o conteudo
        // File file = SD.open("/registros.txt", "a");
        File file = SPIFFS.open("/usuarios.txt", "a");
        // chama a funcao que salva no SD card
        Usuario usuario;
        usuario.id = idGerado;
        usuario.idBiometria = idBiometria;
        usuario.nome = nomeUsuario;
        usuario.tipo = tipoUsuario;
        usuario.senha = senha;

        registroUsuario.salvaUsuarioSdCard(file, usuario);
        file.close();
        estadoAnteriorSistema = estadoAtualSistema;
        backup.backupUsuarios();
      }
      msgUsuario.telaUsuarioCadastrado();
      if (teclaAtual == '#')
      {
        // futuramente, adicionar reconhecimento facial
        estadoAtualSistema = INICIO;
      }
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
    // else if (estadoAtualSistema == REMOVE_USUARIO_INFORME_ID)
    // {
    //   if (estadoAtualSistema != estadoAnteriorSistema)
    //   {
    //     estadoAnteriorSistema = estadoAtualSistema;
    //   }
    //   // Executa toda hora
    //   if (teclaAtual != '\0' && teclaAtual != '#')
    //   {
    //     teclado.armazenaDigito(teclaAtual);
    //   }

    //   msgUsuario.desenhaTelaDigiteId(teclado.digitosArmazenados);

    //   // Transições
    //   if (teclaAtual == '#')
    //   {
    //     id = teclado.digitosArmazenados;
    //     estadoAtualSistema = REMOVE_USUARIO_CONFIRMA_ID;
    //     teclaAtual = '\0';
    //     teclado.limpaDigitosArmazenados();
    //     Serial.println("ID informado: " + id);
    //   }
    // }

    else if (estadoAtualSistema == REMOVE_USUARIO_INFORME_ID)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      // Executa toda hora
      if (teclaAtual != '\0' && teclaAtual != '#')
      {
        teclado.armazenaDigito(teclaAtual);
      }
      msgUsuario.telaDigiteIdRemoveUsuario(teclado.digitosArmazenados);

      // Transições
      if (teclaAtual == '#')
      {
        id = teclado.digitosArmazenados;
        estadoAtualSistema = REMOVE_USUARIO_CONFIRMA_ID;
        teclaAtual = '\0';
        teclado.limpaDigitosArmazenados();
      }
    }

    else if (estadoAtualSistema == REMOVE_USUARIO_CONFIRMA_ID)
    {

      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        Serial.println("Mudou para estado CONFIRMA ID");
        estadoAnteriorSistema = estadoAtualSistema;

        // File file = SD.open("/registros.txt", "r");
        File file = SPIFFS.open("/usuarios.txt", "r");
        stringEncontrada = registroUsuario.buscaIdNoArquivo(file, id.toInt());
        Serial.println("String encontrada: " + stringEncontrada);

        if (stringEncontrada != "")
        {
          Usuario user = registroUsuario.transformaTextoEmUsuario(stringEncontrada);
          Serial.println("Usuario encontrado: " + user.nome);
          nomeUsuario = user.nome;
          msgUsuario.telaConfirmaRemocaoUsuario(nomeUsuario);
        }
        else
        {
          estadoAtualSistema = REMOVE_USUARIO_ID_NAO_ENCONTRADO;
          teclaAtual = '\0';
          teclado.limpaDigitosArmazenados();
          file.close();
          return;
        }
        file.close();
      }

      if (teclaAtual != '\0') // só reage se houver tecla
      {
        if (teclaAtual == '#')
        {
          Serial.println("Confirmou com #");
          estadoAtualSistema = REMOVENDO_USUARIO;
        }
        else if (teclaAtual == '*')
        {
          Serial.println("Cancelou com *");
          estadoAtualSistema = MENU_USUARIO_MASTER;
        }

        teclaAtual = '\0';
        teclado.limpaDigitosArmazenados();
      }
    }

    else if (estadoAtualSistema == REMOVE_USUARIO_ID_NAO_ENCONTRADO)
    {
      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        estadoAnteriorSistema = estadoAtualSistema;
      }
      msgUsuario.telaRemoveUsuarioIdNaoEncontrado();

      if (teclaAtual == '#')
      {
        estadoAtualSistema = REMOVE_USUARIO_INFORME_ID;
        teclaAtual = '\0';
      }
      else if (teclaAtual == '*')
      {
        estadoAtualSistema = MENU_USUARIO_MASTER;
        teclaAtual = '\0';
      }
    }
    else if (estadoAtualSistema == REMOVENDO_USUARIO)
    {
      if (estadoAnteriorSistema != estadoAtualSistema)
      {
        Serial.println("ESTADO REMOVENDO USUARIO");
        timer = millis();
        // File file = SD.open("/registros.txt", "r");
        File file = SPIFFS.open("/usuarios.txt", "r");
        if (!file)
        {
          Serial.println("Erro ao abrir usuarios.txt para leitura!");
          return;
        }
        File fileTemp = SPIFFS.open("/usuariosTemp.txt", "w");
        if (!fileTemp)
        {
          Serial.println("Erro ao criar usuariosTemp.txt!");
          // file.close();
          return;
        }
        usuarioRemovido = registroUsuario.removeUsuarioSdCard(file, fileTemp, registroUsuario.transformaTextoEmUsuario(stringEncontrada));
        file.close();
        fileTemp.close();
        estadoAnteriorSistema = estadoAtualSistema;

        if (usuarioRemovido)
        {
          Serial.println("Usuario removido com sucesso!:");
          Serial.println(usuarioRemovido);
          SPIFFS.remove("/usuarios.txt");
          SPIFFS.rename("/usuariosTemp.txt", "/usuarios.txt");
          // SD.remove("/usuarios.txt");
          // SD.rename("/usuariosTemp.txt", "/usuarios.txt");
          digital.apagarDigital(registroUsuario.transformaTextoEmUsuario(stringEncontrada).idBiometria);
          comunicacaoSerial.removerUsuario(String(registroUsuario.transformaTextoEmUsuario(stringEncontrada).id));
          msgUsuario.telaUsuarioRemovidoComSucesso();
          contaArquivosRemovidos++;

          estadoAtualSistema = USUARIO_REMOVIDO_COM_SUCESSO;
        }
        else
        {
          estadoAtualSistema = ERRO_REMOVER_USUARIO;
        }
      }
    }
    else if (estadoAtualSistema == USUARIO_REMOVIDO_COM_SUCESSO)
    {
      // Executa só na entrada
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        timer = millis();
        backup.backupUsuarios();
        estadoAnteriorSistema = estadoAtualSistema;
        msgUsuario.telaUsuarioRemovidoComSucesso();
      }
      if (millis() - timer > 3000)
      {
        estadoAtualSistema = MENU_USUARIO_MASTER;
      }
    }
    else if (estadoAtualSistema == ERRO_REMOVER_USUARIO)
    {
      // Executa só na entrada
      if (estadoAtualSistema != estadoAnteriorSistema)
      {
        timer = millis();
        msgUsuario.telaErroAoRemoverUsuario();
        estadoAnteriorSistema = estadoAtualSistema;
      }
      if (millis() - timer > 3000)
      {
        estadoAtualSistema = MENU_USUARIO_MASTER;
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
    if (ultimaTecla != '\0')
    {
      ultimaTeclaNaoNula = ultimaTecla;
    }
    // estadoAnteriorSistema = estadoAtualSistema;
  }
