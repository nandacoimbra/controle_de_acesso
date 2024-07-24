#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"
#include "Comandos.h"

Display displayOled;
Biometria digital(displayOled);
Comandos comando(digital);

// define o estado atual do sistema, de acordo com o fluxograma
int estadoSistema = 1;

// String LerSerial(void);

void setup()
{
  Serial.begin(115200);
  digital.setupFingerprintSensor();
  displayOled.displaySetup();
}

void loop()
{

  if (estadoSistema == 1)
  {
    if (digital.leitorTocado())
    {
      digital.verificarDigital();
    }
    if (Serial.available())
    {
      String comandoSerial = Serial.readString();
      comandoSerial.toUpperCase();
      comando.executarComandos(comandoSerial);
    }
  }
  // comando.CMD = LerSerial();
  // comando.executarComandos();

  // if(estadoSistema == 1){
  //   displayOled.telaInicial();
  // }
  // if(estadoSistema==2){
  //   displayOled.digitarId();
  // }

  // // int id = digital.verificarDigital();
  // if (digital.verificarDigital()==true)
  //   Serial.println("Id = " + (String)digital.verificarDigital());
}
