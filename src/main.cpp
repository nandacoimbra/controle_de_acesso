#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"
#include "Comandos.h"
#include "Teclado.h"
#include "MensagemUsuario.h"
#include <TelaSerial.h>

#define SDA_PIN 22
#define SCL_PIN 23

Display displayOled;
Biometria digital;
Teclado teclado;
Comandos comando(digital);
TelaSerial telaSerial(Serial);
MensagemUsuario msgUsuario(&displayOled, &telaSerial);

// define o estado atual do sistema, de acordo com o fluxograma
int estadoSistema = 1;

// String LerSerial(void);

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  digital.setupFingerprintSensor();
  displayOled.displaySetup();
  teclado.setupKeypad();

}

void loop()
{
  char ultimaTecla = teclado.teclaPressionada();
  if (estadoSistema == 1)
  {
    msgUsuario.desenhaTelaDigiteId();
    if (digital.leitorTocado())
    {
      digital.identificaUsuario();
    }
    if (Serial.available())
    {
      String comandoSerial = Serial.readString();
      comandoSerial.toUpperCase();
      comando.executarComandos(comandoSerial);
      Serial.println("teste serial");
    }
    if (ultimaTecla != '\0')
    {
      Serial.println("chegou aqui");
      Serial.println(ultimaTecla);
      estadoSistema = 2;
    }
  }

  if (estadoSistema == 2)
  {
    displayOled.menuTeclado();

    if (ultimaTecla == '2')
    {
      estadoSistema = 3;
    }
  }

  if (estadoSistema == 3)
  {
    displayOled.digitarId();
    teclado.armazenaDigito('0');
  }
  // comando.CMD = LerSerial();
  // comando.executarComandos();

  // if(estadoSistema == 1){
  //   display.telaInicial();
  // }
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
}
