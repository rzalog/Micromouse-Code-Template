#pragma once

#include "motors.h"

const float MAX_IR_SPEED = MAX_SPEED * 0.5f;

class StraightenController {
public:
    StraightenController();
    void reset() volatile;
    void update() volatile;

    bool isDone() volatile;
//private:
    void adjustPwm(float* pwm) volatile;
    void fixBoth(float *rpwm, float *lpwm) volatile;

    int done_counter;
    int done_count;
    
    float Kp;
    float Kd;
    
    float right_error;
    float left_error;
    
    float right_error_old;
    float left_error_old;
    
    float rpwm;
    float lpwm;
    
    bool m_is_done;
};