#pragma once

#include <Arduino.h>
#include <TelaSaida.h>

class MensagemUsuario
{
private:
    //exemplo Display, Serial, etc
    TelaSaida& telaPrincipal;
    TelaSaida& telaSecundaria;

public:
    // métodos
    MensagemUsuario(TelaSaida *tela1, TelaSaida *tela2);

    void desenhaTelaDigiteId(String digitos = "");
};
