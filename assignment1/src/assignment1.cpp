#include "assignment1.h"
#include "globals.h"

/***
 * This will just work, and can be used to just see that
 * you have the template working correctly.
 ***/
void blinky_led() {
    while (true) {
        led.toggle();
        wait(0.5);
    }
}

/***
 * Find, assign, and connect the correct TX, RX pins in "pins.h" to use 
 * the serial PC. Get the output to show up on your computer.
 ***/
void hello_world() {
    int counter = 1;
    while (true) {
        pc.printf("Hello, World %d!\n", counter);
        counter++;
        wait(1);
    }
}