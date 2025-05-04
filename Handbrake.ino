#include <ArduinoTrace.h>

#ifndef ARDUINO_USB_MODE
#error This ESP32 SoC has no Native USB interface
#elif ARDUINO_USB_MODE == 1
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else

#include <Arduino.h>
#include <USB.h>

// Define the analog pin connected to the potentiometer
#define POTENTIOMETER_PIN A1


#define DEBUG 1
#define ARDUINOTRACE_ENABLE DEBUG

#define READING_DELAY 15


int maxMilliVolts = 2500;
int8_t currentY = 0;
bool hasPassedThreshold = false;

#if DEBUG == 1
#include "src/SerialGamepad/SerialGamepad.h";
SerialGamepad Gamepad = SerialGamepad();
#else
#include "USBHIDGamepad.h"
USBHIDGamepad Gamepad = USBHIDGamepad();
#endif

void setup() {
  #if DEBUG == 1
  Serial.begin(9600);
  Serial.println("Starting Serial");
  #endif

  Gamepad.begin();
  #if DEBUG == 0
  USB.begin();
  #endif
}

unsigned int counter = 0;
void loop() {
  int potValue = analogReadMilliVolts(POTENTIOMETER_PIN);


  if(potValue > maxMilliVolts) {
    maxMilliVolts = potValue;
  }
  int8_t yAxisValue = (int8_t) map(potValue, 0, maxMilliVolts, 127, -127);

  if(yAxisValue != currentY) {
    DUMP(potValue);
    DUMP(yAxisValue);

    currentY = yAxisValue;
    
    Gamepad.leftStick((int8_t) 0, yAxisValue);
  }


  delay(READING_DELAY);
}





#endif /* ARDUINO_USB_MODE */
