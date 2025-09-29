#include <Arduino.h>
#include "Biometria.h"         // biblioteca para o sensor biométrico
#include "Display.h"           // biblioteca para o display OLED
#include "Comandos.h"          // biblioteca para os comandos recebidos via serial
#include "Teclado.h"           // biblioteca para o teclado matricial
#include "MensagemUsuario.h"   // biblioteca para as mensagens exibidas ao usuário (display e serial)
#include "TelaSerial.h"        // biblioteca para a "tela" serial
#include "RegistroUsuario.h"   // biblioteca para o registro dos usuários
#include "ComunicacaoSerial.h" // biblioteca para a comunicação serial com outro dispositivo
// sd card e spiffs
#include "FS.h"
#include "SPIFFS.h"
#include "SD.h"
#include "SPI.h"
// servidor
#include "Servidor.h"
#include "servidor.h"
// backup de usuarios
#include <Backup.h>
// wifi
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

// instancias dos objetos
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

char ultimaTecla = '\0';            // Variável para armazenar a última tecla pressionada
char ultimaTeclaNaoNula = '\0';     // Variável para armazenar a última tecla pressionada que não é nula
char letraAtual = '\0';             // Variável para armazenar a letra atual para digitacao do nome
String nomeUsuario;                 // nome do usuario
int contadorLetras = 0;             // contador de letras digitadas
String id;                          // id usado na autenticacao
int idGerado;                       // id gerado para um novo usuario
int idBiometria;                    // idBiometria gerado para um novo usuario
String senha;                       // senha do usuario
String confirmaSenha;               // confirma senha do usuario
TipoUsuario tipoUsuario;            // tipo do usuario
String stringEncontrada;            // string encontrada na busca
Backup backup;                      // objeto para fazer backup dos usuarios do SPIFFS para o SD card
bool usuarioRemovido = false;       // flag para indicar se o usuário foi removido
bool emCadastro = false;            // flag para indicar se o sistema está em modo de cadastro
bool logRegistrado = false;         // flag para indicar se o log de entrada foi registrado
long timer = 0;                     // variável para controlar o tempo de espera em alguns estados
const char *ssid = "House";         // SSID da sua rede Wi-Fi
const char *senhaWifi = "12345678"; // Senha da rede Wi-Fi
const int pinoTranca = 25;          // Pino ligado à fechadura

// Criação do objeto servidor
// Servidor servidor(ssid, senhaWifi, pinoTranca);

// Função para resetar as variáveis globais
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
  senha = "";                // senha do usuario
  confirmaSenha = "";        // confirma senha do usuario
  tipoUsuario = COMUM;       // tipo do usuario
  stringEncontrada = "";     // string encontrada na busca
  usuarioRemovido = false;   // flag para indicar se o usuário foi removido
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, senhaWifi);
  // Conecta-se à rede Wi-Fi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Wire.begin(SDA_PIN, SCL_PIN);
  // Inicializa os módulos
  digital.setupFingerprintSensor();
  displayOled.displaySetup();
  teclado.setupKeypad();

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

  // hora atual
  configTime(-3 * 3600, 0, "pool.ntp.org"); // GMT-3 para Brasil
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    Serial.println(&timeinfo, "%d/%m/%Y %H:%M:%S");
  }

  // pino da tranca
  pinMode(pinoTranca, OUTPUT);

  // Faz o backup de usuários do SPIFFS para o SD card
  backup.backupUsuarios();

  // Mostra o que foi salvo no SD card (TESTE)
  backup.imprimirBackup();

  // Lista os arquivos do SPIFFS (teste)
  File logFile = SPIFFS.open("/logs.txt", "r");
  while (logFile.available())
  {
    Serial.write(logFile.read());
  }
  logFile.close();
}

