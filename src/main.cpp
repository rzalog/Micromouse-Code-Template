#include "beep.h"

#include "run_modes.h"
#include "globals.h"
#include "pins.h"

int main() {
    pc.printf("\n---\nThe program has started.\n---\n");
    led7.on();
    beep();
    wait(0.5);
    led7.off();

    testIRCharging();
}