#include <Arduino.h>
#include <LoRa.h> // Include the LoRa library

// --- LoRa Module Pin Definitions ---
// These pins are HIGHLY dependent on your specific board!
// Use the SAME pin definitions as your Sender (Node 1)!
#define SS      8   // SPI NSS (Slave Select)
#define RST     14  // LoRa Reset
#define DIO0    13  // LoRa DIO0 (Interrupt pin)

// Define the frequency for LoRa communication (MUST MATCH SENDER!)
#define LORA_FREQUENCY 915E6 // For North America (915 MHz)

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect

  Serial.println("LoRa Receiver");

  // Set LoRa module pins. This needs to be done BEFORE LoRa.begin()
  LoRa.setPins(SS, RST, DIO0);

  // Initialize LoRa radio
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("Starting LoRa failed!");
    while (1); // Stay here if LoRa fails
  }
  Serial.println("LoRa initialized successfully!");
}

void loop() {
  // Try to parse a received LoRa packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // Received a packet
    Serial.print("Received packet '");

    // Read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // Print RSSI (Received Signal Strength Indicator) and SNR (Signal-to-Noise Ratio)
    Serial.print("' with RSSI ");
    Serial.print(LoRa.packetRssi());
    Serial.print(" and SNR ");
    Serial.println(LoRa.packetSnr());
  }
}