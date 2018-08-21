#include "globals.h"

// If your volatile class has a constructor that takes arguments,
// pass them in here
volatile Encoders encoders;

// Can assign your default values here
volatile int millis = 0;

Systick systick;
Serial pc;