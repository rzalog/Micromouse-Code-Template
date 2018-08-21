#pragma once

#include "mbed.h"
#include "led.h"
#include "beep.h"
#include "motors.h"
#include "sensors.h"
#include "systick.h"
#include "main_controller.h"
#include "logging.h"

extern Serial pc;

extern Led led1;
extern Led led2;
//extern Led led3; these are bad
//extern Led led4;
extern Led led5;
extern Led led6;
extern Led led7;
extern Led led8;

//extern Beep buzzer;

extern Logging logger;

extern Systick systick;
extern volatile unsigned int millis;

extern Motors motors;
extern MainController mainController;

extern volatile Encoder encoders;
extern volatile Gyro gyro;
extern volatile IR ir;