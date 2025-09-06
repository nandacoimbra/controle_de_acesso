#include "Backup.h"

void Backup::backupUsuarios()
{
    File spiffsFile = SPIFFS.open("/usuarios.txt", FILE_READ);
    if (!spiffsFile)
    {
        Serial.println("Não foi possível abrir usuarios.txt no SPIFFS");
        return;
    }

    File sdFile = SD.open("/usuarios_backup.txt", FILE_WRITE);
    if (!sdFile)
    {
        Serial.println("Não foi possível criar usuarios_backup.txt no SD");
        spiffsFile.close();
        return;
    }

    Serial.println("Iniciando backup dos usuários (SPIFFS -> SD)...");
    while (spiffsFile.available())
    {
        sdFile.write(spiffsFile.read());
    }

    spiffsFile.close();
    sdFile.close();

    Serial.println("Backup concluído com sucesso!");
}

void Backup::imprimirBackup()
{
    File file = SD.open("/usuarios_backup.txt", FILE_READ);
    if (!file)
    {
        Serial.println("Erro ao abrir usuarios_backup.txt no SD");
        return;
    }

    Serial.println("Conteúdo do usuarios_backup.txt no SD:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void Backup::listarArquivosSPIFFS()
{
    Serial.println("Arquivos no SPIFFS:");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        Serial.print("  ");
        Serial.print(file.name());
        Serial.print("  (");
        Serial.print(file.size());
        Serial.println(" bytes)");
        file = root.openNextFile();
    }
}
