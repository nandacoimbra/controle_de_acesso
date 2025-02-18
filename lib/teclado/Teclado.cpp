#include "Teclado.h"

const char Teclado::keys[] = "123A456B789C*0#DNF";
char digitosArmazenados[17] = {0}; // Array para armazenar os dígitos do teclado
uint8_t currentIndex = 0;

Teclado::Teclado() : keyPad(KEYPAD_ADDRESS) // Inicializa o keyPad com o endereço correto
{
    lastKeyPressed = 'N';
}

void Teclado::setupKeypad(void)
{
    if (keyPad.begin() == false)
    {
        Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
        while (1)
            ;
    }
}

char Teclado::teclaPressionada(void)
{
    uint8_t index = keyPad.getKey();
    if (index < 16 && lastKeyPressed != index)
    {
        lastKeyPressed = index;
        return keys[index];
    }
    lastKeyPressed = index;
    return '\0';
}

void Teclado::armazenaDigito(char tecla)
{
    if (tecla == 'B')
    {
        // Verifica se há dígitos armazenados para remover
        if (digitosArmazenados.length() > 0)
        {
            // Remove o último caractere da string
            digitosArmazenados.remove(digitosArmazenados.length() - 1);
        }
    }
    else
    {
        // Adiciona o novo dígito à string
        digitosArmazenados += tecla;
    }
}

void Teclado::limpaDigitosArmazenados(void)
{
    digitosArmazenados.clear();
}
