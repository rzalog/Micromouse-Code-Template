#include "globals.h"
#include "mbed.h"
#include "led.h"
#include "pins.h"

Led led(LED);
Serial pc(TX, RX);

Motors motors;