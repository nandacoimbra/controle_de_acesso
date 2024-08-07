
#include "TelaSerial.h"


TelaSerial::TelaSerial(HardwareSerial serial) : serialUsado(serial)
{
}

void TelaSerial::desenhaTexto(String texto)
{
    serialUsado.println("========================||=========================");
    serialUsado.println(texto);
    serialUsado.println("========================||=========================");
}
