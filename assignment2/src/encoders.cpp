#include "encoders.h"
#include "pins.h"
#include "globals.h"
#include "mbed.h"
#include "QEI.h"

/***
 * Assignment 2
 * 
 * Import the QEI in order to have functioning encoders.
 ***/
QEI wheel_R(ENC_RF, ENC_RB, NC, 624, QEI::X4_ENCODING);
QEI wheel_L(ENC_LF, ENC_LB, NC, 624, QEI::X4_ENCODING);

///////
// Encoder class implementation
///////

Encoder::Encoder() {
    reset();
}

void Encoder::reset() volatile {
    wheel_R.reset();
    wheel_L.reset();
}

int Encoder::getPulsesR() volatile {
    return wheel_R.getPulses();
}

int Encoder::getPulsesL() volatile {
    return wheel_L.getPulses();
}

void Encoder::printValues() volatile {
    pc.printf("R: %d\tL: %d\n", getPulsesR(), getPulsesL());
}