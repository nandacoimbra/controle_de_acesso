#include "mensagemUsuario.h"


MensagemUsuario::MensagemUsuario(TelaSaida *tela1, TelaSaida *tela2) :
telaPrincipal(*tela1), telaSecundaria(*tela2)
{
}

void MensagemUsuario::desenhaTelaDigiteId(String digitos)
{
    String tela = "Digite o ID\n";
    tela += "ID: " + digitos + "\n";
    tela += "Pressione # \n";
    telaPrincipal.desenhaTexto(tela);
    telaSecundaria.desenhaTexto(tela);
}
