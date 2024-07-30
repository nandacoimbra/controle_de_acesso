#include "Teclado.h"

const char Teclado::keys[] = "123A456B789C*0#DNF";
char digitosArmazenados[17] = {0}; // Array para armazenar os dígitos do teclado
uint8_t currentIndex = 0;

Teclado::Teclado() : keyPad(KEYPAD_ADDRESS) // Inicializa o keyPad com o endereço correto
{
    lastKeyPressed = 'N';
}

Teclado::Teclado(Display &display) : keyPad(KEYPAD_ADDRESS)
{
    Teclado::displayTeclado = display;
}

void Teclado::setupKeypad(void)
{
    Wire.begin(SDA_PIN, SCL_PIN);
    if (keyPad.begin() == false)
    {
        Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
        while (1)
            ;
    }
}

char Teclado::teclaPresionada(void)
{
    uint8_t index = keyPad.getKey();
    if (index < 16 && lastKeyPressed != index)
    {
        lastKeyPressed = index;
        return keys[index];
    }
    return '\0';
}

void Teclado::armazenaDigitos(void)
{
    char tecla = teclaPresionada();
    if (tecla != 0 && currentIndex < 16)
    {
        digitosArmazenados[currentIndex++] = tecla;
        digitosArmazenados[currentIndex] = '\0'; // Null-terminate the string

        // Display the stored digits
        displayTeclado.displayPrint(digitosArmazenados);
        Serial.print(digitosArmazenados);
    }
}