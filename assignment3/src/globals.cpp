#include "globals.h"
#include "mbed.h"
#include "led.h"
#include "pins.h"

#include "led.h"
#include "systick.h"
#include "motors.h"
#include "main_controller.h"
#include "encoders.h"

Led led(LED);
Serial pc(TX, RX);

Systick systick;
volatile unsigned int millis = 0;

Motors motors;
MainController mainController;

volatile Encoder encoders;