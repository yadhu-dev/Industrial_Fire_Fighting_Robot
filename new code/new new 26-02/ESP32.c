#include <WiFi.h>

// -------- WiFi --------
const char* ssid = "DISCR-2";
const char* password = "9822009974";

WiFiServer server(80);
WiFiClient clients[2];

// -------- Motor driver pins --------
#define IN1 23
#define IN2 22
#define IN3 21
#define IN4 19
#define ENA 18
#define ENB 5

// -------- IR Sensors --------
#define IR1 35
#define IR2 4

int speedMotor = 250;

// -------- Control flags --------
bool lineFollowerActive = false;
int activeMap = 0;  // 0 = none, 1 = map1, 2 = map2
int n = 0;          // node counter

// -------- Setup --------
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// -------- Motor Functions --------
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedMotor);
  analogWrite(ENB, speedMotor);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, speedMotor);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedMotor);
  analogWrite(ENB, 0);
}

void stopMotor() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// -------- MAP 1 --------
void map1() {
  if (!lineFollowerActive) return;

  int IR_value1 = digitalRead(IR1);
  int IR_value2 = digitalRead(IR2);

  if (IR_value1 == 0 && IR_value2 == 0) {
    if (n < 1) {
      forward();
      delay(400);
      n++;
    } else {
      stopMotor();
      lineFollowerActive = false;
      activeMap = 0;
      Serial.println("MAP1 completed");
    }
  } else if (IR_value1 == 0 && IR_value2 == 1) left();
  else if (IR_value1 == 1 && IR_value2 == 0) right();
  else forward();
}

// -------- MAP 2 --------
void map2() {
  if (!lineFollowerActive) return;

  int IR_value1 = digitalRead(IR1);
  int IR_value2 = digitalRead(IR2);

  Serial.print("IR1 = ");
  Serial.print(IR_value1);
  Serial.print("  IR2 = ");
  Serial.println(IR_value2);

  if (IR_value1 == 0 && IR_value2 == 0) {
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    if (n < 1) {
      forward();
      delay(2500);
      n = n + 1;

    } else {
      stopMotor();
    }
  } else if (IR_value1 == 0 && IR_value2 == 1) {
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    left();
  } else if (IR_value1 == 1 && IR_value2 == 0) {
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    right();
  } else {
    forward();
  }
}

void loop() {

  // ===== Check for new clients =====
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

  // ===== Read data from NodeMCU =====
  for (int i = 0; i < 2; i++) {
    if (clients[i] && clients[i].connected() && clients[i].available()) {

      String data = clients[i].readStringUntil('\n');
      data.trim();

      Serial.print("Received: ");
      Serial.println(data);

      int colonIndex = data.indexOf(':');

      if (colonIndex > 0) {
        String nodeID = data.substring(0, colonIndex);
        String command = data.substring(colonIndex + 1);

        if (command == "STOP") {
          lineFollowerActive = false;
          activeMap = 0;
          stopMotor();
          Serial.println("STOP received → Robot stopped");
        }

        else if (command == "START") {
          lineFollowerActive = true;
          n = 0;

          if (nodeID == "ID1") {
            activeMap = 1;
            Serial.println("Running MAP 1");
          } else if (nodeID == "ID2") {
            activeMap = 2;
            Serial.println("Running MAP 2");
          }
        }
      }
    }
  }

  // ===== Run selected MAP =====
  if (lineFollowerActive) {
    if (activeMap == 1) map1();
    else if (activeMap == 2) map2();
    else stopMotor();
  } else {
    stopMotor();
  }
}
