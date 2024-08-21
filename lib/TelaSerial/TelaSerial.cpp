
#include "TelaSerial.h"

TelaSerial::TelaSerial(HardwareSerial &serial) : serialUsado(serial)
{
}

void TelaSerial::desenhaTexto(String texto)
{
    if (texto != ultimaMsg)
    {
        serialUsado.println("========================||=========================");
        serialUsado.println(texto);
        serialUsado.println("========================||=========================");
        ultimaMsg = texto;
    }
}
