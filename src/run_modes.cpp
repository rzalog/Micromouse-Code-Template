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
        led.toggle();
        wait(0.5);
    }
}