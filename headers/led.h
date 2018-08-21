#pragma once

#include "mbed.h"

class Led {
public:
    Led(PinName pin);
    void toggle();
    void on();
    void off();
    void set(int val);
private:
    DigitalOut m_led;   
};