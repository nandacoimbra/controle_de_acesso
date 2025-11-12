#include "ComunicacaoSerial.h"

//função utilizada na etapa de cadastro do usuário para tirar fotos e associá-las ao ID e nome do usuário
void ComunicacaoSerial::tirarFotos(String id, String nomeUsuario)
{
    String comando = "tirar_foto|" + id + "|" + nomeUsuario;
    Serial.println(comando);
}

//função utilizada para iniciar o reconhecimento facial na aplicação python
void ComunicacaoSerial::iniciarReconhecimentoFacial()
{
    Serial.println("iniciar_reconhecimento_facial");
}

//função utilizada para remover o usuário com o ID especificado
void ComunicacaoSerial::removerUsuario(String id)
{
     Serial.println("remover_usuario|" + id);
}
