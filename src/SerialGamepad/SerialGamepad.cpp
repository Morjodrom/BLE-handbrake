#include "SerialGamepad.h";

SerialGamepad::SerialGamepad() : _x(0), _y(0), _z(0), _rz(0), _rx(0), _ry(0), _hat(0), _buttons(0)
{}

void SerialGamepad::begin(void)
{
    Serial.begin();
    Serial.println("\n==================\nUSB Gamepad Testing\n==================\n");
}

void SerialGamepad::end(void)
{
    Serial.end();
}

bool SerialGamepad::leftStick(int8_t rx, int8_t ry)
{
    print(__func__, rx, ry);
}

bool SerialGamepad::rightStick(int8_t rx, int8_t ry)
{
    print(__func__, rx, ry);
}

bool SerialGamepad::leftTrigger(int8_t rx)
{
    print(__func__, rx);
}

bool SerialGamepad::rightTrigger(int8_t ry)
{
    print(__func__, ry);
}

bool SerialGamepad::hat(uint8_t hat)
{
    print(__func__, hat);
}

bool SerialGamepad::pressButton(uint8_t button)
{
    print(__func__, button);
}

bool SerialGamepad::releaseButton(uint8_t button)
{
    print(__func__, button);
}

bool write()
{
    Serial.println("Write");
}

void SerialGamepad::print(const char* caption, int value)
{
    Serial.println(caption + ' ' + String(value));
}

void SerialGamepad::print(const char* caption, int value, int anotherValue)
{
    Serial.println(caption + ' ' + String(value) + ' ' + String(anotherValue));
}