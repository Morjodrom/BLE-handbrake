#include <BleGamepad.h>

// Define the analog input pin
const int analogPin = 4; // GPIO 4 (Analog Input)

// Create a BLE Gamepad object
BleGamepad bleGamepad;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize the BLE Gamepad
  bleGamepad.begin();
  Serial.println("BLE Gamepad started!");
}

void loop() {
  // Check if the BLE Gamepad is connected
  if (bleGamepad.isConnected()) {
    // Read the analog input value (0 to 4095 for ESP32)
    int analogValue = analogRead(analogPin);

    // Map the analog value to a range suitable for the X-axis (-32767 to 32767)
    int16_t xAxisValue = map(analogValue, 0, 4095, -32767, 32767);

    // Set the X-axis value
    bleGamepad.setX(xAxisValue);

    // Send the updated gamepad state
    bleGamepad.sendReport();

    // Debugging output
    Serial.print("Analog Value: ");
    Serial.print(analogValue);
    Serial.print(" | X-Axis Value: ");
    Serial.println(xAxisValue);
  }

  // Small delay to avoid flooding the BLE connection
  delay(10);
}