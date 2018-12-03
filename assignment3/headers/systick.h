#pragma once

#include "mbed.h"

void systickFunction();

class Systick {
public:
    Systick();
     
    void start();
    void stop();
    
    void wait(float sec);    
private:
    Ticker m_systicker;
};