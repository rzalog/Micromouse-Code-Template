#include "systick.h"
#include "mbed.h"
#include "globals.h"

void systickFunction() {
    millis++;
    
    systick.updateSensors();
    mainController.update();
}

Systick::Systick() {
    NVIC_SetPriority(TIM5_IRQn, 255);
    m_readEncoders = true;
    m_readGyro = false;
    m_readIR = true;
}

void Systick::start() {
    m_systicker.attach(&systickFunction, SYS_TICK_TIME);
}

void Systick::stop() {
    m_systicker.detach();
}

void Systick::wait(float sec) {
    int init = millis;
    
    float num_ticks = sec / SYS_TICK_TIME;

    while (millis - init < num_ticks)
        ;
}

void Systick::setReadEncoders(bool val) {
    m_readEncoders = val;
}

void Systick::setReadGyro(bool val) {
    m_readGyro = val;
}

void Systick::setReadIR(bool val) {
    m_readIR = val;
}

void Systick::updateSensors() {
    if (m_readEncoders) {
        encoders.update();
    }
    
    if (m_readGyro) {
        gyro.update(SYS_TICK_TIME);
    }
    
    if (m_readIR) {
        ir.update();
    }
}

