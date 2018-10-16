#pragma once

const float PWM_SCALE = 100.0f;

/***
 * Assignment 2
 * 
 * Find good values for these parameters.
 * 
 * It is better to have lower MIN_SPEED and MAX_SPEED.
 * However, this may cause your motors to stall or draw
 * too much current, so you have to tune them yourself.
 * 
 * Hint: 0.0 is much too small, 1.0 is much too big!
 ***/

const float MIN_SPEED = 0.0f * PWM_SCALE;
const float MAX_SPEED = 1.0f * PWM_SCALE;

const int RIGHT_MOTOR = 0;
const int LEFT_MOTOR = 1;

class Motors {
public:
    Motors();
    void startBaseSpeed();
    void stop();

    /***
     * Range from 0.0-100.0.
     ***/

    void setLeftPwm(float pwm);
    void setRightPwm(float pwm);
private:
    void setMotorPwm(int motor, float pwm);
    
    float m_rpwm;
    float m_lpwm;
};