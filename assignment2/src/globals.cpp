#include "globals.h"
#include "mbed.h"
#include "pins.h"

#include "led.h"
#include "motors.h"
#include "encoders.h"

Led led(LED);
Serial pc(TX, RX);

Motors motors;
Encoder encoders;