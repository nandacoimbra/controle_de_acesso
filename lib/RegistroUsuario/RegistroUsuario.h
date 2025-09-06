#pragma once

#include <Arduino.h>
// sd card
#include "FS.h"

enum TipoUsuario
{
    COMUM,
    MASTER
};
enum TipoAutenticacao
{
    TECLADO,
    RECONHECIMENTO_FACIAL,
    BIOMETRIA
};

class Usuario
{
public:
    int id = -1;
    int idBiometria = -1;
    String nome;
    TipoUsuario tipo;
    String senha;
};

class RegistroUsuario
{
private:
public:
    //  métodos
    Usuario transformaTextoEmUsuario(String s);
    // nome:XXXXXXXXX,tipo:X,id:XXXXXXXX,senha:XXXXXXXXXXX;
    Usuario recuperaUsuario(Stream &stream, int id, String senha, TipoAutenticacao autenticacao);
    String buscaIdNoArquivo(Stream &stream, int id);
    int buscaProximoIdDisponivel(Stream &stream);
    int buscaIdBiometriaDisponivel(Stream &stream);
    bool salvaUsuarioSdCard(Stream &stream, Usuario usuario);
    bool removeUsuarioSdCard(Stream &stream, Stream &streamTemp, Usuario usuario);
    void registrarLogEntrada(Stream &stream, int idUsuario, String nomeUsuario, String metodoAutenticacao);
};
