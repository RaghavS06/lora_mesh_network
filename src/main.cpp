#include <Arduino.h>
#include <HT_SSD1306Wire.h> // Heltec's specific OLED library

// --- Heltec V3.2 Official OLED Pin & Config Definitions ---
// These are pre-defined macros in the Heltec environment.
// SDA_OLED (GPIO 17)
// SCL_OLED (GPIO 18)
// RST_OLED (GPIO 21) - If your board has an explicit reset pin for the OLED.

// Define SCREEN_WIDTH and SCREEN_HEIGHT explicitly for your sketch
#define SCREEN_WIDTH 128    // OLED display width in pixels
#define SCREEN_HEIGHT 64    // OLED display height in pixels

// Create the display object using HT_SSD1306Wire.
// Constructor: address, freq (Hz), SDA pin, SCL pin, geometry, reset pin
static SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

// --- Onboard LED Definition ---
// For Heltec WiFi LoRa 32 V3.2, the onboard orange LED is GPIO 35.
// The "redefined" warning for LED_BUILTIN is fine and can be ignored.
#define LED_BUILTIN 35

// --- Vext Control Functions ---
// Vext powers external peripherals. The OLED uses it.
// Vext is HIGH by default, meaning OFF. LOW means ON.
void VextON(void) {
  pinMode(Vext, OUTPUT); // Vext is a pre-defined pin in Heltec environment
  digitalWrite(Vext, LOW); // Set Vext to LOW to turn ON power
}

void VextOFF(void) { // Vext default OFF
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH); // Set Vext to HIGH to turn OFF power
}

void setup() {
  // 1. Initialize Serial Communication
  Serial.begin(9600);
  delay(1000); // Give serial monitor time to connect

  // Optional: Wait explicitly for serial if needed for robust debugging
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 4000)) {
    delay(100);
  }

  Serial.println("\n--- Heltec V3.2 Project Base Sketch ---");
  Serial.println("Serial communication initialized.");

  // 2. Control Vext (Power for OLED/external sensors)
  Serial.println("Turning Vext ON (powering OLED)...");
  VextON();
  delay(150); // Give Vext time to stabilize power for the OLED

  // 3. Initialize OLED Display
  Serial.println("Initializing OLED...");
  display.init(); // Initialize the display controller
  
  // Verify OLED connection using display.connect()
  if (!display.connect()) { // <-- FIXED: Changed from isConnected() to connect()
    Serial.println(F("ERROR: OLED not found or not connected!"));
    pinMode(LED_BUILTIN, OUTPUT); // Ensure LED pin is output
    while (1) { // Halt and blink LED on error
      digitalWrite(LED_BUILTIN, HIGH); delay(200);
      digitalWrite(LED_BUILTIN, LOW); delay(200);
    }
  } else {
    Serial.println(F("SUCCESS: OLED found and initialized."));
  }

  // 4. Configure & Clear OLED
  display.clear(); // Clear the display buffer
  display.display(); // Push clear buffer to screen (make it blank)
  display.setContrast(255); // Max contrast
  display.setTextAlignment(TEXT_ALIGN_CENTER); // Center text alignment
  display.setFont(ArialMT_Plain_16); // Use a known working font from your example

  // 5. Display Initial Message on OLED
  // Use SCREEN_WIDTH and SCREEN_HEIGHT for centering calculations
  display.drawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 16 / 2, "Node Initializing..."); // <-- FIXED: Added SCREEN_WIDTH/HEIGHT defines
  display.display();
  Serial.println("Initial message displayed on OLED.");

  // 6. Initialize Onboard LED (for loop indicator)
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Setup complete. Entering loop().");
}

void loop() {
  // Blink the LED to confirm loop is running
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // You can update the OLED here in loop() if you want dynamic content
  // Example: display.clear(); display.drawString(0,0,"Loop: " + String(millis()/1000)); display.display();
}