## Controle de Acesso a Ambientes por Reconhecimento Facial
Este projeto consiste no desenvolvimento de um protótipo de sistema embarcado de controle de acesso a ambientes, capaz de autenticar usuários via reconhecimento facial, biometria (impressão digital) e senha numérica.
A arquitetura do hardware é centrada no microcontrolador **ESP32-WROOM-32U**, responsável pelo gerenciamento de periféricos (**teclado, display OLED, relé e leitor biométrico**) e pelo armazenamento de **logs** de acesso em cartão SD. O reconhecimento facial é processado externamente por um módulo inteligente (Single Board Computer/**TV Box** com Linux Armbian), que executa algoritmos de **visão computacional** em Python e comunica-se com o controlador principal via interface Serial (UART). Vídeo do protótipo final: https://www.youtube.com/watch?v=7OTgbEgAXRU&list=PLtn4Sv8Hu2Zzf-eZot-XWZFwP5ZhO-jWc.

(Veja o código do Módulo de Reconhecimento Facial [aqui](https://github.com/nandacoimbra/Reconhecimento-Facial)

O sistema foi desenvolvido como Trabalho de Conclusão de Curso (TCC) em Engenharia de Controle e Automação no CEFET-MG (Campus Leopoldina), sob a orientação do Prof. Matusalém Martins Lanes.

<div align="center">
  <img width="500" height="800" alt="FluxogramaTCC-Fluxo Geral drawio" src="https://github.com/user-attachments/assets/513296d5-fb62-4866-9b06-9cd6f60a85f2"/> 
  <img width="500" height="800" alt="FluxogramaTCC-Fluxo Geral drawio" src="https://github.com/user-attachments/assets/58b7ea5a-55db-445a-be53-2354aeb45499"/> 
</div>
(Protótipo final)

**Autora:** Fernanda Souza Coimbra Machado  
**Orientador:** Prof. Matusalém Martins Lanes

## Funcionalidades
*   **Autenticação:**
    *   **Identificador + Senha:** Via teclado matricial 4x4.
    *   **Biometria:** Leitura de impressão digital (Sensor DY50).
    *   **Reconhecimento Facial:** Integração com módulo externo (TV Box ) via comandos seriais.
*   **Gestão de Usuários:**
    *   Cadastro de novos usuários (com captura de biometria e face).
    *   Exclusão de usuários.
    *   Níveis de acesso: Usuário Comum (apenas acesso) e Usuário Master (acesso + gestão).
*   **Armazenamento e Logs:**
    *   Dados salvos na memória Flash do ESP32 (SPIFFS).
    *   Backup automático e registros de logs de acesso (data/hora) em Cartão SD.
*   **Interface:** Feedback visual via Display OLED e mensagens de status.


## Etapas de Desenvolvimento
*   **Hardware:**
    *   Escolha dos componentes do sistema.
    *  Construção da Placa de Circuito Impresso (PCI)
*   **Software:**
    *   Definição das funcionalidades do sistema.
    *   Contrução dos fluxogramas dos possíveis estados do sistema.
    *   Definição das tecnologias utilizadas.
*   **Testes de bancada**

## Hardware Utilizado
O projeto da Placa de Circuito Impresso (PCI) integra os seguintes componentes:

*   **Microcontrolador:** ESP32-WROOM-32U.
*   **Entrada:**
    *   Teclado Matricial 4x4 com expansor I2C (PCF8574).
    *   Leitor Biométrico DY50 (AS611).
*   **Saída/Atuadores:**
    *   Display OLED 2.42" (Driver SSD1309).
    *   Relé para acionamento da Fechadura Elétrica.
*   **Armazenamento:** Módulo Cartão SD (Interface SPI).
*   **Alimentação:** Fonte 12V.

<div align="center">
<img width="600" height="600" alt="FluxogramaTCC-Arquitetura simplificada- 2" src="https://github.com/user-attachments/assets/3e0b2225-0589-44d4-9a73-dedef441253e" />
</div>

<div align="center">
  <img src="https://github.com/user-attachments/assets/f6e1d331-db75-44ff-af0a-7f7ee63fa641" alt="Placa com Componentes" width="480">
  <img src="https://github.com/user-attachments/assets/2fd791ae-2fe1-49b3-a7ab-e3992b84253b" alt="Placa com Componentes" width="400">
</div>
*(Modelo da PCI desenvolvida no Proteus / PCI finalizada)*.

## Tecnologias e Bibliotecas da placa controladora (PCI).
Desenvolvido em **C++** utilizando o **PlatformIO** (VS Code) e framework Arduino.
Principais bibliotecas:

*   `FS.h` e `SPIFFS.h` (Sistema de arquivos).
*   `Adafruit Fingerprint Sensor Library` (Biometria).
*   `Wire.h` (Comunicação I2C).
*   `Wifi.h`
*   Bibliotecas próprias para modularização: `Biometria.h`, `Display.h`, `Teclado.h`, entre outras.

## Arquitetura do Sistema
O ESP32 atua como mestre, controlando o fluxo de autenticação.

*   **Fluxograma Geral do Sistema**
<div align="center">
  <img width="1515" height="1117" alt="FluxogramaTCC-Fluxo Geral drawio" src="https://github.com/user-attachments/assets/454b1d3b-cdd7-47cc-9548-23502f4f4256" />
</div>
*Fluxograma geral do sistema*

## Estrutura de Arquivos
*   `/src`: Código fonte principal (`main.cpp`).
*   `/lib`: Bibliotecas auxiliares desenvolvidas para o projeto.
*   `/include`: Cabeçalhos.
*   `platformio.ini`: Configurações do ambiente de compilação.

## Testes
<div align="center">
  <img alt="Montagem do sistema para testes em bancada" src="https://github.com/user-attachments/assets/8f14d213-be77-4b41-bdb3-b44234b18e4e" />
</div>

## Resultados
O protótipo atingiu os objetivos propostos, integrando com sucesso três métodos de autenticação: reconhecimento facial, biometria (impressão digital) e senha numérica. A arquitetura de hardware, centrada no ESP32 e na comunicação serial com a TV Box, mostrou-se adequada para gerenciar os periféricos e o armazenamento de dados.
No entanto, foram identificadas limitações importantes a serem melhoradas em trabalhos futuros:

* Reconhecimento Facial: O sistema reconheceu um usuário cadastrado, através de uma foto, e destrancou a porta. O sistema apresentou um tempo elevado para a autenticação por reconhecimento facial (cerca de 12 segundos) devido às limitações de processamento da TV Box.

<div align="center">
  <img width="400" src="https://github.com/user-attachments/assets/72af8f9e-86db-452d-8b4a-b065c4c3f9fa" alt="Aguardando reconhecimento facial"/>
  <img width="400" src="https://github.com/user-attachments/assets/ff011d1f-34b7-4060-909c-260cc4ccc519" alt="Acesso liberado"/>
</div>

* Biometria: Embora mais rápida, demonstrou dificuldade de leitura dependendo do posicionamento ou umidade do dedo
  <div align="center">
  <img width="400" src="https://github.com/user-attachments/assets/347e188c-383d-4d50-891b-5f438269c4c0" alt="Aguardando reconhecimento facial"/>
  <img width="400" src="https://github.com/user-attachments/assets/97dc8628-6ac5-4e75-b270-f8033714b9b9" alt="Acesso liberado"/>
</div>

(Veja o código do Módulo de Reconhecimento Facial [aqui](https://github.com/nandacoimbra/Reconhecimento-Facial)
