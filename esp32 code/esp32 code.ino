#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


//WIFI SSID
const char* ssid = "Pink Unicorn";
const char* password = "abcd1234";

//MQTT SERVER IP ADDRESS
const char* mqtt_server = "104.248.97.232";

WiFiClient espClient;
PubSubClient client(espClient);

int relay_pin = 4;
int status_pin = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode (relay_pin, OUTPUT);
  pinMode (status_pin, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    //200ms blink when connecting to wifi
    digitalWrite(status_pin, HIGH);
    delay(200);
    digitalWrite(status_pin, LOW);
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //Blink once when conncted to wifi
  digitalWrite(status_pin, HIGH);
  delay(2000);
  digitalWrite(status_pin, LOW);
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "switch1/status") {
    //Serial.print("Changing output to ");
    Serial.println("switch1 triggred");
    messageTemp.trim();
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relay_pin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relay_pin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
        digitalWrite(status_pin, HIGH);
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      for(int i = 0 ; i<5; i++){
        digitalWrite(status_pin, HIGH);
        delay(100);
        digitalWrite(status_pin, LOW);
        delay(100);
      };
     
      // Subscribe
      client.subscribe("switch1/status");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      digitalWrite(status_pin, HIGH);
      delay(1000);
      digitalWrite(status_pin, LOW);
      delay(1000);
      // Wait 5 seconds before retrying
      //delay(5000);
      esp_restart();
    }

    digitalWrite(status_pin, LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    if(WiFi.status() != WL_CONNECTED){
    setup_wifi();
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
