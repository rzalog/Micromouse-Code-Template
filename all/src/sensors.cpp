#include "sensors.h"
#include "pins.h"
#include "globals.h"

/***
 * Assignment 2
 * 
 * Import the QEI in order to have functioning encoders.
 ***/
QEI wheel_R(ENC_RA, ENC_RB, NC, 624, QEI::X4_ENCODING);
QEI wheel_L(ENC_LA, ENC_LB, NC, 624, QEI::X4_ENCODING);

AnalogIn rec_RF(DET_Rfront);
AnalogIn rec_LF(DET_Lfront);
AnalogIn rec_RD(DET_Rdiag);
AnalogIn rec_LD(DET_Ldiag);
AnalogIn rec_RS(DET_Rside);
AnalogIn rec_LS(DET_Lside);

DigitalOut ir_RF(IR_Rfront);
DigitalOut ir_LF(IR_Lfront);
DigitalOut ir_RD(IR_Rdiag);
DigitalOut ir_LD(IR_Ldiag);
DigitalOut ir_RS(IR_Rside);
DigitalOut ir_LS(IR_Lside);

///////
// Encoder class implementation
///////

Encoder::Encoder() {
    reset();
}

void Encoder::reset() volatile {
    m_countR = 0;
    m_countL = 0;
    wheel_R.reset();
    wheel_L.reset();
}

void Encoder::update() volatile {
    m_countR = wheel_R.getPulses();
    m_countL = wheel_L.getPulses();
}

int Encoder::getPulsesR() volatile {
    return m_countR;
}

int Encoder::getPulsesL() volatile {
    return m_countL;
}

void Encoder::printValues() volatile {
    pc.printf("R: %d\tL: %d\n", m_countR, m_countL);
}

///////
// IR class implementation
///////

IR::IR() {
    m_rf = 0;
    m_lf = 0;
    m_rd = 0;
    m_ld = 0;
    m_rs = 0;
    m_ls = 0;
}

/***
 * Assignment 4 START
 ***/

/***
 * Properly turn the IR's on, warm up receiver for WARM_UP_US,
 * read the value, cool down receivers for COOL_DOWN_US,
 * then turn off.
 ***/
float IR::flash_ir(DigitalOut ir, AnalogIn rec) volatile {
    float reading;

    // TODO
    
    return reading;
}

/***
 * You need to come up with these "MID" values,
 * which will allow you to use your IR's for PID.
 ***/
float IR::RS_error() volatile {
    // TODO

    if (RS() > MIDR) {
        return RS_base() - RS();
    }
    else return 0;
}

float IR::LS_error() volatile {
    // TODO

    if (LS() > MIDL) {
        return LS() - LS_base();
    }
    else return 0;
}

/***
 * Assignment 4 END
 ***/

float IR::read_ir(AnalogIn rec) volatile {
    // Read an IR, averaging over 10 reads.
    int n = 10;
    float total = 0;
       
    for (int i = 0; i < n; i++) {
        total += rec.read();   
    }
    
    return total / n;
}

void IR::update() volatile {
    // Update all your IR readings.

    float init_rf, init_lf, init_rs, init_ls;

    init_rf = rec_RF.read();
    init_lf = rec_LF.read();
    init_rs = rec_RS.read();
    init_ls = rec_LS.read();

    m_rf = flash_ir(ir_RF, rec_RF) - init_rf;
    m_lf = flash_ir(ir_LF, rec_LF) - init_lf;
    
    m_rs  = flash_ir(ir_RS, rec_RS) - init_rs;
    m_ls = flash_ir(ir_LS, rec_LS) - init_ls;
}


void IR::printValues() volatile {
    // Utility: Use for diagnostics

    logger.printf("RF: %.5f\tLF: %.5f\tRS: %.5f\tLS: %.5f\n",
        m_rf, m_lf, m_rd, m_ld, m_rs, m_ls);
}