#include <WiFi.h>

//Ultrasonic sensor
#define Trig 27
#define Echo 14

// Motor driver pins
#define IN1 23
#define IN2 22
#define IN3 21
#define IN4 19
#define ENA 18
#define ENB 5

// IR Sensors
#define IR1 2
#define IR2 4

int speed = 180;

const char* ssid = "DISCR-3";         //wifi name
const char* password = "9822009974";  //wifi password

WiFiServer server(80);
WiFiClient clients[2];

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, speed);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speed);
  analogWrite(ENB, 0);
}

void stopMotor() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void Check_Obst() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if (distance <= 10) {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }
}


void rootMap1() {
  int n = 0;
  int IR_value1 = digitalRead(IR1);
  int IR_value2 = digitalRead(IR2);

  Serial.print("IR1 = ");
  Serial.print(IR_value1);
  Serial.print("  IR2 = ");
  Serial.println(IR_value2);

  Check_Obst();
  if (IR_value1 == 1 && IR_value2 == 1) {
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    forward();
  } else if (IR_value1 == 1 && IR_value2 == 0) {
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    left();
  } else if (IR_value1 == 0 && IR_value2 == 1) {
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    right();
  } else {
    stopMotor();
    if (n == 0) {
      while (IR_value1 == 0 && IR_value2 == 0) {
        //to left 45 deg
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);

        analogWrite(ENA, 0);
        analogWrite(ENB, speed);
      }
    }
    if (IR_value1 == 1 && IR_value2 == 1) {
      Serial.print("IR 1 = ");
      Serial.println(IR_value1);
      Serial.print("IR 2 = ");
      Serial.println(IR_value2);
      forward();
    } else if (IR_value1 == 1 && IR_value2 == 0) {
      Serial.print("IR 1 = ");
      Serial.println(IR_value1);
      Serial.print("IR 2 = ");
      Serial.println(IR_value2);
      left();
    } else if (IR_value1 == 0 && IR_value2 == 1) {
      Serial.print("IR 1 = ");
      Serial.println(IR_value1);
      Serial.print("IR 2 = ");
      Serial.println(IR_value2);
      right();
    } else {
      Serial.print("IR 1 = ");
      Serial.println(IR_value1);
      Serial.print("IR 2 = ");
      Serial.println(IR_value2);
      stopMotor();
    }
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    stopMotor();
  }
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

      if (id == 1) {

      } else {
      }
    }
  }
}
