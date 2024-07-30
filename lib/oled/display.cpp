#include "Display.h"

// teclado e oled usam esses pinos
#define SDA_PIN 22
#define SCL_PIN 23

// configurações oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Display::Display()
{
    // Display::displaySetup();
}

void Display::displaySetup()
{

    // Inicializando a comunicação I2C
    Wire.begin(SDA_PIN, SCL_PIN);
    // Inicializando o display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("Falha ao inicializar o display OLED"));
        for (;;)
            ;
    }
    // Limpando o buffer do display
    display.clearDisplay();
}

void Display::displayConfig()
{
    // Limpando o buffer do display
    display.clearDisplay();
    // Desenhando alguns textos
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
}

void Display::telaInicial()
{
    displayConfig();
    display.println("Bem Vindo!");
    display.println("Controle de Acesso");
    display.println("Pressione #");
    display.display();
    // teste display queimado
    Serial.println("Bem Vindo!");
    Serial.println("Controle de Acesso");
    Serial.println("Pressione #");
}
void Display::menuTeclado(void)
{
    Serial.println("menu teclado");
    displayConfig();
    display.println("1 - Abrir porta");
    display.println("2 - Adicionar usuario");
    display.println("2 - Remover usuario");
    display.display();
    // teste display queimado
    Serial.println("1 - Abrir porta");
    Serial.println("2 - Adicionar usuario");
    Serial.println("2 - Remover usuario");
}
void Display::digitarId()
{
    Serial.println("digitar id");
    displayConfig();
    display.println("Digite o ID");
    display.println("ID: ");
    display.println("Pressione #");
    display.display();
    // teste display queimado
    Serial.println("Digite o ID");
    Serial.println("ID: ");
    Serial.println("Pressione #");
}

void Display::displayPrint(String mensagem)
{
    displayConfig();
    display.println(mensagem);
    display.display();
}