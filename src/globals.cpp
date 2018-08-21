#include "globals.h"
#include "mbed.h"
#include "led.h"
#include "pins.h"
#include "beep.h"

// Misc
Serial pc(TX, RX);
Logging logger;
Systick systick;
// Beep buzzer(BUZZER);
volatile unsigned int millis = 0;

// Leds
Led led1(LED1);
Led led2(LED2);
Led led3(LED3);
Led led4(LED4);
Led led5(LED5);
Led led6(LED6);
Led led7(LED7);
Led led8(LED8);

Motors motors;
MainController mainController;

// Sensors
volatile Encoder encoders;
volatile Gyro gyro;
volatile IR ir;
