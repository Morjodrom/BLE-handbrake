#ifndef ARDUINO_USB_MODE
#error This ESP32 SoC has no Native USB interface
#elif ARDUINO_USB_MODE == 1
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else

#include <Arduino.h>
#include "USB.h"
#include "USBHIDGamepad.h"

// Define the analog pin connected to the potentiometer
#define POTENTIOMETER_PIN A1

// Define the threshold values for the button press
#define DESCRETE_BRAKE_MIN 300
#define DESCRETE_BRAKE_MAX 2300
#define DESCRETE_BTN BUTTON_Z
#define DESCRETE_TRIGGER BUTTON_X
#define DEBUG 0
#define READING_DELAY 15


int maxMilliVolts = 2500;
int8_t lastY = 0;
bool hasPassedThreshold = false;

USBHIDGamepad Gamepad = USBHIDGamepad();

void setup() {
  Gamepad.begin();
  USB.begin();

  #if DEBUG == 1
  Serial.begin();
  Serial.println("\n==================\nUSB Gamepad Testing\n==================\n");
  #endif
}

void loop() {
  int potValue = analogReadMilliVolts(POTENTIOMETER_PIN);
  if(potValue > maxMilliVolts) {
    maxMilliVolts = potValue;
  }
  int8_t yAxisValue = (int8_t) map(potValue, 0, maxMilliVolts, 127, -127);

  #if DEBUG == 1
  Serial.println("Pot value" + String(potValue));
  Serial.println("Y value" + String(yAxisValue));
  #else

  Gamepad.leftStick((int8_t) 10, yAxisValue);

  if (potValue <= DESCRETE_BRAKE_MIN && !hasPassedThreshold) {
      Gamepad.pressButton(DESCRETE_TRIGGER);
      hasPassedThreshold = true;
  } else if (potValue > DESCRETE_BRAKE_MIN && hasPassedThreshold) {
      Gamepad.pressButton(DESCRETE_TRIGGER);
      hasPassedThreshold = false;
      Gamepad.releaseButton(DESCRETE_TRIGGER);
  }
  #endif

  delay(READING_DELAY);
}





#endif /* ARDUINO_USB_MODE */
