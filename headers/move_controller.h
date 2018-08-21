#pragma once

#include "sensors.h"
#include "globals.h"

// Human units, m.s, m.s^2
const float accX_mss = 2;
const float decX_mss = 2;

float m_s_to_count_ms(float);

// Now in counts.s, counts.s^2
const float accX = m_s_to_count_ms(accX_mss) / 1000;
const float decX = m_s_to_count_ms(decX_mss) / 1000;

const float accX_correction = accX / 2;
const float decX_correction = 0; //decX * 75;

const float accW = 0;
const float decW = 0;

// PID constants for movement
const float KpX = 0.01;
const float KdX = 0.1;//1;
const float KpW = 0.2;
const float KdW = 1.0; //0.2;//0.12; //0.0086 * 1000;//0.01;

// for turning
const float KpX_turn = 0.1;
const float KdX_turn = 0.8;
const float KpW_turn = 0.08;
const float KdW_turn = 0.8;

// for encoders: KpW=0.025, KdW=0.4

const float MAX_X_SPEED = MAX_SPEED * 0.33f;
const float MAX_W_SPEED = MAX_SPEED * 0.4f;

const float irScaling = 10;

const int STOP_PID_THRESHOLD = 75;

class MoveController {
public:
    MoveController();
    
    void reset() volatile;
    void update() volatile;
    
    float getIRFeedback() volatile;
    
    void setUseEncoders(bool val) volatile;
    void setUseGyro(bool val) volatile;
    void setUseIR(bool val) volatile;
    
    float countsTravelledX() volatile { return m_countsTravelledX; }
    float idealCountsTravelledX() volatile { return m_idealCountsTravelledX; }
    bool isDone() volatile { return m_PIDDone; }
    
    void setCountsToMove(int counts) volatile;
    void setCountsToTurn(int counts) volatile;
    void updateAlt();
    
    float cells() volatile { return m_cells; };
    
    void setAdjustPwm(bool val) volatile { m_adjustPwm = val; }
    
//private:
    void updateSensorFeedback() volatile;
    
    void adjustPwm(float* pwmx, float* pwmw) volatile;
    
    // State
    bool m_useEncoderFeedback;
    bool m_useGyroFeedback;
    bool m_useIRFeedback;
    
    // Speeds
    float m_targetSpeedX;
    float m_targetSpeedW;
    
    float m_curSpeedX;
    float m_curSpeedW;
    
    // Sensor feedback
    int m_rightEncoderOld;
    int m_leftEncoderOld;
    
    float m_rightEncoderChange;
    float m_leftEncoderChange;
    
    // Errors
    float m_posErrorX;
    float m_posErrorW;
    float m_oldPosErrorX;
    float m_oldPosErrorW;
    
    // Pwm's
    float m_pwmX;
    float m_pwmW;
    
    int m_stopPIDCounter;
    
    float m_idealCountsTravelledX;
    float m_idealCountsTravelledW;
    float m_countsTravelledX;
    float m_countsTravelledW;
    
    bool m_PIDDone;
    bool m_isStartingToDecelerate;
    bool m_isStartingToAccelerate;
    
    float m_cells;
    bool m_adjustPwm;
    
    bool is_turning;
};

// meters.second to count.millisecond (need to divide by 1000 for accelerations)
inline float m_s_to_count_ms(float val) {
    float output = mm_to_count(val);
    return output;
}