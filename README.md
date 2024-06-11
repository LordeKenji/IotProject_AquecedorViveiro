# Viveiro IoT 
<p align="justify">
Código desenvolvido em um microcontrolador ESP32 para o monitoramento e controle de um viveiro de passarinhos de pequeno porte,
o protocolo de comunicação utilizado foi o MQTT, conectado ao broker público HiveMQ, enquanto a dashboard foi desenvolvida
na ferramento de desenvolvimento NodeRed.
</p>

## Motivação
<p align="justify">
Com a chegada do inverno, animais de pequeno porte tais como os passarinhos acabam sofrendo com problemas de saúde tais como
a hipotermia, para superar este problema pode-se utilizar sistemas de aquecimento, desde que possam ser controlados, para que 
acidentes como superaquecimento não aconteçam. A abordagem utilizada neste projeto é de baixo custo, escalonável, segura e 
facilmente replicável, trazendo conforto térmico e outras funcionalidades para o viveiro.
</p>

## Objetivos
<p align="justify">
O objetivo deste projeto inicialmente será na efetivação da conexão do microcontrolador ao broker HiveMQ assim como o fluxo do Node-Red
aos tópicos criados dentro do programa. Após feita essa conexão via WiFi, foca-se em implementar o controle de 2 relés os quais ativarão
uma luz ambiente e um pequeno aquecedor, além destes dois dispositivos, também será monitorado um sensor de porta magnético e um sensor de 
temperatura e umidade (DHT11), os dados enviados a DashBoard também poderão ser visualizados por meio de uma tela LCD 16x2.
A temperatura desejada dentro do viveiro poderá ser alterada pela Dashboad do Node-Red assim como a ativação/desativação do sistema.
Também será confeccionada um viveiro para a inserção do sistema de controle.
</p>

## Arquitetura
<p align="justify">
A arquitetura adotada segue um padrão cliente-servidor, onde o dispositivo ESP32 atua como cliente MQTT e se conecta ao Broker público HiveMQ. Dessa forma 
é possível a comunicação entre o dispositivo e outros dispositivos ou serviços que também estejam conectados ao mesmo broker MQTT, neste caso a Dashboard do 
Node-Red
<p align="justify">
Logo o ESP32, neste caso, torna-se responsável por enviar mensagens (publicar) sobre os dados de temperatura, umidade e estado da porta, além de receber comandos (subscrições) para controlar o aquecedor e a luz. Ele se conecta ao servidor MQTT (broker) através de uma conexão WiFi e utiliza a biblioteca PubSubClient para facilitar a comunicação MQTT.
<p align="justify">
Lembrete: MONTAR ESQUEMA DE LIGAÇÃO
</p>

## Manual de Instalação
<p align="justify">
Informações importantes para a correta instalação e configuração do projeto, vide links para download, componentes necessários e as bibliotecas corretas
para o funcionamento dos sensores, conexão e compatibilidade com a IDE do Arduino
</p>

### Softwares necessários:
- [Node-Red](https://nodered.org/docs/getting-started/windows)
- [Arduino IDE](https://www.arduino.cc/en/software)

### Lista de componetes

| Item                                      |
|-------------------------------------------|
| ESP32 DevKit                              |
| Sensor DHT11                              |
| Módulo LCD 16x2 com interface I2C        |
| 2 relés 5V                                |
| Resistor PullDown (10k)                   |
| Sensor de Porta Magnético                |
| Fonte de Alimentação Protoboard MB102 3.3V 5V |
| Fonte de alimentação 12V-3A ou similar   |
| Jumpers Macho-Macho e Macho-Fêmea        |
| Cabo Micro-USB para upload do código      |


<div align="center">
  <img src="https://raw.githubusercontent.com/LordeKenji/IotProject_AquecedorViveiro/images/ESQUEMALIGACAO.png" alt="Esquema de ligação">
  <p><em>Figura 1: Esta é a legenda da imagem.</em></p>
</div>




### Bibliotecas necessárias:
Para isso acesse o Gerenciador de Bibliotecas pelo menu "Sketch" --> "Incluir Biblioteca" --> "Gerenciar Bibliotecas..." e pesquise
pela biblioteca desejada na lista.
- "ESP32 by Espressif Systems" (ESP32)
- WiFi.h
- PubSubClient.h
- DHT.h
- LiquidCrystal_I2C.h

## Referências
- [How to Use 16x2 i2C LCD Display With ESP32](https://youtu.be/wkbScuJQxFc?si=fXbAD8nzfIJz3pD0)
- [MQTT with ESP32 and DHT22 using Wokwi](https://youtu.be/fbQRF_IKuo4?si=3CgeNY1zY-cKv3Mq)
- [Como usar um Relê 5V com ESP32](https://youtu.be/DnvoEBK24SQ?si=m33_L0pOCrrggAXE)
