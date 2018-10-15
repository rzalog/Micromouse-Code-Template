#include "run_modes.h"
#include "sensors.h"
#include "globals.h"
#include "main_controller.h"
#include "move_controller.h"
#include "modes.h"
#include "pins.h"

#include "logging.h"

void blinky_led() {
    while (true) {
        /***
         * Assignment 1
         * Ensure that you are using the correct pin for the LED,
         * and then toggle the LED back and forth.
         ***/

        led.toggle();
        wait(0.5);
    }
}

void hello_world() {
    /***
     * Assignment 1
     * Find and assign the correct TX, RX pins in "pins.h" to use 
     * the serial PC. Get the output to show up on your computer.
     ***/
    
    while (true) {
        int counter = 1;
        pc.printf("Hello, World %d!", counter);
        counter++;
        wait(1);
    }
}