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

void loop() {
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
    Serial.print("IR 1 = ");
    Serial.println(IR_value1);
    Serial.print("IR 2 = ");
    Serial.println(IR_value2);
    stopMotor();
  }
}
