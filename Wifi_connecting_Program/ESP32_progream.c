#include <WiFi.h>

const char* ssid = "DISCR-3";          //wifi name
const char* password = "9822009974";  //wifi password

WiFiServer server(80);
WiFiClient clients[2];

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  WiFiClient newClient = server.available();
  if (newClient) {
    for (int i = 0; i < 2; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        Serial.print("Client ");
        Serial.print(i);
        Serial.println(" connected");
        break;
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    if (clients[i] && clients[i].connected() && clients[i].available()) {

      String data = clients[i].readStringUntil('\n');
      data.trim();

      int id = data.substring(0, 1).toInt();
      String msg = data.substring(2);

      Serial.print("From ESP8266 #");
      Serial.print(id);
      Serial.print(" → ");
      Serial.println(msg);
    }
  }
}
