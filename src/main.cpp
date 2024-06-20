#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"

Biometria digital;
Display displayOled;

int estadoTela = 1;

void LerSerial(void);

void setup()
{
  Serial.begin(115200);
  digital.setupFingerprintSensor();
  displayOled.displaySetup();
}

void loop()
{
  
  LerSerial();

  // if(estadoTela == 1){
  //   displayOled.telaInicial();
  // }
  // if(estadoTela==2){
  //   displayOled.digitarId();
  // }
  

  // int id = digital.verificarDigital();
  // if (id>0)
  //   Serial.println("Id = " + (String)id);
}

void LerSerial(void)
{
  String CMD_serial = "";
  if (Serial.available())
  {
    CMD_serial = Serial.readString();
    CMD_serial.toUpperCase();
    if (CMD_serial == "?")
    {
      Serial.println("Sistema de controle de acesso.");
    }
    else if (CMD_serial == "??")
    {
      Serial.println("Autora: Fernanda");
    }

    else if (CMD_serial.substring(0, 2) == "CD") // Criar Digital: CD=01
    {
      int pos_igual = CMD_serial.indexOf("=", 0);
      String id_str = CMD_serial.substring(pos_igual + 1);
      digital.criarDigital(id_str.toInt());
    }
    else if (CMD_serial.substring(0, 2) == "VD") // Verificar digital
    {
      digital.verificarDigital();
    }
    else if (CMD_serial.substring(0, 2) == "AD") // Deletar digital
    {
      int pos_igual = CMD_serial.indexOf("=", 0);
      String id_str = CMD_serial.substring(pos_igual + 1);
      digital.apagarDigital(id_str.toInt());
    }
    else if (CMD_serial.substring(0, 3) == "ATD") // Deletar digital
    {
      digital.apagarTodasDigitais();
    }
    else if (CMD_serial == "#") // Interação Display
    {
      estadoTela = 2;

      // if (CMD_serial == "#")
      // {
      //   // SENHA
      // }
    }
  }
}