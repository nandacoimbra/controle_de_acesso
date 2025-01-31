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
    MensagemUsuario(TelaSaida &tela1, TelaSaida &tela2);

    void telaBemVindo();
    void telaMenuInicial();
    void desenhaTelaDigiteId(String digitos = "");
    void desenhaTelaDigiteSenha(String digitos = "");
    void desenhaTelaAutenticacao();
    void telaUsuarioNaoCadastrado();
    void telaUsuarioEncontrado(String nome);
    void telaMenuMaster();
    void telaCadastroDigiteSenha(String digitos = "");
    void telaCadastroDigiteNovamenteSenha(String digitos = "");
    void telaUsuarioCadastrado();
    void telaCadastroBiometriaEncosteDedo();
    void telaBiometriaJaCadastrada(int id);
    void telaBiometriaCadastradaSucesso();
    void telaCadastroBiometriaRetireDedo();
    void telaCadastroBiometriaEncosteDedoNovamente();
};
