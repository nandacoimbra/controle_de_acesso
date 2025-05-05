#include "ComunicacaoSerial.h"

void ComunicacaoSerial::tirarFotos(String id, String nomeUsuario)
{
    String comando = "tirar_foto|" + id + "|" + nomeUsuario;
    Serial.println(comando);
}