/*
 * MQTT and AskSensors IoT Platform
 * Description: ESP32 publishes data to AskSensors using MQTT
 *  Author: https://asksensors.com, 2019
 *  github: https://github.com/asksensors
 */
 
#include <WiFi.h>
#include <PubSubClient.h>

// AskSensors MQTT user config

const char* ssid = ".................."; // Wifi SSID
const char* password = ".................."; // Wifi Password
const char* pubTopic = "publish/.................."; // API KEY IN
const char* mqtt_server = "asksensors.com";
unsigned int mqtt_port = 1883;

WiFiClient askClient;
PubSubClient client(askClient);

void setup() {
  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : ESP32 publishes data to AskSensors over MQTT");
  Serial.print("********** connecting to WIFI : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("->WiFi connected");
  Serial.println("->IP address: ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) 
    reconnect();
  client.loop();
  Serial.println("********** Publish MQTT data to ASKSENSORS");
  char mqtt_payload[30] = "";
  snprintf (mqtt_payload, 30, "module1=%ld", random(10,100));
  Serial.print("Publish message: ");
  Serial.println(mqtt_payload);
  client.publish(pubTopic, mqtt_payload);
  Serial.println("> MQTT data published");
  Serial.println("********** End ");
  Serial.println("*****************************************************");
  
 delay(25000);// delay
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("********** Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("-> MQTT client connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("-> try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
