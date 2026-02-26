// Motor driver pins
#define IN1 23
#define IN2 22
#define IN3 21
#define IN4 19
#define ENA 18
#define ENB 5

// IR Sensors
#define IR1 35
#define IR2 4

int speed = 180;

bool line_follow = true;   

// ================= STATE MACHINE =================
enum RobotState {
  LINE_FOLLOW_TO_A,
  STRAIGHT_A_TO_B,
  LINE_FOLLOW_TO_STOP,
  STOPPED
};

RobotState state = LINE_FOLLOW_TO_A;

bool leftA = false;
bool leftB = false;
// =================================================

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  Serial.begin(115200);
}

// ================= MOTOR FUNCTIONS =================
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

// ================= LINE FOLLOW =================
void lineFollow(int IR_value1, int IR_value2) {
  if (IR_value1 == 0 && IR_value2 == 1) {
    left();
  }
  else if (IR_value1 == 1 && IR_value2 == 0) {
    right();
  }
  else if(IR_value1 == 1 && IR_value2 == 1) {
    forward();
  }
  else{
    stopMotor();
  }
}

// ================= LOOP =================
void loop() {
  int IR_value1 = digitalRead(IR1);
  int IR_value2 = digitalRead(IR2);

  Serial.print("IR1 = ");
  Serial.print(IR_value1);
  Serial.print("  IR2 = ");
  Serial.print(IR_value2);
  Serial.print("  STATE = ");
  Serial.print(state);
  Serial.print("  LF = ");
  Serial.println(line_follow);

  // ================= STATE 1 =================
  if (state == LINE_FOLLOW_TO_A) {
    line_follow = true;

    if (line_follow) {
      lineFollow(IR_value1, IR_value2);
    }

    if (IR_value1 == 0 && IR_value2 == 0) {
      Serial.println("A DETECTED");
      state = STRAIGHT_A_TO_B;
      leftA = false;
    }
  }

  // ================= STATE 2 =================
  else if (state == STRAIGHT_A_TO_B) {
    line_follow = false;

    if (!line_follow) {
      forward();   // ignore IR
    }

    if (!leftA && (IR_value1 != 0 || IR_value2 != 0)) {
      leftA = true;
    }

    if (leftA && IR_value1 == 0 && IR_value2 == 0) {
      Serial.println("B DETECTED");
      state = LINE_FOLLOW_TO_STOP;
      leftB = false;
    }
  }

  // ================= STATE 3 =================
  else if (state == LINE_FOLLOW_TO_STOP) {
    line_follow = true;

    if (line_follow) {
      lineFollow(IR_value1, IR_value2);
    }

    if (!leftB && (IR_value1 != 0 || IR_value2 != 0)) {
      leftB = true;
    }

    if (leftB && IR_value1 == 0 && IR_value2 == 0) {
      Serial.println("FINAL NODE → STOP");
      stopMotor();
      state = STOPPED;
    }
  }

  // ================= STATE 4 =================
  else if (state == STOPPED) {
    line_follow = false;
    stopMotor();
  }
}
