#include "motors.h"
#include "mbed.h"
#include "pins.h"

PwmOut m_RF(MOTOR_RF);
PwmOut m_RB(MOTOR_RB);
PwmOut m_LF(MOTOR_LF);
PwmOut m_LB(MOTOR_LB);

Motors::Motors() {
    // DO NOT initialize PWM!!! It breaks the mouse.
}

void Motors::startBaseSpeed() {
    setRightPwm(BASE_SPEED);
    setLeftPwm(BASE_SPEED);
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

void Motors::incrementPwm(float pwm) {
    float new_rpwm, new_lpwm;
    new_rpwm = BASE_SPEED + pwm;
    new_lpwm = BASE_SPEED - pwm;
    setRightPwm(new_rpwm);
    setLeftPwm(new_lpwm);
}

void Motors::setMotorPwm(int motor, float pwm) {
    if (abs(pwm) > MAX_SPEED) {
        if (pwm > 0) pwm = MAX_SPEED;
        else if (pwm < 0) pwm = -MAX_SPEED;
    }
    else if (abs(pwm) < MIN_SPEED) {
        if (pwm > 0) pwm = MIN_SPEED;
        else if (pwm < 0) pwm = -MIN_SPEED;   
    }
    
    pwm /= PWM_SCALE;
    
    if (pwm > 0) {
        if (motor == RIGHT_MOTOR) {
//            m_RB = 0;
//            m_RF = pwm;
            m_RB = pwm;
            m_RF = 0;  
        } else {
            m_LB = 0;
            m_LF = pwm;
        }
    } else {
        if (motor == RIGHT_MOTOR) {
//            m_RF = 0;
//            m_RB = abs(pwm);
            m_RF = abs(pwm);
            m_RB = 0;
        } else {
            m_LF = 0;
            m_LB = abs(pwm);
        }
    }
}