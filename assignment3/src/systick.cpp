#include "systick.h"
#include "mbed.h"
#include "globals.h"

void systickFunction() {
    // Update global counter
    millis++;

    /**
     * Assignment 3ab: Do something with your controllers here,
     * maybe update them? (hint hint)
     **/
}

void Systick::start() {
    /**
     * Assignment 3a: Start your systick function (above).
     * Hint: use 'm_systicker', the Ticker variable (systick.h).
     **/
}

void Systick::stop() {
    /**
     * Assignment 3a: Stop the systick (use "detach" function of the Ticker class).
     **/
}

Systick::Systick() {
    // A little bit of magic; if you want to know why, ask Robert
    NVIC_SetPriority(TIM5_IRQn, 255);
}

void Systick::wait(float sec) {
    // Utility function: allows you to wait while using systick.
    int init = millis;
    
    float num_ticks = sec / SYS_TICK_TIME;

    while (millis - init < num_ticks)
        ;
}