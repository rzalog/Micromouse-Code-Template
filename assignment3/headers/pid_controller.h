#pragma once

#include "globals.h"

const float KpX = 1;
const float KdX = 1;

const float KpW = 1;
const float KdW = 1;

class PIDController {
public:
    PIDController();
    
    void reset() volatile;
    void update() volatile;

    void setXGoal(int counts);
    void setWGoal(int counts);

    bool isDone() volatile;
    char* getData();
private:
    void getSensorFeedback() volatile;
    void x_controller() volatile;
    void w_controller() volatile;
    void updateMotorPwm() volatile;
    
    int m_goalW;
    int m_goalX;
    
    float m_pwmW;
    float m_pwmX;
    
    int m_errorW;
    int m_errorX;
    int m_errorW_old;
    int m_errorX_old;
    
    int m_countsW;
    int m_countsX;
    
    char buf[200];
};
