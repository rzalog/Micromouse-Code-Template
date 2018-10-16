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
 * Add logic to limit the min and max speeds,
 * and actually setting the PWM.
 ***/

void Motors::setMotorPwm(int motor, float pwm) {
    if (abs(pwm) > MAX_SPEED) {
        // TODO
    }
    else if (abs(pwm) < MIN_SPEED) {
        // TODO
    }
    
    //  Turn 0-100 scale to 0.0-1.0 scale
    pwm /= PWM_SCALE;
    
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

