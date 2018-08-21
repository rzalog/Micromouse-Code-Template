#include "modes.h"
#include "sensors.h"
#include "beep.h"
#include "globals.h"

int cur_mode = 0;

void set_mode() {
    led1.on();
    
    int count = encoders.getPulsesR();
    int increment = 2000;
    
    while (1) {        
        int p = encoders.getPulsesR();
        
        if (p - count < 0) {
            count = p;
        }
        else if (p - count > increment * (MAX_MODES - 1)) {
            count = p - increment * (MAX_MODES - 1);
        }
        
        for (int i = 0; i < MAX_MODES; i++) {
            int val = p - count;
            int mode = i + 1;
            if (val >= increment * (mode - 1) && val <= increment * (mode)) {
                if (cur_mode != mode) {
                    beeps(mode);
                }
                cur_mode = mode;
                break;
            }
        }
        
        if (veryShortWall()) {
            led1.off();
            return;
        }
    }
}