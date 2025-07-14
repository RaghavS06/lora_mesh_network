#include <Arduino.h>
#include <LoRa.h>         // LoRa library
#include <SPI.h>          // SPI library for LoRa
#include <Wire.h>         // I2C library for OLED
#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_SSD1306.h> // OLED display library

// --- LoRa Module Pin Definitions ---
// These pins MUST match your Heltec WiFi LoRa 32 V3 setup!
#define SS      8   // SPI NSS (Slave Select)
#define RST     14  // LoRa Reset
#define DIO0    13  // LoRa DIO0 (Interrupt pin)

// Define the frequency for LoRa communication (MUST MATCH SENDER!)
#define LORA_FREQUENCY 915E6 // For North America (915 MHz)

// --- OLED Display Definitions ---
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The Heltec V3 often uses specific I2C pins for the OLED.
// Common for Heltec V3: SDA=GPIO4, SCL=GPIO15.
// Some boards might internally wire it, check your board's examples if you face issues.
// If your board has built-in OLED, it often uses Wire (I2C) and the standard pins.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Node ID ---
const int MY_NODE_ID = 2; // This is Node 2 (the receiver)

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect. Needed for native USB port only

  Serial.println("LoRa Receiver with OLED");

  // --- OLED Initialization ---
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C is common for 128x64 OLED
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) delay(1000); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); // Set text color to white
  display.setTextSize(1);              // Set text size (1 is smallest)
  display.setCursor(0,0);              // Set cursor to top-left
  display.println("LoRa Receiver");
  display.print("Node ID: ");
  display.println(MY_NODE_ID);
  display.display();                   // Show initial display buffer contents on the screen

  delay(2000); // Short delay to read initial message

  // --- LoRa Initialization ---
  LoRa.setPins(SS, RST, DIO0); // Set LoRa module pins
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("Starting LoRa failed!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("LoRa FAIL!");
    display.display();
    while (1); // Stay here if LoRa fails
  }
  LoRa.setSyncWord(0xF4); // Optional: Set a unique sync word to avoid interference with other LoRa devices
  Serial.println("LoRa initialized successfully!");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LoRa Ready!");
  display.display();
}

void loop() {
  // Try to parse a received LoRa packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // Received a packet
    Serial.print("Received packet '");

    String receivedMessage = "";
    int senderId = -1; // Placeholder for sender ID
    char firstChar = (char)LoRa.peek(); // Peek at the first character

    // Basic assumption: sender ID is the first character/digit.
    // This will be refined with your structured message later!
    if (isDigit(firstChar)) {
      senderId = firstChar - '0'; // Convert char to int
      LoRa.read(); // Consume the character
    } else {
      senderId = -1; // Unknown sender
    }


    // Read the rest of the packet
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }

    // Print to Serial Monitor for debugging
    Serial.print(receivedMessage);
    Serial.print("' from Node ");
    Serial.print(senderId == -1 ? "UNKNOWN" : String(senderId));
    Serial.print(" with RSSI ");
    Serial.print(LoRa.packetRssi());
    Serial.print(" and SNR ");
    Serial.println(LoRa.packetSnr());

    // --- Display on OLED ---
    display.clearDisplay();        // Clear the entire display buffer
    display.setTextSize(1);        // Set text size (1 is smallest, 2 is larger)
    display.setTextColor(SSD1306_WHITE); // Set text color

    // Display Sender ID
    display.setCursor(0,0);        // Top-left corner
    display.print("From Node: ");
    display.println(senderId == -1 ? "UNKNOWN" : String(senderId));

    // Display RSSI and SNR
    display.print("RSSI: ");
    display.print(LoRa.packetRssi());
    display.print(" SNR: ");
    display.println(LoRa.packetSnr());

    // Add a line break or blank line
    display.println(); // Prints a newline based on text size

    // Display the received message, potentially wrapping text
    display.setTextSize(1); // Maybe revert to smaller size for message content
    display.setCursor(0, display.getCursorY()); // Continue from current Y position
    display.println("Msg: "); // Label for message
    display.println(receivedMessage); // Actual message

    display.display(); // Push everything from the buffer to the screen
  }
}