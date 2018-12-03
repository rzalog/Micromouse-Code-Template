float get_ir_error() {
    float error;

    if (ir.right() > MIDR) {
        error = ir.right() - MIDR;
    }
    else if (ir.left() > MIDL) {
        // Make sure to change the sign for the error!
        error = MIDL - ir.left();
    }
}

void w_controller() {
    // once again, make sure these signs match up!
    float w_error = m_countsW + get_ir_error();

    /* ... the rest of the W controller code ... */
}