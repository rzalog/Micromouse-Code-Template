#pragma once

#include "mbed.h"

const float SYS_TICK_TIME = .001;

void systickFunction();

class Systick {
public:
    Systick();
     
    void start();
    void stop();
    
    void wait(float sec);
    
    void setReadEncoders(bool val);
    void setReadGyro(bool val);
    void setReadIR(bool val);
    
    void updateSensors();
    
private:
    Ticker m_systicker;
    
    bool m_readEncoders;
    bool m_readGyro;
    bool m_readIR;
};