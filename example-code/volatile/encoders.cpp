#include "encoders.h"

// Don't forget the volatile here as well!
void Encoders::update() volatile {
    this.right = right_wheel.get_pulses();
    this.left = left_wheel.get_pulses();
}

int Encoders::right() volatile {
    return this.right;
}

int Encoders::left() volatile {
    return this.left;
}