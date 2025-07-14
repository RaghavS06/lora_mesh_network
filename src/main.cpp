#include <Arduino.h>

// --- IMPORTANT: CHANGE THIS TO YOUR BOARD'S ACTUAL LED PIN ---
// For Heltec WiFi LoRa 32 V3 (common): try GPIO 35
// For LilyGO TTGO T-Beam (common): try GPIO 4
// For older Heltec WiFi LoRa 32 (V2 from your Amazon link): often GPIO 25 or GPIO 2
#define MY_LED_PIN 35 // <--- **CHANGE THIS** to your likely LED pin

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Starting up...");
  Serial.print("Attempting to blink LED on GPIO: ");
  Serial.println(MY_LED_PIN);

  // Initialize LED pin as an OUTPUT.
  pinMode(MY_LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(MY_LED_PIN, HIGH);   // Turn the LED on
  Serial.println("LED ON");
  delay(1000);                       // Wait for a second
  digitalWrite(MY_LED_PIN, LOW);    // Turn the LED off
  Serial.println("LED OFF");
  delay(1000);                       // Wait for a second
}