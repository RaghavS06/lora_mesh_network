#include <Arduino.h>

void setup() {
  Serial.begin(9600); // Initialize serial communication at 115200 baud rate
  Serial.println("--- ESP32 Startup ---");
}

void loop() {
  Serial.println("Hello from ESP32!"); // Print a message to the serial monitor
  delay(2000); // Wait for 2 seconds
}