void loop()
{

  // Lê a tecla pressionada
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

    if (digital.leitorTocado()) // Verifica se o leitor biométrico foi tocado
    {
      int id = digital.identificaUsuario(); // Verifica se o usuário está cadastrado no sensor biométrico
      if (id != -1)                         // Se o ID for diferente de -1, o usuário foi identificado
      {
        // Serial.println("ID lido: " + String(id));
        File file = SPIFFS.open("/usuarios.txt", "r"); // Abre o arquivo de usuários
        Usuario userEncontrado;
        bool encontrado = false;
        while (file.available())
        {
          String linha = file.readStringUntil('\n');
          Usuario user = registroUsuario.transformaTextoEmUsuario(linha);
          if (user.idBiometria == id) // Compara o ID lido com o ID da biometria do usuário no arquivo
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
            registroUsuario.registrarLogEntrada(logFile, userEncontrado.id, userEncontrado.nome, "BIOMETRIA"); // Registrar log de entrada
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
    else if (Serial.available()) // Verifica se há dados disponíveis na serial
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

  else if (estadoAtualSistema == RECONHECIMENTO_FACIAL_AGUARDANDO) // aguarda reconhecimento facial, após usuario apertar 'C'
  {
    if (estadoAnteriorSistema != estadoAtualSistema)
    {
      msgUsuario.telaAguardandoReconhecimentoFacial();
      // A aplicação em python precisa estar rodando nesse momento
      comunicacaoSerial.iniciarReconhecimentoFacial();
      timer = millis();
      estadoAnteriorSistema = estadoAtualSistema;
    }

    // Aguarda resposta da aplicação Python via Serial
    if (Serial.available())
    {
      String resposta = Serial.readStringUntil('\n');
      resposta.trim();
      if (resposta.startsWith("face_detectada:")) // Exemplo: "face_detectada:1234"
      {
        id = resposta.substring(resposta.indexOf(':') + 1); // Extrai o ID após os dois pontos
        // Serial.print("ID recebido: ");
        // Serial.println(id);
        // Serial.print("ID convertido: ");
        Serial.println(id.toInt());
        estadoAtualSistema = RECONHECIMENTO_FACIAL_PROCESSANDO;
      }
      else if (resposta == "face_nao_reconhecida")
      {
        timer = millis();
        msgUsuario.telaFaceNaoReconhecida();
        Serial.println("Usuario não detectado");
        if (millis() - timer > 3000)
        {
          estadoAtualSistema = INICIO;
        }
      }
    }
  }

  else if (estadoAtualSistema == RECONHECIMENTO_FACIAL_PROCESSANDO)
  {
    File file = SPIFFS.open("/usuarios.txt", "r");                                               // Abre o arquivo de usuários
    Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), "", RECONHECIMENTO_FACIAL); // Busca o usuário pelo ID
    file.close();
    if (user.id != -1)
    {
      msgUsuario.telaUsuarioEncontrado(user.nome);
      digitalWrite(pinoTranca, HIGH);               // Abre a porta
      File logFile = SPIFFS.open("/logs.txt", "a"); // Abre o arquivo de logs para registrar a entrada
      if (logFile)
      {
        registroUsuario.registrarLogEntrada(logFile, user.id, user.nome, "RECONHECIMENTO_FACIAL"); // Registrar log de entrada
        logFile.close();
      }
      delay(4000);
      estadoAtualSistema = INICIO;
      digitalWrite(pinoTranca, LOW); // FECHA a porta
    }
    else
    {
      msgUsuario.telaUsuarioNaoCadastrado();
      delay(3000);
      estadoAtualSistema = INICIO;
    }
  }

  else if (estadoAtualSistema == INSERCAO_ID_USUARIO) // insercao do id do usuario via teclado
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

      if (id.length() > 0) // Verifica se o ID não está vazio
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
  }

  else if (estadoAtualSistema == ERRO_INSERCAO_ID_USUARIO)
  {
    estadoAnteriorSistema = estadoAtualSistema;
    msgUsuario.telaIdNaoInformado(); // Mensagem avisando para pressionar * para retornar ao menu ou # para tentar novamente
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
  else if (estadoAtualSistema == INSERCAO_SENHA_USUARIO) // insercao da senha do usuario via teclado
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

      if (senha.length() > 0) // Verifica se a senha não está vazia
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
  }

  else if (estadoAtualSistema == ERRO_INSERCAO_SENHA_USUARIO) //
  {
    estadoAnteriorSistema = estadoAtualSistema;
    msgUsuario.telaSenhaNaoInformada(); // Mensagem avisando para pressionar * ou tentar novamente
    // timer = millis();
    if (teclaAtual == '*') // volta para o menu inicial
    {
      estadoAtualSistema = INICIO;
      teclado.limpaDigitosArmazenados();
      teclaAtual = '\0';
    }
    else if (teclaAtual == '#') // tenta inserir a senha novamente
    {
      estadoAtualSistema = INSERCAO_SENHA_USUARIO;
      teclado.limpaDigitosArmazenados();
      teclaAtual = '\0';
    }
  }
  else if (estadoAtualSistema == AUTENTICACAO) // autentica o usuario com id e senha
  {
    if (estadoAnteriorSistema != estadoAtualSistema)
    {
      timer = millis();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    File file = SPIFFS.open("/usuarios.txt", "r");                                    // Abre o arquivo de usuários do SPIFFS
    Usuario user = registroUsuario.recuperaUsuario(file, id.toInt(), senha, TECLADO); // Busca o usuário pelo ID e senha
    nomeUsuario = user.nome;
    file.close();
    if (user.id == -1) // Se o ID for -1, o usuário não foi encontrado ou a senha está incorreta
    {
      estadoAtualSistema = USUARIO_NAO_CADASTRADO;
    }
    else
    {
      File logFile = SPIFFS.open("/logs.txt", "a"); // Abre o arquivo de logs para registrar a entrada
      if (logFile && !logRegistrado)
      {
        registroUsuario.registrarLogEntrada(logFile, user.id, user.nome, "teclado"); // Registrar log de entrada
        logFile.close();
        logRegistrado = true;
      }
      if (user.tipo == MASTER) // Se o usuário for master, vai para o menu master
      {
        estadoAtualSistema = MENU_USUARIO_MASTER;
      }
      else
      {
        msgUsuario.telaUsuarioEncontrado(user.nome);
        digitalWrite(pinoTranca, HIGH); // Abre a porta;
        delay(4000);
        digitalWrite(pinoTranca, LOW); // FECHA a porta
        if (millis() - timer > 3000)
        {
          // colocar estado para abrir a porta
          estadoAtualSistema = INICIO;
        }
      }
    }
  }

  else if (estadoAtualSistema == MENU_USUARIO_MASTER) // menu do usuario master
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
      digitalWrite(pinoTranca, HIGH);
      delay(4000);
      digitalWrite(pinoTranca, LOW);
      estadoAtualSistema = INICIO;
      teclaAtual = '\0';
    }
    else if (teclaAtual == '2') // cadastro novo usuario
    {
      estadoAtualSistema = CADASTRO_DIGITANDO_NOME;
      teclaAtual = '\0';
    }
    else if (teclaAtual == '3') // remove usuario
    {
      estadoAtualSistema = REMOVE_USUARIO_INFORME_ID;
      teclaAtual = '\0';
    }
    else if (teclaAtual == '4') // volta para o menu inicial
    {
      teclaAtual = '\0';
      estadoAtualSistema = INICIO;
    }
    // estados de teste
    else if (teclaAtual == '5') // apaga todas digitais (teste)
    {
      estadoAtualSistema = INICIO;
      teclaAtual = '\0';
      digital.apagarTodasDigitais();
    }
    else if (teclaAtual == '6') // cadastro via foto (teste)
    {
      estadoAtualSistema = CADASTRO_PREPARAR_PARA_FOTO;
      teclaAtual = '\0';
    }
  }

  else if (estadoAtualSistema == CADASTRO_DIGITANDO_NOME) // cadastro do nome do usuario via teclado
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
      // teclado funciona como em celulares antigos (pressionar varias vezes a mesma tecla para mudar a letra)
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
    if (teclaAtual == 'B') // backspace (apaga ultima letra)
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

  else if (estadoAtualSistema == CADASTRO_DIGITANDO_SENHA) // informa a senha do novo usuario via teclado
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      File file = SPIFFS.open("/usuarios.txt", "r");
      idGerado = registroUsuario.buscaProximoIdDisponivel(file); // busca o próximo ID disponível no arquivo
      file.close();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }

    msgUsuario.telaCadastroDigiteSenha(teclado.digitosArmazenados, idGerado); // mostra o id gerado e a senha digitada

    if (teclaAtual == '#')
    {
      senha = teclado.digitosArmazenados; // armazena senha digitada
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
      estadoAtualSistema = CADASTRO_CONFIRMANDO_SENHA_USUARIO;
    }
  }
  else if (estadoAtualSistema == CADASTRO_CONFIRMANDO_SENHA_USUARIO) // confirma a senha do novo usuario via teclado

  {
    estadoAnteriorSistema = estadoAtualSistema;

    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }

    msgUsuario.telaCadastroDigiteNovamenteSenha(teclado.digitosArmazenados);

    if (teclaAtual == '#')
    {
      confirmaSenha = teclado.digitosArmazenados; // armazena senha digitada pela 2 vez
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();

      if (senha == confirmaSenha) // compara as senhas digitadas
      {
        estadoAtualSistema = CADASTRO_BIOMETRIA_ENCOSTE_DEDO; // se as senhas estiveram iguais, segue p/ o cadastro da biometria
      }
      else
      {
        estadoAtualSistema = CADASTRO_SENHA_INCORRETA; // se as senhas estiveram diferentes, retorna ao inicio do cadastro
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
    msgUsuario.telaSenhaIncorreta(); // exibe mensagem de senha incorreta e retorno ao cadastro

    if (millis() - timer > 3000) // aguarda 3s para retornar
    {
      estadoAtualSistema = CADASTRO_DIGITANDO_SENHA;
    }
  }
  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO) // aguarda o usuario encostar o dedo no leitor biometrico
  {
    estadoAnteriorSistema = estadoAtualSistema;
    msgUsuario.telaCadastroBiometriaEncosteDedo();
    if (digital.leitorTocado())
    {
      bool primeiraImagemOk = digital.iniciaCriacaoDigital(); // inicia a criação da digital (pega a primeira imagem)
      if (primeiraImagemOk)
      {
        estadoAtualSistema = CADASTRO_BIOMETRIA_RETIRE_DEDO;
      }
    }
  }
  // estado para caso a digital ja esteja cadastrada (não utilizado no momento)
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

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_RETIRE_DEDO) // pede para o usuario retirar o dedo do leitor biometrico
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

  else if (estadoAtualSistema == CADASTRO_BIOMETRIA_ENCOSTE_DEDO_NOVAMENTE) // pede para o usuario encostar o dedo novamente no leitor biometrico
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaCadastroBiometriaEncosteDedoNovamente();
    }
    if (digital.leitorTocado())
    {
      File file = SPIFFS.open("/usuarios.txt", "r");
      idBiometria = registroUsuario.buscaIdBiometriaDisponivel(file); // busca o próximo ID de biometria disponível no arquivo
      file.close();
      if (digital.finalizaCriacaoDigital(idBiometria)) // finaliza a criação da digital (pega a segunda imagem e cria o modelo)
      {
        Serial.println("Biometria cadastrada com sucesso na posicao: " + idBiometria); // debug
        estadoAtualSistema = BIOMETRIA_CADASTRADA_COM_SUCESSO;
      }
    }
  }

  else if (estadoAtualSistema == BIOMETRIA_CADASTRADA_COM_SUCESSO) // informa que a biometria foi cadastrada com sucesso
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
    }
  }

  else if (estadoAtualSistema == CADASTRO_PREPARAR_PARA_FOTO) // prepara para tirar a foto do usuario para salvar encoding facial na tvbox
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

  else if (estadoAtualSistema == CADASTRO_TIRANDO_FOTO) // tira a foto do usuario para salvar encoding facial na tvbox
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      msgUsuario.telaCadastroTirandoFoto();
      estadoAnteriorSistema = estadoAtualSistema;
      comunicacaoSerial.tirarFotos(String(idGerado), nomeUsuario); // envia comando via serial para a aplicação python tirar a foto
    }

    if (Serial.available())
    {
      String resposta = Serial.readStringUntil('\n'); // Lê até o \n
      Serial.print("Recebido pela serial: ");
      Serial.println(resposta); // debug
      resposta.trim();
      if (resposta == "cadastro_finalizado") // aguarda a confirmação da aplicação python que o cadastro foi finalizado
      {
        estadoAtualSistema = CADASTRO_INFORMA_TIPO_USUARIO;
      }
    }
  }

  else if (estadoAtualSistema == CADASTRO_INFORMA_TIPO_USUARIO) // informa o tipo do usuario (comum ou master)
  {
    estadoAnteriorSistema = estadoAtualSistema;
    msgUsuario.telaCadastroInformeTipoUsuario();
    if (teclaAtual == '0') // comum
    {
      tipoUsuario = COMUM;
      estadoAtualSistema = SALVA_USUARIO_SD_CARD;
    }
    else if (teclaAtual == '1') // master
    {
      tipoUsuario = MASTER;
      estadoAtualSistema = SALVA_USUARIO_SD_CARD;
    }
  }
  else if (estadoAtualSistema == SALVA_USUARIO_SD_CARD) // salva o novo usuario no SD card (mudar para SPIFFS)
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      File file = SPIFFS.open("/usuarios.txt", "a"); // abre o arquivo no modo de escrita "append", que escreve no final do arquivo e mantem o conteudo
      Usuario usuario;                               // cria um objeto usuario
      usuario.id = idGerado;                         // atribui os valores ao objeto usuario
      usuario.idBiometria = idBiometria;
      usuario.nome = nomeUsuario;
      usuario.tipo = tipoUsuario;
      usuario.senha = senha;

      registroUsuario.salvaUsuarioSdCard(file, usuario); // salva o usuario no SPIFFS (mudar nome da função)
      file.close();
      estadoAnteriorSistema = estadoAtualSistema;
      backup.backupUsuarios(); // faz o backup dos usuarios do SPIFFS para o SD card
    }
    msgUsuario.telaUsuarioCadastrado();
    if (teclaAtual == '#')
    {
      estadoAtualSistema = INICIO;
    }
  }
  else if (estadoAtualSistema == USUARIO_NAO_CADASTRADO) // informa que o usuario não está cadastrado
  {
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

  else if (estadoAtualSistema == REMOVE_USUARIO_INFORME_ID) // informa o id do usuario a ser removido
  {
    estadoAnteriorSistema = estadoAtualSistema;
    if (teclaAtual != '\0' && teclaAtual != '#')
    {
      teclado.armazenaDigito(teclaAtual);
    }
    msgUsuario.telaDigiteIdRemoveUsuario(teclado.digitosArmazenados);

    if (teclaAtual == '#')
    {
      id = teclado.digitosArmazenados;
      estadoAtualSistema = REMOVE_USUARIO_CONFIRMA_ID;
      teclaAtual = '\0';
      teclado.limpaDigitosArmazenados();
    }
  }

  else if (estadoAtualSistema == REMOVE_USUARIO_CONFIRMA_ID) // confirma o id do usuario a ser removido
  {

    if (estadoAnteriorSistema != estadoAtualSistema)
    {
      estadoAnteriorSistema = estadoAtualSistema;
      File file = SPIFFS.open("/usuarios.txt", "r");                         // Abre o arquivo de usuários do SPIFFS
      stringEncontrada = registroUsuario.buscaIdNoArquivo(file, id.toInt()); // Busca a string do usuário pelo ID
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

  else if (estadoAtualSistema == REMOVE_USUARIO_ID_NAO_ENCONTRADO) // informa que o id do usuario a ser removido não foi encontrado
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
  else if (estadoAtualSistema == REMOVENDO_USUARIO) // remove o usuario
  {
    if (estadoAnteriorSistema != estadoAtualSistema)
    {
      Serial.println("ESTADO REMOVENDO USUARIO");
      timer = millis();
      File file = SPIFFS.open("/usuarios.txt", "r");
      if (!file)
      {
        Serial.println("Erro ao abrir usuarios.txt para leitura!");
        return;
      }
      File fileTemp = SPIFFS.open("/usuariosTemp.txt", "w"); // arquivo temporario para salvar os usuarios que não serao removidos
      if (!fileTemp)
      {
        Serial.println("Erro ao criar usuariosTemp.txt!");
        // file.close();
        return;
      }
      usuarioRemovido = registroUsuario.removeUsuarioSdCard(file, fileTemp, registroUsuario.transformaTextoEmUsuario(stringEncontrada)); // remove o usuario do SPIFFS
      file.close();
      fileTemp.close();
      estadoAnteriorSistema = estadoAtualSistema;

      if (usuarioRemovido) // se o usuario foi removido com sucesso
      {
        Serial.println("Usuario removido com sucesso!:");
        Serial.println(usuarioRemovido);
        SPIFFS.remove("/usuarios.txt");                                                                // apaga o arquivo original
        SPIFFS.rename("/usuariosTemp.txt", "/usuarios.txt");                                           // renomeia o arquivo temporario para o nome original
        digital.apagarDigital(registroUsuario.transformaTextoEmUsuario(stringEncontrada).idBiometria); // apaga a digital do usuario no sensor
        // Envia comando via serial para a aplicação python remover o encoding facial
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
  else if (estadoAtualSistema == USUARIO_REMOVIDO_COM_SUCESSO) // informa que o usuario foi removido com sucesso
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      timer = millis();
      backup.backupUsuarios(); // faz o backup dos usuarios do SPIFFS para o SD card
      estadoAnteriorSistema = estadoAtualSistema;
      msgUsuario.telaUsuarioRemovidoComSucesso();
    }
    if (millis() - timer > 3000)
    {
      estadoAtualSistema = MENU_USUARIO_MASTER;
    }
  }
  else if (estadoAtualSistema == ERRO_REMOVER_USUARIO) // informa que houve um erro ao remover o usuario
  {
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
  else if (estadoAtualSistema == USUARIO_ENCONTRADO) // informa que o usuario foi encontrado
  {
    if (estadoAtualSistema != estadoAnteriorSistema)
    {
      timer = millis();
      msgUsuario.telaMsgUsuarioEncontrado();
      estadoAnteriorSistema = estadoAtualSistema;
    }
    // if (millis() - timer > 3000)
    // {
    //   estadoAtualSistema = INICIO;
    // }
    digitalWrite(pinoTranca, HIGH); // Abre a porta;
    delay(4000);
    digitalWrite(pinoTranca, LOW); // FECHA a porta
    estadoAtualSistema = INICIO;
  }

  ultimaTecla = teclaAtual;
  if (ultimaTecla != '\0')
  {
    ultimaTeclaNaoNula = ultimaTecla;
  }
}
