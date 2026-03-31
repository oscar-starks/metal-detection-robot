// =============================================
// 4-Wheel Arduino Robot - Movement Sequence
// =============================================
// H-Bridge Pin Mapping:
//   Pin 1 → Front Left  (FL)
//   Pin 2 → Front Right (FR)
//   Pin 3 → Rear Right  (RR)
//   Pin 4 → Rear Left   (RL)
// =============================================

// Motor control pins
const int FL = 1; // Front Left
const int FR = 2; // Front Right
const int RR = 3; // Rear Right
const int RL = 4; // Rear Left

// Duration in milliseconds
const int DURATION = 5000;

void setup() {
  pinMode(FL, OUTPUT);
  pinMode(FR, OUTPUT);
  pinMode(RR, OUTPUT);
  pinMode(RL, OUTPUT);

  stopMotors();
  delay(1000); // Brief pause before starting

  moveForward();
  delay(DURATION);

  stopMotors();
  delay(500);

  moveBackward();
  delay(DURATION);

  stopMotors();
  delay(500);

  turnRight();
  delay(DURATION);

  stopMotors();
  delay(500);

  turnLeft();
  delay(DURATION);

  stopMotors(); // All done
}

void loop() {
  // Nothing here — sequence runs once in setup()
}

// ─── Movement Functions ───────────────────────

void moveForward() {
  // All wheels spin forward
  digitalWrite(FL, HIGH);
  digitalWrite(FR, HIGH);
  digitalWrite(RR, HIGH);
  digitalWrite(RL, HIGH);
}

void moveBackward() {
  // All wheels spin backward
  digitalWrite(FL, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(RR, LOW);
  digitalWrite(RL, LOW);
}

void turnRight() {
  // Left wheels forward, right wheels backward → spins right
  digitalWrite(FL, HIGH);
  digitalWrite(RL, HIGH);
  digitalWrite(FR, LOW);
  digitalWrite(RR, LOW);
}

void turnLeft() {
  // Right wheels forward, left wheels backward → spins left
  digitalWrite(FR, HIGH);
  digitalWrite(RR, HIGH);
  digitalWrite(FL, LOW);
  digitalWrite(RL, LOW);
}

void stopMotors() {
  digitalWrite(FL, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(RR, LOW);
  digitalWrite(RL, LOW);
}