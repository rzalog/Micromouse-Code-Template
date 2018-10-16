#pragma once

#include "mbed.h"
#include "led.h"
#include "motors.h"
#include "sensors.h"
#include "systick.h"
#include "main_controller.h"
#include "logging.h"

extern Led led;
extern Logging logger;

extern Serial pc;

extern Systick systick;
extern volatile unsigned int millis;

extern Motors motors;
extern MainController mainController;

extern volatile Encoder encoders;
extern volatile Gyro gyro;
extern volatile IR ir;