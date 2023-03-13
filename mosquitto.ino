// Librairies
#include <WiFi.h>
#include <Wire.h>
#include <MQTT.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>

// Define
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DI0 26
#define freq 870E6
#define sf 9
#define sb 33E3

// connexion point d'accès + serveur mqqt
char ssid[] = "FraiseTagada";
char pass[] = "tmop4262";
const char* mqtt_server = "test.mosquitto.org";//Adresse IP du Broker Mqtt
const int mqttPort = 1883; //port utilisé par le Broker

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//packets LoRa sous forme de chaine d'octets
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
  //client.publish("srt/victor", "Bonjour c'est Victor");

// Initialisation des connexions avec le modem LoRa et les paramètres radio
  pinMode(DI0, INPUT);
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(sb);
}

float d1=12, d2=321.54;

void loop()
{
  // Boucle qui affiche des points si pas de connexion avec le point d'accès
  while(WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  reconnect();
  client.loop(); 

  // Pulication des paramètres LoRa au travers de MQTT
  float tps = 0;
  if (millis()-tps>5000){
    tps=millis();
    client.publish("srt/victor", "870000000, 9, 33000");
  }

//partie données LoRa
  Serial.println("New Packet");
  LoRa.beginPacket();
  sdp.data[0]=d1;
  sdp.data[1]=d2;
  LoRa.write(sdp.frame,16);
  LoRa.endPacket();
  d1++; d2+=2;
  delay(2000);    
}

// Mise en place du serveur mqtt
void setup_mqtt(){
  client.setServer(mqtt_server, mqttPort);
  reconnect();
}

// Connexion au serveur mqtt
// Si on a pas réussi, on réessaie toutes les 2 secondes
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