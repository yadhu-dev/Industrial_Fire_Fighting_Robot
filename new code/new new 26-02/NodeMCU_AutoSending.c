#include <ESP8266WiFi.h>

#define MQ2_pin A0
#define Flame_pin 5

const char* ssid = "DISCR-2";
const char* password = "9822009974";

const char* host = "192.168.0.239";  // ESP32 IP
const int port = 80;

WiFiClient client;

int nodeID = 2;
bool fireSent = false;   // prevent spamming

void setup() {
  Serial.begin(115200);

  pinMode(Flame_pin, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");

  connectToServer();
}

void connectToServer() {
  while (!client.connect(host, port)) {
    Serial.println("Connecting to ESP32...");
    delay(1000);
  }
  Serial.println("Connected to ESP32");
}

void loop() {

  int Smoke_value = analogRead(MQ2_pin);
  int Fire_Status = digitalRead(Flame_pin);

  Serial.print("Smoke: ");
  Serial.print(Smoke_value);
  Serial.print("  Flame: ");
  Serial.println(Fire_Status);

  //  Fire condition
  if (Smoke_value > 400 && Fire_Status == LOW && !fireSent) {

    if (!client.connected()) {
      connectToServer();
    }

    String finalMsg = "ID" + String(nodeID) + ":START";

    client.println(finalMsg);

    Serial.print("Sent → ");
    Serial.println(finalMsg);

    fireSent = true;   // send only once
  }

  // Reset when fire gone
  if (Smoke_value < 300 && Fire_Status == HIGH) {
    fireSent = false;
  }

  delay(500);
}
