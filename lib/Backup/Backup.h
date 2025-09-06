#pragma once

#include <Arduino.h>
#include "SPIFFS.h"
#include "SD.h"
#include "SPI.h"

class Backup
{
private:
    

public:
    // métodos
    void backupUsuarios();
    void imprimirBackup();
    void listarArquivosSPIFFS();
 
};
