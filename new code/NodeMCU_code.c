#include <ESP8266WiFi.h>

const char* ssid = "DISCR-2";
const char* password = "9822009974";

const char* host = "192.168.0.239";  // ESP32 IP
const int port = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");

  connectToServer();

  Serial.println("Type START or STOP and press ENTER:");
}

void connectToServer() {
  while (!client.connect(host, port)) {
    Serial.println("Connecting to ESP32...");
    delay(1000);
  }
  Serial.println("Connected to ESP32");
}

void loop() {

  if (!client.connected()) {
    connectToServer();
  }

  if (Serial.available()) {

    String userMsg = Serial.readStringUntil('\n');
    userMsg.trim();

    if (userMsg.length() > 0) {

      client.println(userMsg);  // send only START or STOP

      Serial.print("Sent → ");
      Serial.println(userMsg);
    }
  }
}
