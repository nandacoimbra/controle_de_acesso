#include "Comandos.h"

Comandos::Comandos(Biometria D) : digital(D)
{
  CMD = "";
 // digital = D;
}

String Comandos::buscaComando()
{
  while(!Serial.available()) delay(100);
  return Serial.readStringUntil('\n');
}

void Comandos::executarComandos()
{
    if (CMD.substring(0, 2) == "CD") // Criar Digital: CD=01
    {
      int pos_igual = CMD.indexOf("=", 0);
      String id_str = CMD.substring(pos_igual + 1);
      //digital.criarDigital(id_str.toInt());
    }
    else if (CMD.substring(0, 2) == "VD") // Verificar digital
    {
      //digital.verificarDigital();
    }
    else if (CMD.substring(0, 2) == "AD") // Deletar digital
    {
      int pos_igual = CMD.indexOf("=", 0);
      String id_str = CMD.substring(pos_igual + 1);
      //digital.apagarDigital(id_str.toInt());
    }
    else if (CMD.substring(0, 3) == "ATD") // Deletar digital
    {
      //digital.apagarTodasDigitais();
    }
    else if (CMD == "#") // Interação Display
    {
      //estadoTela = 2;

      // if (CMD == "#")
      // {
      //   // SENHA
      // }
    }
}