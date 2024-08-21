#include "RegistroUsuario.h"

// nome:XXXXXXXXX,tipo:X,id:XXXXXXXX,senha:XXXXXXXXXXX;
Usuario RegistroUsuario::transformaTextoEmUsuario(String s)
{
    Usuario usuario;
    int startNome = s.indexOf("nome:") + 5;
    int endNome = s.indexOf(",", startNome);
    usuario.nome = s.substring(startNome, endNome);

    int startTipo = s.indexOf("tipo:") + 5;
    int endTipo = s.indexOf(",", startTipo);
    usuario.tipo = (TipoUsuario)s.substring(startTipo, endTipo).toInt();

    int startId = s.indexOf("id:") + 3;
    int endId = s.indexOf(",", startId);
    usuario.id = s.substring(startId, endId).toInt();

    int startSenha = s.indexOf("senha:") + 6;
    int endSenha = s.indexOf(";", startSenha);
    usuario.senha = s.substring(startSenha, endSenha);

    return usuario;
}

Usuario RegistroUsuario::recuperaUsuario(Stream &stream, int id, String senha, TipoAutenticacao autenticacao)
{
    String stringEncontrada = buscaIdNoArquivo(stream, id);
    Usuario usuario = transformaTextoEmUsuario(stringEncontrada);
    if(autenticacao == TECLADO){
        if(usuario.senha == senha){
            return usuario;
        }
    }

    return Usuario();
}

String RegistroUsuario::buscaIdNoArquivo(Stream &stream, int id)
{
    String stringId = "id:" + String(id) + ",";
    int contaAcertos = 0;
    while (stream.available())
    {
        char caracterLido = stream.read();
        char caracterEsperado = stringId.charAt(contaAcertos);

        if (caracterLido == caracterEsperado)
        {
            contaAcertos++;
        }
        else
        {
            contaAcertos = 0;
        }

        if (stringId.length() == contaAcertos)
        {
            break;
        }
    }

    if (stringId.length() == contaAcertos)
    {
        stringId += stream.readStringUntil(';');
        return stringId + ";";
    }

    return "";
}
