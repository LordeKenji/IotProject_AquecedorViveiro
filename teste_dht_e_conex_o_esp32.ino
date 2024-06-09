//Teste sensor DHT e Conexão com o ESP32

//CONFIGURAÇÃO DO DHT:
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);




void setup() {
  //Monitor Serial:
  Serial.begin(9600);
  delay(1500);
  
   //Configurando DHT11
  dht.begin();
  
  //Pino do Rele(LED por enquanto)
  pinMode(2, OUTPUT);
  
  //Sensor Porta
  pinMode(21, INPUT);



}

void loop() {
  leituraSensor();
  int estadPorta = digitalRead(21);
  
  
}

//Função leitura do Sensor:
void leituraSensor() {

  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  //Printar no monitor serial:
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println("");

  Serial.print(F("Humidity: "));
  Serial.print(umid);
  Serial.println("");
  
  delay(1000);
}
