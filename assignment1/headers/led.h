#pragma once

#include "mbed.h"

class Led {
public:
    Led(PinName pin);
    void toggle();
    void on();
    void off();
private:
    DigitalOut m_led;
};