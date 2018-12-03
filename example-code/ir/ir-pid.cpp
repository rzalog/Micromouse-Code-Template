float get_ir_error() {
    float error;

    /*
    - These MID values can be hardcoded, or maybe even calculated at some
    point in your program...
    - You may find that setting these mid values to something that doesn't
    quite correspond to the middle (e.g., MIDR slightly to the right of the
    middle rather that directly in the center), may make your Rat perform
    better. Play around with it!
    */  
    if (ir.right() > MIDR) {
        error = ir.right() - MIDR;
    }
    else if (ir.left() > MIDL) {
        // Make sure to change the sign for the error!
        error = MIDL - ir.left();
    }
    else {
        // No useful information from IR's
        error = 0;
    }

    return error;
}

void w_controller() {
    // once again, make sure these signs match up!
    float w_error = m_countsW + get_ir_error();

    /* ... the rest of the W controller code ... */
}