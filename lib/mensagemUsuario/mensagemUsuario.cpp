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

void MensagemUsuario::telaDigiteIdRemoveUsuario(String digitos)
{
    String tela = "Digite o ID do usuario a ser removido\n";
    tela += "ID:" + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaRemoveUsuarioIdNaoEncontrado()
{
    String tela = "Usuario nao encontrado\n";
    tela += "Pressione # para tentar novamente\n";
    tela += "Pressione * para retornar ao Menu\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaConfirmaRemocaoUsuario(String nomeUsuario)
{
    String tela = "Confirma remocao do usuario" + nomeUsuario + "?\n";
    tela += "Pressione # para SIM\n";
    tela += "Pressione * para NAO\n";
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
    String tela = "Bem vindo, " + nome + "!";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaMsgUsuarioEncontrado()
{
    String tela = "Usuario encontrado";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaMenuMaster()
{
    String tela = "Bem vindo\n";
    tela += "1 - Abrir porta\n";
    tela += "2 - Cadastrar usuario\n";
    tela += "3 - Remover usuario\n";
    tela += "4 - Menu inicial";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroDigiteNome(String digitos)
{
    String tela = "Informe o nome do usuario: \n";
    tela += digitos + "\n";
    tela += "Pressione # para confirmar\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroDigiteSenha(String digitos, int id)
{
    for (int i = 0; i < digitos.length(); i++)
    {
        digitos[i] = '*';
    }

    String tela = "ID gerado: " + String(id) + "\n";
    tela += "Informe uma senha numerica\n";
    tela += "Senha:" + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroDigiteNovamenteSenha(String digitos)
{
    for (int i = 0; i < digitos.length(); i++)
    {
        digitos[i] = '*';
    }

    String tela = "Digite novamente a senha\n";
    tela += "Senha:" + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaSenhaIncorreta()
{
    String tela = "Senha incorreta!\n";
    tela += "Retornando a tela de cadastro ...\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaUsuarioCadastrado()
{
    String tela = "Usuario cadastrado com sucesso\n";
    tela+= "Aperte # para retornar ao Menu Inicial\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroBiometriaEncosteDedo()
{
    String tela = "=Cadastro biometria=\n";
    tela += "Encoste o dedo no sensor\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroBiometriaEncosteDedoNovamente()
{
    String tela = "=Cadastro biometria=\n";
    tela += "Encoste o MESMO dedo novamente\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroInformeTipoUsuario()
{
    String tela = "Informe o tipo de usuario: \n";
    tela += "0 - Comum\n";
    tela += "1 - Master (Admin)\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}


void MensagemUsuario::telaCadastroBiometriaJaCadastradaErro()
{
    String tela = "Biometria ja cadastrada";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaBiometriaCadastradaSucesso()
{
    String tela = "Biometria cadastrada com sucesso ...\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroBiometriaRetireDedo()
{
    String tela = "Retire o dedo do sensor\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}
