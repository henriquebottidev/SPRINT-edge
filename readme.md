# Projeto IoT - SPRINT MAHINDRA 

## Visão Geral

Este projeto visa desenvolver uma solução IoT completa, envolvendo a integração de dispositivos IoT, backend para processamento de dados e uma interface de usuário para a visualização das informações e interação com os dispositivos. A solução propõe uma comunicação eficiente entre dispositivos IoT e uma infraestrutura robusta para o gerenciamento de dados.

Capaz de futuramente receber de React a posição, piloto e sua volta na corrida, para que assim posamos melhorar os sistemas e permitir um controle maior da corrida.

## Arquitetura da Solução

A arquitetura proposta envolve três camadas principais: **dispositivos IoT**, **backend** e **frontend**. A comunicação entre as camadas é realizada através de protocolos como MQTT ou HTTP, garantindo a integridade dos dados.

### Componentes

1. **IoT Devices**:
   - Dispositivos sensores e atuadores (por exemplo: sensores de temperatura, umidade, presença, e atuadores como relés ou motores).
   - Conexão com a internet via Wi-Fi ou protocolo apropriado (MQTT, HTTP, etc.).

2. **Backend**:
   - API para coletar e processar os dados dos dispositivos IoT.
   - Comunicação com os dispositivos IoT via MQTT ou HTTP.
   - Armazenamento de dados em banco de dados, conforme a necessidade (SQL ou NoSQL).

3. **Frontend**:
   - Interface web ou aplicação móvel para visualização dos dados coletados e controle dos dispositivos.
   - Dashboard com visualização em tempo real dos dados dos sensores e controle dos atuadores.

### Diagrama de Arquitetura

O diagrama da arquitetura a seguir ilustra a interação entre os dispositivos IoT, o backend e o frontend:

![Diagrama da Arquitetura](https://drive.google.com/file/d/1x6EWihbr-Syn5gd97AYBpSC94VDAW3eC/view?usp=sharing)

![Screenshot 2024-09-30 at 18 07 33](https://github.com/user-attachments/assets/78520030-e0b4-4928-a5ce-19bb288f5563)




## Recursos Necessários

### Dispositivos IoT
- **Microcontrolador**: ESP32
- **Sensores**: Sensores apropriados para a sua aplicação, como temperatura, umidade, presença, etc.
- **Atuadores**: Relés, motores ou outros dispositivos de controle.
- **Protocolo de Comunicação**: MQTT (leve e eficiente) ou HTTP (para maior compatibilidade com APIs).

### Backend
- **Linguagem de Programação**: Python e C++
- **Banco de Dados**: MongoDB
- **Broker MQTT**: Mosquitto, AWS IoT Core ou qualquer outro broker de sua escolha para gerenciar a comunicação dos dispositivos.

### Frontend
- **Interface de usuário**: Dashboard com gráficos em tempo real e Administradores que enviam eventos pela rota através de React.

## Instruções de Uso

### 1. Clonando o Repositório

```bash
git clone https://github.com/henriquebottidev/SPRINT-edge
cd SPRINT-edge

