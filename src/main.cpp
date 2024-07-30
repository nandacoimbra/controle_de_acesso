#include <Arduino.h>
#include "Biometria.h"
#include "Display.h"
#include "Comandos.h"
#include "Teclado.h"

Display displayOled;
Biometria digital(displayOled);
Teclado teclado(displayOled);
Comandos comando(digital);

// define o estado atual do sistema, de acordo com o fluxograma
int estadoSistema = 1;

// String LerSerial(void);

void setup()
{
  Serial.begin(115200);
  digital.setupFingerprintSensor();
  displayOled.displaySetup();
  teclado.setupKeypad();
}

void loop()
{
  char ultimaTecla = teclado.teclaPresionada();
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
    teclado.armazenaDigitos();
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
}
