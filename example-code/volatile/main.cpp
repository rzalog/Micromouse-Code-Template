#include "mbed.h"
#include "globals.h"
#include "encoders.h"
#include "systick.h" // Assume this exists

int main() {
    systick.start();

    while (True) {
        /*
        Here, the systick is running and doing important things,
        like updating encoder values, etc. Without volatile,
        none of these changes will be reflected in the main loop,
        so the same values would print over and over. But with 
        volatile, values will correctly update.
        */
        pc.printf("Time: %d, right: %d, left: %d\n",
            millis,
            encoders.right(),
            encoders.left());
    }
}