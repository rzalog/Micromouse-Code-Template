#include "led.h"
#include "pins.h"

Led::Led(PinName pin) : m_led(pin) {
    
}

void Led::toggle() {
    m_led = !m_led;   
}

void Led::on() {
    m_led = 1;   
}

void Led::off() {
    m_led = 0;   
}

void Led::set(int val) {
    m_led = val;
}