//Autor: Rafael Kenji Issaka 201270072 - UNESP Sorocaba
//Dedicado a: Projeto IoT - Prof. Dhiego Fernandes Carvalho


//Relacionados a conexão com a Internet:
#include <WiFi.h>
const char* ssid = "******************";
const char* password = "******************";
const char* mqtt_server = "********************";

//Relacionado com a conexão MQTT:
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

//Tela LCD
#include <LiquidCrystal_I2C.h>
#define I2C_ADDR    0x27  // Endereço I2C do módulo LCD
#define LCD_COLS    16    // Número de colunas do LCD
#define LCD_ROWS    2     // Número de linhas do LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);  // Inicializa o LCD I2C

//Configurações sensor DHT11:
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);

//Configurações das GPIos utilizadas:
#define PIN_AQUECEDOR 2
#define PIN_LUZ 12
#define PIN_SENSOR 5

//Declarações das varíaveis utilizadas:
float temperature = 0.0;
float humi = 0.0;
float controle = 23.0;
int porta = 0;
int ativa_controle = 0;
int evento = 0;

//Função para conectar ao WiFi:
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi conectada!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Função da Biblioteca PubSubClient de suma importância:
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String mensagem;
  for (unsigned int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.println(mensagem);
  Serial.println();

  //Filtro de comandos baseado no tópico do payload:

  //Valor desejado de temperatura:
  if (String(topic) == "********/controle") {
    controle = mensagem.toFloat();
    Serial.print("Valor convertido para float: ");
    Serial.println(controle);
  }

  //Ativa ou desativa a função controla de temperatura:
  if (String(topic) == "********/ativa_controle") {
    ativa_controle = mensagem.toInt();
    Serial.print("Valor convertido para int: ");
    Serial.println(ativa_controle);
  }

  //Ativa ou desativa luz do viveiro:
  if (String(topic) == "********/lampada") {
    int estado = mensagem.toInt();
    if (estado == 1) {
      digitalWrite(PIN_LUZ, HIGH);
    }
    else {
      digitalWrite(PIN_LUZ, LOW);
    }
  }
}

//Em caso de perda de conexão:
void reconnect() {

  //Inseri a busca por WiFi primeiramente pois o broker possui menos chance de cair:
  setup_wifi();

  //Logo em seguida caso a conexão com a WiFi seja possível ele conecta ao Broker:
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "********";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");

      //Tópicos para poder monitorar via Monitor Serial:
      client.subscribe("********/lampada");
      client.subscribe("********/controle");
      client.subscribe("********/ativa_controle");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  //Inicia primeiramente o monitor serial para sabermos o que esta acontecendo:
  Serial.begin(115200);

  //Configurações das GPIOs
  pinMode(PIN_AQUECEDOR, OUTPUT);
  pinMode(PIN_SENSOR, INPUT_PULLUP);
  pinMode(PIN_LUZ, OUTPUT);

  //Inicia o sensor de temperatura e umidade:
  dht.begin();

  //Inicar conexão:
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //Valores iniciais preferênciais:
  evento = 0;

  //Inicia a tela LCD:
  lcd.init();     // Inicializa o LCD I2C
  lcd.backlight(); // Liga o backlight do LCD
  lcd.clear();    // Limpa o LCD
  //Formatação da tela:
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Umidade: ");

}

void loop() {

  //Primeiro deve-se estabelecer a conexão:
  if (!client.connected()) {
    reconnect();
  }
  //Inicial o callback:
  client.loop();


  //Para não sobrecarregar o sistema, intervalo de 1,5 segundos para cada leitura e envio:
  unsigned long now = millis();
  if (now - lastMsg > 1500) {
    //Leitura da porta e do sensor:
    porta = digitalRead(5);
    temperature = dht.readTemperature();
    humi = dht.readHumidity();

    // Exibe os valores no LCD
    lcd.setCursor(6, 0);
    lcd.print(temperature);
    lcd.print(" C");
    lcd.setCursor(9, 1);
    lcd.print(humi);
    lcd.print(" %");


    //Publica os valores de temperatura pelo seguinte topico:
    String temp = String(dht.readTemperature());
    Serial.print("Temperature: ");
    Serial.println(temp);
    client.publish("********/temperature", temp.c_str());
    //Publica os valores de umidade pelo seguinte topico:
    String hum = String(dht.readHumidity());
    Serial.print("Humidity: ");
    Serial.println(hum);
    client.publish("********/humidity", hum.c_str());

    //Checa estado da porta:
    if (porta == 1) {
      //Publica estado da porta:
      Serial.println("Sensor de Porta Fechado");
      snprintf(msg, MSG_BUFFER_SIZE, "Porta Fechada");
      client.publish("********/porta", msg);

      //Logica para não realizar esse laço mais de uma vez:
      if (evento == 1) {
        digitalWrite(PIN_LUZ, LOW);
        evento = 0;
      }
    }
    if (porta == 0) {
      //Publica estado da porta:
      Serial.println("Sensor de Porta Aberta");
      snprintf(msg, MSG_BUFFER_SIZE, "Porta Aberta");
      client.publish("********/porta", msg);

      if (evento == 0) {
        digitalWrite(PIN_LUZ, HIGH);
        evento = 1;
      }
    }

    //Condições para ativar o sistema de controle de temperatura:
    if (ativa_controle == 1) {

      //Se a temperatura estiver menor que a desejada deve-se ligar a lâmpada:
      if (temperature < controle) {
        digitalWrite(PIN_AQUECEDOR, HIGH);
        //Publica estado do sistema de aquecimento:
        snprintf(msg, MSG_BUFFER_SIZE, "Aquecedor ligado");
        client.publish("********/aquecedor", msg);
      }

      //Eleva-se a temperatura um grau acima do desejado para não disparar com tanta frequência o sistema:
      if (temperature >= (controle + 1) ) {
        digitalWrite(PIN_AQUECEDOR, LOW);
        //Publica status do aquecedor:
        snprintf(msg, MSG_BUFFER_SIZE, "Aquecedor desligado");
        client.publish("********/aquecedor", msg);
      }

    }
    //Se o sistema for desativado a aquecedor não deve ser ligado:
    if (ativa_controle == 0) {
      digitalWrite(PIN_AQUECEDOR, LOW);
      //Publica o status do aquecedor para a dashboard:
      snprintf(msg, MSG_BUFFER_SIZE, "Sistema Desativado");
      client.publish("********/aquecedor", msg);
    }

    //Importante, sempre atualizar o valor da última mensagem:
    lastMsg = now;
  }
}
