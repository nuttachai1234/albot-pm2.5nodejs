//Prototype MQTT ESP32 Programming
//Write by Nuttachai D. Subangate
//Credit Software Engineering NPRU. 2019

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "vivo 1718";
const char* password = "1234567890";
const char* mqtt_server = "iot.eclipse.org";

int relayPin = 23;
int relayPin2 = 22;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
 // Serial.println();
 // Serial.print("Connecting to ");
 // Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   // Serial.print(".");
  }

  randomSeed(micros());

  //Serial.println("");
 // Serial.println("WiFi connected");
 // Serial.println("IP address: ");
 // Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '1') {
    digitalWrite(relayPin, HIGH);  
  } 
  if ((char)payload[0] == '0') {
    digitalWrite(relayPin, LOW);  
  } 
   if ((char)payload[0] == '2') {
    digitalWrite(relayPin2, HIGH);  
  }
    if ((char)payload[0] == '3') {
    digitalWrite(relayPin2, LOW);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("aalbot-npru/", "hello world");
      // ... and resubscribe
      client.subscribe("aalbot-npru/");
    } else {
      //Serial.print("failed, rc=");
      ///Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "#%ld", value);
    //Serial.print("Publish message: ");
   // Serial.println(msg);
    client.publish("aalbot-npru/", msg);
  }
  digitalWrite(LED_BUILTIN, LOW);
  
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
  Serial.println(F("Failed to read from DHT sensor!"));
  return;
}
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
    if (t >= "30") {
    digitalWrite(relayPin2, HIGH);  
  }
    if (t <= "30") {
    digitalWrite(relayPin2, LOW);
  }
}
