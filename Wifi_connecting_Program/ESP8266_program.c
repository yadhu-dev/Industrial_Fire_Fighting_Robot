#include <ESP8266WiFi.h>

const char* ssid = "DISCR-3";
const char* password = "9822009974";

const char* host = "----------";  // ESP32 IP
const int port = 80;

WiFiClient client;

int deviceID = 1;   

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

  Serial.println("Type message and press ENTER:");
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

  // Check if user typed something in Serial Monitor
  if (Serial.available()) {

    String userMsg = Serial.readStringUntil('\n');
    userMsg.trim();

    if (userMsg.length() > 0) {

      String fullMsg = String(deviceID) + ":" + userMsg;

      client.println(fullMsg);

      Serial.print("Sent → ");
      Serial.println(fullMsg);
    }
  }
}
