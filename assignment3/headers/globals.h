#pragma once

#include "mbed.h"
#include "led.h"
#include "motors.h"
#include "encoders.h"
#include "systick.h"
#include "main_controller.h"

extern Led led;
extern Serial pc;

extern Systick systick;
extern volatile unsigned int millis;

extern Motors motors;
extern MainController mainController;

extern volatile Encoder encoders;