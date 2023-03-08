#include <WiFi.h>
#include <Wire.h>
#include <MQTT.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
#define freq 868E6
#define sf 8
#define sb 125E3

char ssid[] = "FraiseTagada";
char pass[] = "tmop4262";
const char* mqtt_server = "test.mosquitto.org";//Adresse IP du Broker Mqtt
const int mqttPort = 1883; //port utilisé par le Broker 
IPAddress ip;
long tps=0;

WiFiClient espClient;
PubSubClient client(espClient);

union pack
{
  uint8_t frame[16];
  float data[4];
} sdp;

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(ssid, pass);
  delay(1000);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500); Serial.print(".");
    }
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    Serial.println("WiFi setup ok");
    delay(1000);
  setup_mqtt();
  client.publish("srt/victor", "Bonjour c'est Victor");
  pinMode(DIO, INPUT);
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(sb);
}

void loop()
{
  while(WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  reconnect();
  client.loop(); 
  //On utilise pas un delay pour ne pas bloquer la réception de messages 
  if (millis()-tps>5000){
    tps=millis();
    float temp = random(30);
    mqtt_publish("srt/victor",temp);
    Serial.print("victor : ");
    Serial.println(temp); 
  }
}

void setup_mqtt(){
  client.setServer(mqtt_server, mqttPort);
  reconnect();
}

void reconnect(){
  while (!client.connected()) {
    Serial.println("Connection au serveur MQTT ...");
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT connecté");
    }
    else {
      Serial.print("echec, code erreur= ");
      Serial.println(client.state());
      Serial.println("nouvel essai dans 2s");
    delay(2000);
    }
  }
}
//Fonction pour publier un float sur un topic 
void mqtt_publish(String topic, float t){
  char top[topic.length()+1];
  topic.toCharArray(top,topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  client.publish(top,t_char);
}