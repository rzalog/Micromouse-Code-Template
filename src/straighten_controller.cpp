#include "straighten_controller.h"
#include "sensors.h"
#include "motors.h"
#include "globals.h"

void fixBoth(float *rpwm, float *lpwm);
void adjustPwm(float* pwm);

StraightenController::StraightenController() {
    this->reset();
}

void StraightenController::reset() volatile
{
    done_counter = 0;
    done_count = 100;
    
    Kp = 1.5f;
    Kd = 0.0f;//100;

    right_error = 0;
    left_error = 0;
    
    right_error_old = 0;
    left_error_old = 0;

    rpwm = 0;
    lpwm = 0;
    
    m_is_done = false;
}

void StraightenController::update() volatile
{
    if (!m_is_done) {
        right_error = ir.RF_base() - ir.RF();
        left_error = ir.LF_base() - ir.LF();

        rpwm = Kp * right_error + Kd * (right_error - right_error_old);
        lpwm = Kp * left_error + Kd * (left_error - left_error_old);
        
        float tempr = rpwm;
        float templ = lpwm;
        fixBoth(&tempr, &templ);
        rpwm = tempr;
        lpwm = templ;

        right_error_old = right_error;
        left_error_old = left_error;
    
        if (abs(rpwm) <= MIN_SPEED && abs(lpwm) <= MIN_SPEED) {
            done_counter++;
            
            m_is_done = done_counter >= done_count;

            if (m_is_done) {
                motors.stop();
                m_is_done = true;
            }
        }
        if (!m_is_done) {
            motors.setRightPwm(rpwm);
            motors.setLeftPwm(lpwm);
        }
    }
}

bool StraightenController::isDone() volatile
{
    return m_is_done;   
}

void StraightenController::fixBoth(float *rpwm, float *lpwm) volatile {
    adjustPwm(rpwm);
    adjustPwm(lpwm);   
}

void StraightenController::adjustPwm(float* pwm) volatile {
    if (abs(*pwm) > MAX_IR_SPEED) {
        if (*pwm > 0) *pwm = MAX_IR_SPEED;
        else *pwm = -MAX_IR_SPEED;
    }
    if (abs(*pwm) < MIN_SPEED - 5.0f) {
        *pwm = 0;   
    }
}