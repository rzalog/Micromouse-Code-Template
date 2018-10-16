#pragma once

#include "mbed.h"
#include "QEI.h"

/***
 * Assignment 4
 * 
 * Determine these values. They should be slightly
 * off middle. E.g., MIDR is the right side reading,
 * when the mouse is pushed slightly to the right
 ***/
const float MIDR = 0.0f; // TODO
const float MIDL = 0.0f; // TODO

/***
 * Assignment 2
 * 
 * Experimentally determine this value.
 ***/
const float COUNTS_FOR_CELL = 0; // TODO

// IR Stuff
const int WARM_UP_US = 40;
const int COOL_DOWN_US = 50;

class Encoder {
public:
    Encoder();
    void reset() volatile;
    void update() volatile;
    int getPulsesR() volatile;
    int getPulsesL() volatile;
    
    void printValues() volatile;
private:    
    volatile int m_countR;
    volatile int m_countL;
};

class IR {
public:
    IR();
    void update() volatile;

    float RF() volatile { return m_rf; };
    float LF() volatile { return m_lf; };
    float RD() volatile { return m_rd; };
    float LD() volatile { return m_ld; };
    float RS() volatile { return m_rs; };
    float LS() volatile { return m_ls; };
        
    float RS_error() volatile;
    float LS_error() volatile;
    
    void printValues() volatile;
private:
    float flash_ir(DigitalOut ir, AnalogIn rec) volatile;
    float read_ir(AnalogIn rec) volatile;

    float m_rf;
    float m_lf;
    float m_rd;
    float m_ld;
    float m_rs;
    float m_ls;
};

class Gyro {
public:
    Gyro();
    void update(float dt) volatile;
    float angularVelocity() volatile;
    float theta() volatile;
    void reset() volatile;
    
    void setDrift() volatile;
    float getDrift() volatile;
    
    float rawReading() volatile;
    
    void printValues() volatile;
private:
    float volatile m_curAngularVelocity;
    float volatile m_theta;
    float volatile m_correction;
};