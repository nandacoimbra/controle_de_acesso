#ifndef comandos_h
#define comandos_h

#include <Arduino.h>
#include "Stream.h"
#include "Biometria.h"


class Comandos
{
  public:
    String comando;
    Biometria digital;
    Comandos(Biometria biometria);
    String buscaComando();
    String lerSerial();
    void executarComandos(String cmd);
};

#endif
