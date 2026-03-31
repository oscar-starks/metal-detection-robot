#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);  // RX=2, TX=3

// Motor control pins
const int FL = 5;  // Front Left
const int FR = 6;  // Front Right
const int RR = 7;  // Rear Right
const int RL = 8;  // Rear Left

const int motorDelay = 200;

String inputString = "";  // Buffer for incoming BT string

void setup() {
  pinMode(FL, OUTPUT);
  pinMode(FR, OUTPUT);
  pinMode(RR, OUTPUT);
  pinMode(RL, OUTPUT);

  stopMotors();

  Serial.begin(9600);    // USB Serial Monitor
  BT.begin(9600);        // Bluetooth module baud rate

  Serial.println("Bluetooth Robot Ready.");
  Serial.println("Commands: F/front, B/back, L/left, R/right, S/stop");
}

void loop() {
  // Read incoming Bluetooth data character by character
  while (BT.available()) {
    char c = BT.read();

    if (c == '\n' || c == '\r') {
      // Full command received — process it
      inputString.trim();
      inputString.toLowerCase();

      if (inputString.length() > 0) {
        Serial.print("Received: ");
        Serial.println(inputString);
        handleCommand(inputString);
      }

      inputString = "";  // Clear buffer for next command
    } else {
      inputString += c;  // Keep building the string
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

// ─── Movement Functions ───────────────────────────────────

void moveForward() {
  digitalWrite(FL, HIGH);
  digitalWrite(FR, HIGH);
  digitalWrite(RR, HIGH);
  digitalWrite(RL, HIGH);
}

void moveBackward() {
  digitalWrite(FL, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(RR, LOW);
  digitalWrite(RL, LOW);
}

void turnLeft() {
  // Right side fast (outer), left side stops (inner)
  digitalWrite(FR, HIGH);
  digitalWrite(RR, HIGH);
  digitalWrite(FL, LOW);
  digitalWrite(RL, LOW);
}

void turnRight() {
  // Left side fast (outer), right side stops (inner)
  digitalWrite(FL, HIGH);
  digitalWrite(RL, HIGH);
  digitalWrite(FR, LOW);
  digitalWrite(RR, LOW);
}

void stopMotors() {
  digitalWrite(FL, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(RR, LOW);
  digitalWrite(RL, LOW);
}