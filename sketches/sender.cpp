#include <Arduino.h>
#include <LoRa.h> // Include the LoRa library

// --- LoRa Module Pin Definitions ---
// These pins are HIGHLY dependent on your specific board!
// For Heltec WiFi LoRa 32 V3, common pins are:
#define SS      8   // SPI NSS (Slave Select)
#define RST     14  // LoRa Reset
#define DIO0    13  // LoRa DIO0 (Interrupt pin)
// For other boards (e.g., LilyGO T-Beam), these pins will be different.
// Check your board's schematics or examples! If unsure, search for "your_board_name LoRa pins".

// Define the frequency for LoRa communication (e.g., 915E6 for 915 MHz, 868E6 for 868 MHz)
// Use the frequency appropriate for your region!
#define LORA_FREQUENCY 915E6 // For North America (915 MHz)

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect. Needed for native USB port only

  Serial.println("LoRa Sender");

  // Set LoRa module pins. This needs to be done BEFORE LoRa.begin()
  LoRa.setPins(SS, RST, DIO0);

  // Initialize LoRa radio
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("Starting LoRa failed!");
    while (1); // Stay here if LoRa fails
  }
  Serial.println("LoRa initialized successfully!");
}

int counter = 0; // Message counter

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // Send LoRa packet
  LoRa.beginPacket();        // Start a new LoRa packet
  LoRa.print("hello ");     // Add your message
  LoRa.print(counter);       // Add the counter
  LoRa.endPacket();          // Finish and send the packet

  counter++;                 // Increment counter
  delay(5000);               // Wait 5 seconds before sending next packet
}