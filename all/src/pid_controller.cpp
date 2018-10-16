#include "pid_controller.h"
#include "globals.h"

PIDController::PIDController() {
    this->reset();
}

/***
 * Assignment 3
 * 
 * Implement PID!
 ***/

void PIDController::reset() volatile {    
    /**
     * You'll have a lot to keep track of. Make sure to reset all 
     * those variables here.
     **/
}

void PIDController::update() volatile {
    /**
     * Update your PID controller. This is what should be called
     * every systick, so you should call all your important
     * functions here.
     **/
}

void PIDController::setXGoal() {
    /**
     * Set goal for X controller.
     **/
}

void PIDController::setWGoal() {
    /**
     * Set goal for W controller. Make sure to associate a turning
     * direction with a sign, e.g., positive is clockwise, negative
     * is counter-clockwise.
     **/
}

bool PIDController::isDone() {
    /**
     * When is the PID done? Well, probably when you've reached
     * your goals...
     **/
    return false;
}

/**
 * Private functions to do the internal work for PID.
 **/
void PIDController::getSensorFeedback() {
    /**
     * Update sensor values, from encoders, IR's, and gyro
     **/
}

float PIDController::x_controller() {
    /**
     * Your X PID controller, returns pwm_x
     **/
}

float PIDController::w_controller() {
    /**
     * Your W PID controller, returns pwm_w
     **/
}

void PIDController::updateMotorPwm() {
    /**
     * Takes pwm_x, pwm_w, and adjusts the motors based on those values.
     **/
}