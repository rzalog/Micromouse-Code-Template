#pragma once

const float LEFT_SPEED_FACTOR = 0.97;

const float PWM_SCALE = 100.0f;

const float MIN_SPEED = .085f * PWM_SCALE;
const float MAX_SPEED = 0.375f * PWM_SCALE;
const float BASE_SPEED = 0.12f * PWM_SCALE;

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
    void incrementPwm(float pwm);
private:
    void setMotorPwm(int motor, float pwm);
    
    float m_rpwm;
    float m_lpwm;
};