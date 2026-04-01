#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);

// Motor control pins
const int FL = 5;
const int FR = 6;
const int RR = 9;
const int RL = 10;
const int EN = 11;

// Metal detector pins
const int BUZZER = 4;
const int SENSOR_1 = A1;
const int SENSOR_2 = A2;
const int SENSOR_3 = A3;

// Adjust this threshold based on your sensor's output when metal is near
const int METAL_THRESHOLD = 0;

const int motorDelay = 500;
String inputString = "";
bool metalDetected = false;

void setup() {
  pinMode(FL, OUTPUT);
  pinMode(FR, OUTPUT);
  pinMode(RR, OUTPUT);
  pinMode(RL, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);

  stopMotors();
  digitalWrite(BUZZER, LOW);

  Serial.begin(9600);
  BT.begin(9600);

  Serial.println("Bluetooth Robot Ready.");
  Serial.println("Commands: F/front, B/back, L/left, R/right, S/stop");
}

void loop() {
  checkMetalSensors();

  while (BT.available()) {
    char c = BT.read();

    if (c == '\n' || c == '\r') {
      inputString.trim();
      inputString.toLowerCase();

      if (inputString.length() > 0) {
        Serial.print("Received: ");
        Serial.println(inputString);
        handleCommand(inputString);
      }

      inputString = "";
    } else {
      inputString += c;
    }
  }
}

// ─── Command Handler ──────────────────────────────────────

void handleCommand(String cmd) {
  if (cmd == "f" || cmd == "front") {
    Serial.println(">> FORWARD");
    moveForward();
    delay(motorDelay);
    stopMotors();

  } else if (cmd == "b" || cmd == "back") {
    Serial.println(">> BACKWARD");
    moveBackward();
    delay(motorDelay);
    stopMotors();

  } else if (cmd == "l" || cmd == "left") {
    Serial.println(">> TURN LEFT");
    turnLeft();
    delay(motorDelay);
    stopMotors();

  } else if (cmd == "r" || cmd == "right") {
    Serial.println(">> TURN RIGHT");
    turnRight();
    delay(motorDelay);
    stopMotors();

  } else if (cmd == "s" || cmd == "stop") {
    Serial.println(">> STOP");
    stopMotors();

  } else {
    Serial.print("Unknown command: ");
    Serial.println(cmd);
  }
}

// ─── Metal Detection ─────────────────────────────────────

void checkMetalSensors() {
  int s1 = analogRead(SENSOR_1);
  int s2 = analogRead(SENSOR_2);
  int s3 = analogRead(SENSOR_3);

  Serial.println("Values that are printed");
  Serial.println(s1);
  Serial.println(s2);
  Serial.println(s3);
  delay(500);


  bool detected = (s1 == METAL_THRESHOLD) || (s2 == METAL_THRESHOLD) || (s3 == METAL_THRESHOLD);

  if (detected && !metalDetected) {
    metalDetected = true;
    digitalWrite(BUZZER, HIGH);
    Serial.println("!! METAL DETECTED !!");
  } else if (!detected && metalDetected) {
    metalDetected = false;
    digitalWrite(BUZZER, LOW);
    Serial.println("-- Metal out of range");
  }
}

// ─── Movement Functions ───────────────────────────────────

void turnRight() {
  // this is Right
  analogWrite(EN, 60);      
  digitalWrite(FL, HIGH);
  digitalWrite(FR, LOW);
  digitalWrite(RL, LOW);
  digitalWrite(RR, LOW);
}

void turnLeft() {
  // THIS IS GOING LEFT
  analogWrite(EN, 60);   
  digitalWrite(FL, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(RL, HIGH);
  digitalWrite(RR, LOW);
}

void moveBackward() {
  // this is reverse
  analogWrite(EN, 60);
  digitalWrite(FR, HIGH);   
  digitalWrite(RR, HIGH);
  digitalWrite(FL, LOW);   
  digitalWrite(RL, LOW);
}

void moveForward() {
  analogWrite(EN, 60);
  digitalWrite(FL, HIGH);   
  digitalWrite(RL, HIGH);
  digitalWrite(FR, LOW);   
  digitalWrite(RR, LOW);
}

void stopMotors() {
  analogWrite(EN, 0);     
  digitalWrite(FL, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(RL, LOW);
  digitalWrite(RR, LOW);
}