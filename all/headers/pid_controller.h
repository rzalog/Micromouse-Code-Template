#pragma once

#include "sensors.h"
#include "globals.h"

float KpX = 1;
float KdX = 1;

float KpW = 1;
float KdW = 1;

class MoveController {
public:
    MoveController();
    
    void reset() volatile;
    void update() volatile;

    void setXGoal(int counts);
    void setWGoal(int counts);

    bool isDone() volatile;
private:
    void getSensorFeedback() volatile;
    float x_controller() volatile;
    float w_controller() volatile;
    void updateMotorPwm() volatile;
};