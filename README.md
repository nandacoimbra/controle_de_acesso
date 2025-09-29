# Controle de Acesso por Reconhecimento Facial

Este projeto é um sistema de **controle de acesso a ambientes** utilizando **reconhecimento facial**, com suporte a autenticação via **teclado (login e senha)** e **biometria (digital)**.

## Descrição do Sistema

O sistema é composto por:

- **ESP32 WROOM-32U**: controla periféricos e aciona a fechadura via relé.  
- **Teclado matricial**: permite login via id e senha.  
- **Display OLED**: exibe mensagens e status do sistema.  
- **Leitor biométrico**: captura digitais para autenticação adicional.  
- **Módulo SD Card**: armazena logs de usuários e registros de acesso.  
- **Relé**: aciona a fechadura elétrica do ambiente.  
- **TV Box (descaracterizada) com Linux**: processa o reconhecimento facial.  
- **Webcam USB**: captura imagens do usuário para autenticação facial.

O ESP32 se comunica via **serial** com a "TV Box", que roda uma aplicação Python utilizando a biblioteca [`face_recognition`](https://github.com/ageitgey/face_recognition) para identificar usuários autorizados.

## Funcionalidades

- Autenticação por **reconhecimento facial** (principal).  
- Autenticação por **biometria** ou **senha**, como métodos alternativos.  
- **Cadastro de novos usuários**:  
  - Gerar ID, registrar nome e senha.  
  - Capturar biometria e foto para gerar o encoding facial.  
- **Remoção de usuários** pelo administrador (via ID).  
- Controle diferenciado: **usuário comum** abre porta; **administrador** acessa menu de gerenciamento.  

O sistema ainda está em desenvolvimento.

## Tecnologias Utilizadas

- **Hardware**: ESP32 WROOM-32U, teclado matricial, display OLED, leitor biométrico, módulo SD Card, relé, fechadura elétrica, webcam.  
- **Software**: Python, `face_recognition`, C++ para ESP32, comunicação serial.  
- **Sistema Operacional**: Armbian Linux na TV Box.  

## Estrutura do Repositório

---

**Autor:** Fernanda Coimbra  
**Curso:** Engenharia de Controle e Automação – CEFET-MG
