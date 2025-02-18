#pragma once

#include <Arduino.h>
#include <TelaSaida.h>

class MensagemUsuario
{
private:
    // exemplo Display, Serial, etc
    TelaSaida &telaPrincipal;
    TelaSaida &telaSecundaria;

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
    // teste
    void telaMsgUsuarioEncontrado();
    void telaMenuMaster();
    void telaCadastroDigiteNome(String digitos = "");
    void telaCadastroDigiteSenha(String digitos = "", int id = 0);
    void telaCadastroDigiteNovamenteSenha(String digitos = "");
    void telaSenhaIncorreta();
    void telaUsuarioCadastrado();
    void telaCadastroBiometriaEncosteDedo();
    //incluir id
    void telaCadastroBiometriaJaCadastradaErro();
    void telaBiometriaCadastradaSucesso();
    void telaCadastroBiometriaRetireDedo();
    void telaCadastroBiometriaEncosteDedoNovamente();
    void telaCadastroInformeTipoUsuario(); 
 
};
