#include "encoders.h"
#include "pins.h"
#include "globals.h"
#include "mbed.h"

/***
 * Assignment 2
 * 
 * Import the QEI in order to have functioning encoders.
 ***/
QEI wheel_R(ENC_RA, ENC_RB, NC, 624, QEI::X4_ENCODING);
QEI wheel_L(ENC_LA, ENC_LB, NC, 624, QEI::X4_ENCODING);

///////
// Encoder class implementation
///////

Encoder::Encoder() {
    reset();
}

void Encoder::reset() volatile {
    m_countR = 0;
    m_countL = 0;
    wheel_R.reset();
    wheel_L.reset();
}

void Encoder::update() volatile {
    m_countR = wheel_R.getPulses();
    m_countL = wheel_L.getPulses();
}

int Encoder::getPulsesR() volatile {
    return m_countR;
}

int Encoder::getPulsesL() volatile {
    return m_countL;
}

void Encoder::printValues() volatile {
    pc.printf("R: %d\tL: %d\n", m_countR, m_countL);
}