#include "mbed.h"
#include "pins.h"
#include "beep.h"

DigitalOut buzz(BUZZER);

Ticker rise_t;
Ticker fall_t;

Timeout turn_off;
Timeout start_fall;

bool is_beeping = false;

// in us
int period = 1000;
float dc = 0.2;

float beep_time;
float beep_time_short = 0.05;
float beep_time_long = 0.3;

float beep_time_between = 0.2f;

void start_beep();
void start_down();
void beep_off();
void on();
void off();

void async_beep() {
    beep_time = beep_time_short;
    start_beep();
}

void beep_for_time(float time) {
    Timer t;
    t.start();
    
    while (t.read() < time) {
        buzz = 1;
        wait_us(period * dc);
        buzz = 0;
        wait_us(period * dc);
    }
}

void beep() {
    beep_for_time(beep_time_short);
}

void beep_long() {
    beep_for_time(beep_time_long);
}

void beeps(int n) {
    for (int i = 0; i < n; i++) {
        beep();
        wait(beep_time_between);
    }   
}

void start_beep() {
    if (!is_beeping) {
        is_beeping = true;
        rise_t.attach_us(&on, period);
        start_fall.attach_us(&start_down, period * dc);
    }
}

void start_down() {
    fall_t.attach_us(&off, period);
    turn_off.attach(&beep_off, beep_time);
}

void on() {
    buzz = 1;
}

void off() {
    buzz = 0;   
}

void beep_off() {
    rise_t.detach();
    fall_t.detach(); 
    is_beeping = false;  
}
