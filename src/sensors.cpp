#include "sensors.h"
#include "pins.h"
#include "globals.h"

///////
// Global variables
///////

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

AnalogIn gyroPin(GYRO);
AnalogIn vrefPin(VREF);

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
    m_countR = -wheel_R.getPulses() * RIGHT_ENCODER_FIX;
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
    
    front_ON = false;
    diag_on = false;
    front_flash = true;
    diag_flash = true;
}

float IR::flash_ir(DigitalOut ir, AnalogIn rec) volatile {
    ir = 1;
    wait_us(WARM_UP_US);
    float ret = read_ir(rec);
    ir = 0;
    wait_us(COOL_DOWN_US);
    
    return ret;
}

float IR::read_ir(AnalogIn rec) volatile {
    int n = 10;
    float total = 0;
       
    for (int i = 0; i < n; i++) {
        total += rec.read();   
    }
    
    return total / n;
}

void IR::update() volatile {
    float init_rf, init_lf, init_rs, init_ls;
        
    init_rf = rec_RF.read();
    init_lf = rec_LF.read();
    init_rs = rec_RS.read();
    init_ls = rec_LS.read();

//    if (front_ON || front_flash) {
        m_rf = adjust_reading(flash_ir(ir_RF, rec_RF) - init_rf);
        m_lf = adjust_reading(flash_ir(ir_LF, rec_LF) - init_lf);
        front_flash = false;
//    }
    
    m_rs = adjust_reading(flash_ir(ir_RS, rec_RS) - init_rs);
    m_ls = adjust_reading(flash_ir(ir_LS, rec_LS) - init_ls);
    
    if (diag_flash || diag_on) {
        updateDiag();
        diag_flash = false;   
    }
}

void IR::updateDiag() volatile {
    float init_rd, init_ld;

    init_rd = rec_RD.read();
    init_ld = rec_LD.read();

    m_rd = adjust_reading(flash_ir(ir_RD, rec_RD) - init_rd);
    m_ld = adjust_reading(flash_ir(ir_LD, rec_LD) - init_ld);
}

void IR::flashDiag() volatile {
    diag_flash = true;   
}

void IR::flashFront() volatile {
    front_flash = true;
}

void IR::flashBoth() volatile {
    this->flashDiag();
    this->flashFront();   
}

void IR::setFrontBase() volatile {
    ir.update();
    m_rf_base = m_rf;
    m_lf_base = m_lf;   
}

void IR::setSideBase() volatile {
    ir.update();
    m_rs_base = m_rs;
    m_ls_base = m_ls;   
}

float IR::RS_error() volatile {
    if (RS() > MIDR) {
        return RS_base() - RS();//RS() - RS_base();
    }
    else return 0;
}

float IR::LS_error() volatile {
    if (LS() > MIDL) {
        return LS() - LS_base();//LS_base() - LS();
    }
    else return 0;
}

void IR::printValues() volatile {
    logger.printf("RF: %.5f\tLF: %.5f\tRD: %.5f\tLD: %.5f\tRS: %.5f\tLS: %.5f\n",
        m_rf, m_lf, m_rd, m_ld, m_rs, m_ls);
}

float IR::adjust_reading(float reading) volatile {
//    if (reading < 1 / INF_IR_READING) {
//        return INF_IR_READING;
//    }
    
    float val = reading * 100.0f;    
    return val;
}

///////
// Gyro implementation
///////

Gyro::Gyro() {
    m_theta = 0;
    m_curAngularVelocity = 0;
    
    m_correction = GYRO_DRIFT_CORRECTION;
}

void Gyro::update(float dt) volatile {
    float sample = 0.0;
    int num_samples = 20;
    for (int i = 0; i < num_samples; i++) {
        sample += this->rawReading();
    }
    sample /= num_samples;

    m_curAngularVelocity = sample - m_correction;
    
    m_theta += m_curAngularVelocity * dt;
}

float Gyro::angularVelocity() volatile {
    return m_curAngularVelocity;
}

float Gyro::theta() volatile {
    return m_theta;
}

void Gyro::reset() volatile {
    m_theta = 0;   
}

void Gyro::setDrift() volatile {
    m_correction = GYRO_DRIFT_CORRECTION;
}

float Gyro::getDrift() volatile {
    int n = 10000;
    volatile float accum = 0;

    for (int i = 0; i < n; i++) {
        if (i >= n / 2)
            accum += rawReading();
    }
    
    return accum / float(n/2);
}

void Gyro::printValues() volatile {
    logger.printf("Theta: %.5f\tW: %.5f\tRaw: %.5f\n", m_theta, m_curAngularVelocity, rawReading());    
}

float Gyro::rawReading() volatile {
    float voltage = 3.3f * static_cast<float>(gyroPin.read());
    
    float vref = 1.5f;
    
    return (voltage - vref) * 1500.0f;
}

///////
// Aux functions
///////

bool veryShortWall() {
    return ir.RF() > VSHORT_RIGHT || ir.LF() > VSHORT_LEFT;   
}

bool shortWall() {
    return ir.RF() > SHORT_FRONT_WALL_THRESHOLD_RF && ir.LF() > SHORT_FRONT_WALL_THRESHOLD_LF;  
}

bool longWall() {
    return ir.RF() > LONG_FRONT_WALL_THRESHOLD_RF && ir.LF() > LONG_FRONT_WALL_THRESHOLD_LF;  
}

bool leftDiagOpening() {
    return ir.LD() < DIAG_LEFT;   
}

bool rightDiagOpening() {
    return ir.RD() < DIAG_RIGHT;
}

bool leftWallOpening() {
    return ir.LS() < WALL_LEFT;   
}

bool rightWallOpening() {
    return ir.RS() < WALL_RIGHT;   
}