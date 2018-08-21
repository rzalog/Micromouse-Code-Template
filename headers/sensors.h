#pragma once

#include "mbed.h"
#include "QEI.h"

const float INF_IR_READING = 1000.0f;

const float VSHORT_RIGHT = 20.0f;
const float VSHORT_LEFT = 25.0f;

const float SHORT_FRONT_WALL_THRESHOLD_RF = 4.5f;
const float SHORT_FRONT_WALL_THRESHOLD_LF = 4.6f;

const float LONG_FRONT_WALL_THRESHOLD_RF = 1.0f;
const float LONG_FRONT_WALL_THRESHOLD_LF = 1.3f;

const float DIAG_RIGHT = 0.4f;
const float DIAG_LEFT = 0.8f;

const float WALL_RIGHT = 5.0f;
const float WALL_LEFT = 4.2f;

const float FRONTR = 31.60f;
const float FRONTL = 39.67f;

const float MIDR = 41.04f;
const float MIDL = 26.93f;

const float IR_PID_THRESHOLD = 10.0f;

const float MM_TO_COUNT = 28.017f * 1.045f;//1.02f;
const float COUNT_TO_MM = 1 / MM_TO_COUNT;

const float COUNTS_FOR_CELL = 180.0f * MM_TO_COUNT;

const float RIGHT_ENCODER_FIX = 1.0;

const float GYRO_DRIFT_CORRECTION = 13.2; // Experimentally determined...

// IR Stuff
const int WARM_UP_US = 40;
const int COOL_DOWN_US = 50;
const int NREADS = 20;

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
    void updateDiag() volatile;
    void flashDiag() volatile;
    void flashFront() volatile;
    void flashBoth() volatile;
    void frontOn() volatile { front_ON = true; }
    void frontOff() volatile { front_ON = false; }
    void diagOn() volatile { diag_on = true; }
    void diagOff() volatile { diag_on = false; }
//    void diagOn() volatile ( diag_on = true; }
//    void diagOff() volatile ( diag_on = false; }
    float RF() volatile { return m_rf; };
    float LF() volatile { return m_lf; };
    float RD() volatile { return m_rd; };
    float LD() volatile { return m_ld; };
    float RS() volatile { return m_rs; };
    float LS() volatile { return m_ls * 2; };   // yes this is on purpose, to match RS
    
    void setFrontBase() volatile;
    float RF_base() volatile { return FRONTR; };
    float LF_base() volatile { return FRONTL; };
    void setSideBase() volatile;
    float RS_base() volatile { return MIDR; };
    float LS_base() volatile { return MIDL; };
    
    float RS_error() volatile;
    float LS_error() volatile;
    
    void printValues() volatile;
private:
    float adjust_reading(float reading) volatile;
    float flash_ir(DigitalOut ir, AnalogIn rec) volatile;
    float read_ir(AnalogIn rec) volatile;

    float m_rf;
    float m_lf;
    float m_rd;
    float m_ld;
    float m_rs;
    float m_ls;
    
    float m_rf_base;
    float m_lf_base;
    float m_rs_base;
    float m_ls_base;
    
    bool diag_flash;
    bool front_flash;
    
    bool front_ON;
    bool diag_on;
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

bool veryShortWall();
bool shortWall();
bool longWall();

bool leftDiagOpening();
bool rightDiagOpening();

bool leftWallOpening();
bool rightWallOpening();

inline bool isLeftWall() { return !leftWallOpening(); }
inline bool isRightWall() { return !rightWallOpening(); }

inline float count_to_mm (float count) {
    return count * COUNT_TO_MM;
}

inline float mm_to_count(float mm) {
    return mm * MM_TO_COUNT;
}