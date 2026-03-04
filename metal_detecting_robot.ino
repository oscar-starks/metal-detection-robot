#include <SoftwareSerial.h>

// Bluetooth module pins (HC-05/HC-06)
#define BT_RX 10  // Connect to TX of Bluetooth module
#define BT_TX 11  // Connect to RX of Bluetooth module

// Buzzer pin
#define BUZZER_PIN 12

// Metal sensor pins - Sides
#define SENSOR_FRONT  2
#define SENSOR_BACK   3
#define SENSOR_LEFT   4
#define SENSOR_RIGHT  5

// Metal sensor pins - Corners
#define SENSOR_FRONT_LEFT   6
#define SENSOR_FRONT_RIGHT  7
#define SENSOR_BACK_LEFT    8
#define SENSOR_BACK_RIGHT   9

// Number of sensors
#define NUM_SENSORS 8

// Sensor configuration
const int sensorPins[NUM_SENSORS] = {
  SENSOR_FRONT, SENSOR_BACK, SENSOR_LEFT, SENSOR_RIGHT,
  SENSOR_FRONT_LEFT, SENSOR_FRONT_RIGHT, SENSOR_BACK_LEFT, SENSOR_BACK_RIGHT
};

const char* sensorNames[NUM_SENSORS] = {
  "FRONT", "BACK", "LEFT", "RIGHT",
  "FRONT_LEFT", "FRONT_RIGHT", "BACK_LEFT", "BACK_RIGHT"
};

// Previous sensor states for change detection
bool prevSensorStates[NUM_SENSORS] = {false};

// Initialize Bluetooth serial
SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  
  // Initialize Bluetooth serial
  bluetooth.begin(9600);
  
  // Configure buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Configure all sensor pins as inputs with pull-up resistors
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT_PULLUP);
    prevSensorStates[i] = false;
  }
  
  // Send startup message via Bluetooth
  bluetooth.println("ROBOT_READY");
  Serial.println("Metal Detecting Robot Initialized");
}

void loop() {
  bool metalDetected = false;
  
  // Check all sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    // Read sensor (LOW = metal detected for most inductive sensors)
    bool currentState = (digitalRead(sensorPins[i]) == LOW);
    
    // Check if metal is detected
    if (currentState) {
      metalDetected = true;
      
      // Send notification only when state changes (metal newly detected)
      if (!prevSensorStates[i]) {
        sendMetalAlert(i);
      }
    } else {
      // Send clear notification when metal is no longer detected
      if (prevSensorStates[i]) {
        sendMetalClear(i);
      }
    }
    
    // Update previous state
    prevSensorStates[i] = currentState;
  }
  
  // Control buzzer based on metal detection
  if (metalDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  // Handle incoming Bluetooth commands from app
  handleBluetoothCommands();
  
  // Small delay to prevent excessive readings
  delay(50);
}

// Send metal detection alert via Bluetooth
void sendMetalAlert(int sensorIndex) {
  String message = "METAL_DETECTED:";
  message += sensorNames[sensorIndex];
  
  bluetooth.println(message);
  Serial.println(message);
}

// Send metal clear notification via Bluetooth
void sendMetalClear(int sensorIndex) {
  String message = "METAL_CLEARED:";
  message += sensorNames[sensorIndex];
  
  bluetooth.println(message);
  Serial.println(message);
}

// Handle incoming Bluetooth commands from the app
void handleBluetoothCommands() {
  if (bluetooth.available()) {
    String command = bluetooth.readStringUntil('\n');
    command.trim();
    
    if (command == "STATUS") {
      // Send status of all sensors
      sendAllSensorStatus();
    } else if (command == "PING") {
      bluetooth.println("PONG");
    } else if (command == "BUZZER_TEST") {
      // Test buzzer
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      bluetooth.println("BUZZER_TEST_DONE");
    }
  }
}

// Send status of all sensors
void sendAllSensorStatus() {
  bluetooth.println("STATUS_START");
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    bool currentState = (digitalRead(sensorPins[i]) == LOW);
    String status = sensorNames[i];
    status += ":";
    status += (currentState ? "DETECTED" : "CLEAR");
    bluetooth.println(status);
  }
  
  bluetooth.println("STATUS_END");
}
