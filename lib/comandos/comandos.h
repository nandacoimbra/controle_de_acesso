#ifndef comandos_h
#define comandos_h

#include <Arduino.h>
#include "Stream.h"
#include "Biometria.h"


class Comandos
{
  public:
    String CMD;
    Biometria digital;
    Comandos(Biometria D);
    String buscaComando();
    void executarComandos();
};

#endif
