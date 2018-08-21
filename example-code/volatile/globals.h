#pragma once

/*
Notes:
- Don't forget the "extern"
- Use volatile keyword
*/

// Instance of user-defined volatile class
extern volatile Encoders encoders;

/*
A normal variable that you want to be volatile. This one represents
the total number of milliseconds that have passed since you started
your program. We need it to be volatile because it will be modified
in systick.
*/
extern volatile int millis;

extern Systick systick;
extern Serial pc;