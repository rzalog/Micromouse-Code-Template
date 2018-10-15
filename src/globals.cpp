#include "globals.h"
#include "mbed.h"
#include "led.h"
#include "pins.h"


Led led(LED);
Logging logger;

Serial pc(TX, RX);

Systick systick;
volatile unsigned int millis = 0;

Motors motors;
MainController mainController;

volatile Encoder encoders;
volatile Gyro gyro;
volatile IR ir;