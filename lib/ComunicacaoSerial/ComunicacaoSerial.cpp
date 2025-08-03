#include "ComunicacaoSerial.h"

void ComunicacaoSerial::tirarFotos(String id, String nomeUsuario)
{
    String comando = "tirar_foto|" + id + "|" + nomeUsuario;
    Serial.println(comando);
}

void ComunicacaoSerial::iniciarReconhecimentoFacial()
{
    Serial.println("iniciar_reconhecimento_facial");
}

void ComunicacaoSerial::removerUsuario(String id)
{
     Serial.println("remover_usuario|" + id);
}
