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
    while (true) {
        int counter = 1;
        pc.printf("Hello, World %d!", counter);
        counter++;
        wait(1);
    }
}