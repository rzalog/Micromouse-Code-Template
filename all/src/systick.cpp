#include "systick.h"
#include "mbed.h"

void systickFunction() {
    millis++;
    
    /**
     * Need to do something in systick? This is the place!
     **/
}

Systick::Systick() {
    // A little bit of magic: if you want to know why, ask Robert
    NVIC_SetPriority(TIM5_IRQn, 255);
}

void Systick::start() {
    /**
     * Start systick. Hint: use 'm_ticker', the Ticker variable.
     **/
}

void Systick::stop() {
    // Stop systick
}

void Systick::wait(float sec) {
    // Utility function: allows you to wait while using systick.
    int init = millis;
    
    float num_ticks = sec / SYS_TICK_TIME;

    while (millis - init < num_ticks)
        ;
}