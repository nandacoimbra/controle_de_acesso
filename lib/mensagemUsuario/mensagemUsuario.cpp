#include "mensagemUsuario.h"

MensagemUsuario::MensagemUsuario(TelaSaida &tela1, TelaSaida &tela2) : telaPrincipal(tela1), telaSecundaria(tela2)
{
}

void MensagemUsuario::telaBemVindo()
{
    String tela = "BEM VINDO!\n"; 
    tela += "\n"; 
    tela += "Selecione o acesso:\n"; 
    tela += "\n";
    tela += "  [C] - Face\n"; 
    tela += "  [#] - ID/Senha\n"; 
    tela += "  [Toque] - Biometria"; 
    telaPrincipal.desenhaTexto(tela);
}


void MensagemUsuario::telaBemVindoMaster()
{
    String tela = "Bem vindo!\n"; 
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}
// void MensagemUsuario::telaBemVindo()
// {
//     String tela = "CONTROLE DE ACESSO\n";
//     tela += "CEFET Leopoldina\n";
//     tela += "Bem Vindo!\n";
//     tela += "Pressione:\n";
//     tela += "C -> reconhecimento facial\n";
//     tela += "# -> login via teclado\n";
//     telaPrincipal.desenhaTexto(tela);
// }

void MensagemUsuario::telaMenuInicial()
{
    String tela = "Controle de Acesso\n";
}

void MensagemUsuario::telaAguardandoReconhecimentoFacial()
{
    String tela = "Aguardando reconhecimento facial...\n";
    tela += "Posicione o rosto corretamente\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaFaceNaoReconhecida()
{
    String tela = "Rosto nao reconhecido\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}
void MensagemUsuario::desenhaTelaDigiteId(String digitos)
{
    String tela = "Digite o ID\n";
    tela += "ID:" + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaIdNaoInformado()
{
    String tela = "ID nao informado\n";
    tela += "Pressione # para tentar novamente\n";
    tela += "Pressione * para retornar ao Menu\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaSenhaNaoInformada()
{
    String tela = "Senha nao informada\n";
    tela += "Pressione # para tentar novamente\n";
    tela += "Pressione * para retornar ao Menu\n";
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
    tela += "Pressione # para sim\n";
    tela += "Pressione * para nao\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaUsuarioRemovidoComSucesso()
{
    String tela = "Usuario removido com sucesso\n";
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
    String tela = "Bem vindo," + nome + "!\n";
    tela += "Acesso liberado\n";
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
    tela += "4 - Menu inicial\n";
    // tela += "5 - Apagar Digitais";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaErroAoRemoverUsuario()
{
    String tela = "Erro ao remover usuario\n";
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
    tela += "Informe uma senha\n";
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
    String tela = "Cadastro da biometria\n";
    tela += "Encoste o dedo no sensor\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroBiometriaEncosteDedoNovamente()
{
    String tela = "Cadastro da biometria\n";
    tela += "Encoste novamente o mesmo dedo no sensor\n";
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

// void MensagemUsuario::telaApagarTodasDigitais()
// {
//     String tela = "Apagando todas as digitais\n";
//     telaPrincipal.desenhaTexto(tela);
//     telaSecundaria.desenhaTexto(tela);
// }

void MensagemUsuario::telaFaceDetectada()
{
    String tela = "Rosto encontrado\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroPrepararParaFoto()
{
    String tela = "Reconhecimento Facial\n";
    tela += "\n";
    tela += "Preparar para foto\n";
    tela += "Posicione o rosto corretamente\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaCadastroTirandoFoto()
{
    String tela = "Tirando foto\n";
    tela += "Aguarde ...\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaConectandoWifi()
{
    String tela = "Conectando ao Wifi...\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaSucessoConexaoWifi()
{
    String tela = "Conexao Wifi bem sucedida\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaFalhaConexaoWifi()
{
    String tela = "Falha na conexao Wifi\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaFalhaConexaoLeitorBiometrico()
{
    String tela = "Falha na conexao com o leitor biometrico\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);

}

void MensagemUsuario::telaSucessoConexaoLeitorBiometrico()
{
    String tela = "Conexao com o leitor biometrico bem sucedida\n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}

void MensagemUsuario::telaVerificandoBiometria()
{
    String tela = "Verificando biometria...\n";
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
