#include <WiFi.h>
#include <DHTesp.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "esp32_mqtt";
const char *TOPIC_PUBLISH_BPM = "fiap/LifeGame/bpm";
const char *TOPIC_PUBLISH_KM = "fiap/LifeGame/passoskm";
const char *TOPIC_PUBLISH_CALORIAS = "fiap/LifeGame/calorias";
const char *TOPIC_PUBLISH_LED = "fiap/LifeGame/leds";

// WiFi
const char *SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI que deseja se conectar
const char *PASSWORD = "";        // Senha da rede WI-FI que deseja se conectar

// Variáveis globais
DHTesp dht;
TempAndHumidity sensorValues;
WiFiClient espClient;
PubSubClient MQTT(espClient);
unsigned long publishUpdate = 0;
const int TAMANHO = 100;
int ldrValue;
int traveledKm;
int tempValue;
int bpmValue;
int duration;
int calories;


// Configurações de Hardware
#define LDR_PIN 34 // = TraveledKM
#define PIN_GREENLED 5 //TraveledKM LED
const int TEMP_PIN = 27;
// #define TEMP_PIN 27 // = BPM
#define PIN_GREENLED2 18 // = BPM LED
#define ECHO_PIN 32 // = Caloria ECHO
#define TRIGGER_PIN 33 // = Caloria TRIGGER
#define PIN_GREENLED3 19 // = Caloria LED
#define PUBLISH_DELAY 2000

//INIT WIFI
void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

}

//RECONNECT WIFI
void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

// CHECK WIFI AND MQTT 
void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

//INIT MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

//RECONNECT MQTT
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      MQTT.subscribe(TOPIC_PUBLISH_BPM);
      MQTT.subscribe(TOPIC_PUBLISH_KM);
      MQTT.subscribe(TOPIC_PUBLISH_CALORIAS);
      MQTT.subscribe(TOPIC_PUBLISH_LED);
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}


//CALLBACK
void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  
  Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);

  if (strcmp(topic, TOPIC_PUBLISH_LED) == 0) {
    int valor = atoi(msg.c_str());

    if (valor == 1) {
      digitalWrite(PIN_GREENLED, HIGH);
      Serial.println("LED ligado via comando MQTT");
    } else if (valor == 0) {
      digitalWrite(PIN_GREENLED, LOW);
      Serial.println("LED desligado via comando MQTT");
    }
  }
}

void updateDhtValues() {
  sensorValues = dht.getTempAndHumidity();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //LEDS
  pinMode(PIN_GREENLED, OUTPUT);
  digitalWrite(PIN_GREENLED, LOW);
  pinMode(PIN_GREENLED2 ,OUTPUT);
  digitalWrite(PIN_GREENLED2, LOW);
  pinMode(PIN_GREENLED3, OUTPUT);
  digitalWrite(PIN_GREENLED3, LOW);

  //TEMP DHT
  dht.setup(TEMP_PIN, DHTesp::DHT22);

  //UltraSonic
  pinMode(TRIGGER_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an Input

  initWiFi();
  initMQTT();

  Serial.println("Iniciando checagem do corpo...");
}

void loop() {

  updateDhtValues();
  checkWiFIAndMQTT();
  MQTT.loop();

  // 1 sensor -> Traveled Kilometers
  ldrValue = analogRead(LDR_PIN);
  traveledKm = map(ldrValue, 32, 4063, 500, 0);

  // Km LED
  if(traveledKm >= 250){
    digitalWrite(PIN_GREENLED, HIGH);
  }else{
    digitalWrite(PIN_GREENLED, LOW);
  }

  Serial.print("Dist. Percorrida: ");
  Serial.print(traveledKm);
  Serial.println(" Km");

  // 2 sensor -> BPM
  tempValue = sensorValues.temperature;
  bpmValue = map(tempValue, -40, 80, 0, 200);

  // BPM LED
  if(bpmValue < 60 || bpmValue > 100){
    digitalWrite(PIN_GREENLED2, LOW);
  }else{
    digitalWrite(PIN_GREENLED2, HIGH);
  }

  Serial.print("BPM: ");
  Serial.print(bpmValue);
  Serial.println(" bpm");

  // 3 sensor -> Calorie
  // Clear pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets pin on HIGH  for 10 micro sec
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  calories = map(duration, 116, 23528, 0, 3000);

  // Calorie LED
  if(calories > 500){
    digitalWrite(PIN_GREENLED3, HIGH);
  }else{
    digitalWrite(PIN_GREENLED3, LOW);
  }

  Serial.print("Calorias: ");
  Serial.print(calories);
  Serial.println(" kcal");
  Serial.println("----------------------");

  // PRINT ON MQTT
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
  publishUpdate = millis();
  updateDhtValues();

  // KM
  if (!isnan(traveledKm)) {
    StaticJsonDocument<TAMANHO> travel;
    travel["traveledkm"] = traveledKm;

    char buffer[TAMANHO];
    serializeJson(travel, buffer);
    MQTT.publish(TOPIC_PUBLISH_KM, buffer);
    Serial.println(buffer); 
  }

  // BPM
  if (!isnan(sensorValues.temperature)) {
    StaticJsonDocument<TAMANHO> bpm;
    bpm["bpm"] = bpmValue;

    char buffer[TAMANHO];
    serializeJson(bpm, buffer);
    MQTT.publish(TOPIC_PUBLISH_BPM, buffer);
    Serial.println(buffer); 
  }

  // Calorie
  if (!isnan(calories)) {
    StaticJsonDocument<TAMANHO> calorie;
    calorie["calories"] = calories;

    char buffer[TAMANHO];
    serializeJson(calorie, buffer);
    MQTT.publish(TOPIC_PUBLISH_CALORIAS, buffer);
    Serial.println(buffer); 
  }
  
  }
  delay(5000);
}
