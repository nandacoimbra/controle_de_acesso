#ifndef comunicacaoSerial_h
#define comunicacaoSerial_h

#include <Arduino.h>
#include "Display.h"



class ComunicacaoSerial
{


public:
    // métodos
    void tirarFotos(String id, String nomeUsuario);
    void iniciarReconhecimentoFacial();
};

#endif
