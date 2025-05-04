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
#define GREEN_BUTTON_PIN 1
#define RED_BUTTON_PIN 3


#define DEBUG 1
#define ARDUINOTRACE_ENABLE DEBUG

#define ANALOG_POLLING_INTERVAL_MS 15
uint32_t lastTime = 0;
int maxMilliVolts = 1500;
int minMilliVolts = 1499;
int8_t currentY = 0;

const uint16_t BTN_DEBOUNCE_MS = 50;
bool wasGreenPressed = false;
uint32_t lastGreenTime = 0;
bool wasRedPressed = false;
uint32_t lastRedTime = 0;


#if DEBUG == 1
#include "src/SerialGamepad/SerialGamepad.h";
SerialGamepad Gamepad = SerialGamepad();
#else
#include "USBHIDGamepad.h"
USBHIDGamepad Gamepad = USBHIDGamepad();
#endif

void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);

  #if DEBUG == 1
  Serial.begin(9600);
  Serial.println("Starting Serial");
  #endif

  Gamepad.begin();
  #if DEBUG == 0
  USB.begin();
  #endif
}

void loop() {
  uint32_t currentTime = millis();

  pollButton(BUTTON_A, GREEN_BUTTON_PIN, &wasGreenPressed, &lastGreenTime);
  pollButton(BUTTON_B, RED_BUTTON_PIN, &wasRedPressed, &lastRedTime);

  if(currentTime - lastTime > ANALOG_POLLING_INTERVAL_MS) {
    pollAnalog();
  }
}

void pollButton(uint8_t btn, uint8_t pin, bool* wasBtnPressed, uint32_t* lastBtnPollTime)
{
  uint32_t currentTime = millis();
  bool isGreenPressed = !digitalRead(pin);
  if(isGreenPressed && !*wasBtnPressed && currentTime - *lastBtnPollTime > BTN_DEBOUNCE_MS) {
    *wasBtnPressed = true;
    *lastBtnPollTime = currentTime;
    Gamepad.pressButton(btn);
  }
  if(!isGreenPressed && *wasBtnPressed && currentTime - *lastBtnPollTime > BTN_DEBOUNCE_MS) {
    *wasBtnPressed = false;
    *lastBtnPollTime = currentTime;
    Gamepad.releaseButton(btn);
  }
}

void pollAnalog()
{
  int potValue = analogReadMilliVolts(POTENTIOMETER_PIN);

  if (potValue > maxMilliVolts){
    maxMilliVolts = potValue;
  }
  if(potValue < minMilliVolts){
    minMilliVolts = potValue;
  }

  int8_t yAxisValue = (int8_t)map(potValue, minMilliVolts, maxMilliVolts, 127, -127);

  if (yAxisValue != currentY)
  {
    DUMP(potValue);
    DUMP(yAxisValue);

    currentY = yAxisValue;

    Gamepad.leftStick((int8_t)0, yAxisValue);
  }
}

#endif /* ARDUINO_USB_MODE */
