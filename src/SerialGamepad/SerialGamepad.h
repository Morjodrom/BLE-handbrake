#ifndef SERIAL_GAMEPAD_H
#define SERIAL_GAMEPAD_H
#include <Arduino.h>
#include <USBHIDGamepad.h>

class SerialGamepad: public USBHIDGamepad
{
    protected:
        int8_t _x;         
        int8_t _y;         
        int8_t _z;         
        int8_t _rz;        
        int8_t _rx;        
        int8_t _ry;        
        uint8_t _hat;      
        uint32_t _buttons; 

    public:
        SerialGamepad(void);
        void begin(unsigned long baud = 9600);

        void end(void);

        bool leftStick(int8_t x, int8_t y);
        bool rightStick(int8_t z, int8_t rz);

        bool leftTrigger(int8_t rx);
        bool rightTrigger(int8_t ry);

        bool hat(uint8_t hat);

        bool pressButton(uint8_t button);
        bool releaseButton(uint8_t button);

    protected: 

        bool print(const char* caption, int value);
        bool print(const char* captoin, int value, int anotherValue);
};
#endif