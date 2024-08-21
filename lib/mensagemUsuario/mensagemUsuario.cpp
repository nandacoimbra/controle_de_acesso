#include "mensagemUsuario.h"

MensagemUsuario::MensagemUsuario(TelaSaida &tela1, TelaSaida &tela2) : telaPrincipal(tela1), telaSecundaria(tela2)
{
}

void MensagemUsuario::telaBemVindo()
{
    String tela = "Sistema de Controle de Acesso\n";
    tela += "CEFET Leopoldina\n";
    tela += "Bem Vindo!\n";
    tela += "Pressione #\n";
    telaPrincipal.desenhaTexto(tela);
}

void MensagemUsuario::telaMenuInicial()
{
    String tela = "Controle de Acesso\n";
}

void MensagemUsuario::desenhaTelaDigiteId(String digitos)
{
    String tela = "Digite o ID\n";
    tela += "ID:" + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::desenhaTelaDigiteSenha(String digitos)
{
    for (int i = 0; i < digitos.length(); i++)
    {
        digitos[i] = '*';
    }

    String tela = "Digite a senha\n";
    tela += "Senha:" + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}
void MensagemUsuario::desenhaTelaAutenticacao()
{
    String tela = "Autenticacao\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaUsuarioNaoCadastrado()
{
    String tela = "Usuario nao cadastrado";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaUsuarioEncontrado(String nome)
{
    String tela = "Bem vindo, "+nome+"!";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}
