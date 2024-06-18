#include <Arduino.h>
#include "Biometria.h"

// Biometria digital;

// void LerSerial(void);

void setup()
{
  Serial.begin(115200);
}

void loop()
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
    else if(CMD_serial == "??"){
      Serial.println("Autora Fernanda");
    }
  // int id = digital.verificarDigital();
  // if (id>0)
  //   Serial.println("Id = " + (String)id);
  // LerSerial();
}}

// void LerSerial(void)
// {
//   String CMD_serial = "";
//   if (Serial.available())
//   {
//     CMD_serial = Serial.readString();
//     CMD_serial.toUpperCase();
//     if (CMD_serial == "?")
//     {
//       Serial.println("Sistema de controle de acesso.");
//     }
//     else if (CMD_serial == "??")
//     {
//       Serial.println("Autora: Fernanda");
//     }
//     else{
//     Serial.println("sem resposta");

//     }

    // else if (CMD_serial.substring(0, 2) == "CD") // Criar Digital: CD=01
    // {
    //   int pos_igual = CMD_serial.indexOf("=", 0);
    //   String id_str = CMD_serial.substring(pos_igual+1);
    //   digital.criarDigital(id_str.toInt());
    // }
    // else if (CMD_serial == "??")
    // {
    //   Serial.println("Autora: Fernanda");
    // }
  
// }
// }