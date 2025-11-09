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
    void telaBemVindoMaster();
    void telaMenuInicial();
    void telaAguardandoReconhecimentoFacial();
    void telaFaceNaoReconhecida();
    void desenhaTelaDigiteId(String digitos = "");
    void telaIdNaoInformado();
    void telaSenhaNaoInformada();
    void desenhaTelaDigiteSenha(String digitos = "");
    void desenhaTelaAutenticacao();
    void telaUsuarioNaoCadastrado();
    void telaUsuarioEncontrado(String nome);
    void telaMsgUsuarioEncontrado();
    void telaMenuMaster();
    void telaDigiteIdRemoveUsuario(String digitos = "");
    void telaRemoveUsuarioIdNaoEncontrado();
    void telaConfirmaRemocaoUsuario(String nomeUsuario = "");
    void telaUsuarioRemovidoComSucesso();
    void telaErroAoRemoverUsuario();
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
    void telaApagarTodasDigitais(); 
    void telaFaceDetectada();
    void telaCadastroPrepararParaFoto();
    void telaCadastroTirandoFoto();
    void telaConectandoWifi();
    void telaSucessoConexaoWifi();
    void telaFalhaConexaoWifi();
    void telaFalhaConexaoLeitorBiometrico();
    void telaSucessoConexaoLeitorBiometrico();
    void telaVerificandoBiometria();

 
};
