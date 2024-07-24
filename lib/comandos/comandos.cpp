#include "Comandos.h"

Comandos::Comandos(Biometria biometria) : digital(biometria)
{
  comando = "";
  digital = biometria;
}

String Comandos::buscaComando()
{
  while (!Serial.available())
    delay(100);
  return Serial.readStringUntil('\n');
}

String Comandos::lerSerial()
{
  String comandoSerial = "";
  if (Serial.available())
  {
    comandoSerial = Serial.readString();
    comandoSerial.toUpperCase();
    return comandoSerial;
  }
}

void Comandos::executarComandos(String cmd)
{
  if (cmd.substring(0, 2) == "CD") // Criar Digital: CD=01
  {
    int pos_igual = cmd.indexOf("=", 0);
    String id_str = cmd.substring(pos_igual + 1);
    digital.criarDigital(id_str.toInt());
  }
  else if (cmd.substring(0, 2) == "VD") // Verificar digital
  {
    digital.verificarDigital();
  }
  else if (cmd.substring(0, 2) == "AD") // Deletar digital
  {
    int pos_igual = cmd.indexOf("=", 0);
    String id_str = cmd.substring(pos_igual + 1);
    digital.apagarDigital(id_str.toInt());
  }
  else if (cmd.substring(0, 3) == "ATD") // Deletar digital
  {
    digital.apagarTodasDigitais();
  }
  
}