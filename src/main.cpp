#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"
#include "Comandos.h"


Display displayOled;
Biometria digital(displayOled);
Comandos comando(digital);

int estadoTela = 1;

String LerSerial(void);

void setup()
{
  Serial.begin(115200);
  //digital.setupFingerprintSensor();
  //displayOled.displaySetup();
}

void loop()
{

  comando.CMD = LerSerial();
  comando.executarComandos();

  if(estadoTela == 1){
    displayOled.telaInicial();
  }
  if(estadoTela==2){
    displayOled.digitarId();
  }

  // int id = digital.verificarDigital();
  if (digital.verificarDigital()==true)
    Serial.println("Id = " + (String)digital.verificarDigital());
}

String LerSerial(void)
{
  String CMD_serial = "";
  if (Serial.available())
  {
    CMD_serial = Serial.readString();
    CMD_serial.toUpperCase();
    return CMD_serial;
  }
}