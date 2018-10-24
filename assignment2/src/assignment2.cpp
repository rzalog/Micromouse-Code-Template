#include "assignment2.h"
#include "globals.h"


void read_encoders() {
    while (true) {
        encoders.printValues();
        wait(0.5);
    }
}

void basic_motor_movement() {
    // TODO: Move forwards

    wait(3);

    // TODO: Move backwards

    wait(3);

    // TODO: Turn
}