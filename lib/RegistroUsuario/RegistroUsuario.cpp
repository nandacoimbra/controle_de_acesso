#include "RegistroUsuario.h"

// Formato esperado da string: "nome:XXXXXXXXX,tipo:X,id:XXXXXXXX,senha:XXXXXXXXXXX;"
Usuario RegistroUsuario::transformaTextoEmUsuario(String s)
{
    Usuario usuario;
    
    // Extrai o nome do usuário
    int startNome = s.indexOf("nome:") + 5;
    int endNome = s.indexOf(",", startNome);
    usuario.nome = s.substring(startNome, endNome);

    // Extrai o tipo de usuário
    int startTipo = s.indexOf("tipo:") + 5;
    int endTipo = s.indexOf(",", startTipo);
    usuario.tipo = (TipoUsuario)s.substring(startTipo, endTipo).toInt();

    // Extrai o ID do usuário
    int startId = s.indexOf("id:") + 3;
    int endId = s.indexOf(",", startId);
    usuario.id = s.substring(startId, endId).toInt();

    // Extrai a senha do usuário
    int startSenha = s.indexOf("senha:") + 6;
    int endSenha = s.indexOf(";", startSenha);
    usuario.senha = s.substring(startSenha, endSenha);

    // Retorna o objeto Usuario preenchido
    return usuario;
}

// Função para recuperar um usuário do fluxo de dados com base no ID e senha fornecidos
Usuario RegistroUsuario::recuperaUsuario(Stream &stream, int id, String senha, TipoAutenticacao autenticacao)
{
    
    // Busca o usuário com o ID fornecido no fluxo de dados
    String stringEncontrada = buscaIdNoArquivo(stream, id);
    Usuario usuario = transformaTextoEmUsuario(stringEncontrada);
    if(autenticacao == TECLADO){
        if(usuario.senha == senha){
            return usuario;
        }
    }

    // Se a autenticação falhar, retorna um objeto Usuario vazio
    return Usuario();
}

// Função para buscar uma string correspondente a um ID específico no fluxo de dados
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

    //verifica se terminou de ler o arquivo e se o id foi encontrado
    if (stringId.length() == contaAcertos)
    {
        stringId += stream.readStringUntil(';');
        return stringId + ";";
    }

    return "";
}

String RegistroUsuario::cadastraUsuario(Stream &stream, int id, int senha)
{
    return String();
}


