#include "motors.h"
#include "mbed.h"
#include "pins.h"

PwmOut m_RF(MOTOR_RF);
PwmOut m_RB(MOTOR_RB);
PwmOut m_LF(MOTOR_LF);
PwmOut m_LB(MOTOR_LB);

/***
 * Assignment 2
 * 
 * Add logic to set the PWM based on postiive/negative number.
 ***/

void Motors::setMotorPwm(int motor, float pwm) {
    if (abs(pwm) > MAX_SPEED) {
        if (pwm > 0) pwm = MAX_SPEED;
        else if (pwm < 0) pwm = -MAX_SPEED;
    }
    else if (abs(pwm) < MIN_SPEED) {
        if (pwm > 0) pwm = MIN_SPEED;
        else if (pwm < 0) pwm = -MIN_SPEED;
    }
    
    // TODO
    // Use the "PwmOut" objects defined above
    // Hint: Stop your backwards/forward motor before going forward/backwards.
    if (pwm > 0) {
        if (motor == RIGHT_MOTOR) {
            // TODO
        } else {
            // TODO
        }
    } else {
        if (motor == RIGHT_MOTOR) {
            // TODO
        } else {
            // TODO
        }
    }
}

Motors::Motors() {
    // DO NOT initialize PWM!!! It breaks the mouse.
}

void Motors::stop() {
    setRightPwm(0);
    setLeftPwm(0);   
}

void Motors::setRightPwm(float pwm) {
    m_rpwm = pwm;
    setMotorPwm(RIGHT_MOTOR, pwm);
}

void Motors::setLeftPwm(float pwm) {
    m_lpwm = pwm;
    setMotorPwm(LEFT_MOTOR, pwm);
}

