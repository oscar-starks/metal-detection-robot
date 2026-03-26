#include <SoftwareSerial.h>

/*
  HC-05 Bluetooth Module Wiring (Arduino Nano/Uno):
  -------------------------------------------------
  HC-05 TX  →  Arduino Pin 10 (Software RX)
  HC-05 RX  →  Arduino Pin 11 (Software TX) via voltage divider*
  HC-05 VCC →  Arduino 5V (or 3.3V depending on your module)
  HC-05 GND →  Arduino GND

  * Voltage divider on Pin 11 → HC-05 RX (protects the module from 5V):
      Arduino Pin 11 → 1kΩ → junction → HC-05 RX
                                 |
                                2kΩ
                                 |
                                GND

  Notes:
  - Upload sketch with HC-05 disconnected to avoid upload errors
  - Reconnect HC-05 after upload is complete
  - Open Serial Monitor at 9600 baud
*/

// SoftwareSerial(RX, TX) — creates a serial port on pins 10 and 11
// This frees up pins 0 & 1 so the Serial Monitor still works over USB
SoftwareSerial BT(10, 11); // RX = Pin 10, TX = Pin 11

void setup() {
  Serial.begin(9600);   // Start Serial Monitor communication (USB)
  BT.begin(9600);       // Start Bluetooth communication (must match HC-05 baud rate)
  Serial.println("Bluetooth ready. Waiting for data...");
}

void loop() {

  // --- Receive from phone, print to Serial Monitor ---
  if (BT.available()) {
    String received = "";

    // Read all available characters into a string
    while (BT.available()) {
      received += (char)BT.read();
      delay(2); // short delay to allow buffer to fill between characters
    }

    Serial.print("From phone: ");
    Serial.println(received);
  }

  // --- Receive from Serial Monitor, send to phone ---
  if (Serial.available()) {
    String outgoing = "";

    // Read all available characters into a string
    while (Serial.available()) {
      outgoing += (char)Serial.read();
      delay(2); // short delay to allow buffer to fill between characters
    }

    BT.print(outgoing);
    Serial.print("Sent to phone: ");
    Serial.println(outgoing);
  }
}