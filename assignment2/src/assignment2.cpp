#include "assignment2.h"
#include "globals.h"

void basic_motor_movement() {
    // TODO: Move forwards

    wait(3);

    // TODO: Move backwards

    wait(3);

    // TODO: Turn
}

/***
 * The second function is done for you, but you will need to make
 * the encoders actually work and read the correct values, and verify
 * with the steps detailed in the check-off list.
 ***/
void read_encoders() {
    while (true) {
        encoders.printValues();
        wait(0.5);
    }
}