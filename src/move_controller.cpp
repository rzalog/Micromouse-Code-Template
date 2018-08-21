#include "move_controller.h"
#include "globals.h"

MoveController::MoveController() {
    this->reset();
    
    m_useEncoderFeedback = true;
    m_useGyroFeedback = false;
    m_useIRFeedback = true;
    
    m_adjustPwm = true;
}

void MoveController::reset() volatile {    
    m_rightEncoderOld = encoders.getPulsesR();
    m_leftEncoderOld = encoders.getPulsesL();
    m_rightEncoderChange = 0;
    m_leftEncoderChange = 0;
    
    m_posErrorX = 0;
    m_posErrorW = 0;
    m_oldPosErrorX = 0;
    m_oldPosErrorW = 0;
    
    m_pwmX = 0;
    m_pwmW = 0;
    
    m_stopPIDCounter = 0;
    
    m_idealCountsTravelledX = 0;
    m_idealCountsTravelledW = 0;
    m_countsTravelledX = 0;
    m_countsTravelledW = 0;
    
    m_PIDDone = false;
    m_isStartingToDecelerate = false;
    m_isStartingToAccelerate = false;
    
    m_cells = 0;
}

void MoveController::updateSensorFeedback() volatile {
    int right, left;
    right = encoders.getPulsesR();
    left = encoders.getPulsesL();
    
    m_rightEncoderChange = right - m_rightEncoderOld;
    m_leftEncoderChange = left - m_leftEncoderOld;
    
    m_rightEncoderOld = right;
    m_leftEncoderOld = left;
    
    m_countsTravelledX += (m_rightEncoderChange + m_leftEncoderChange) / 2;
    m_countsTravelledW += (m_leftEncoderChange - m_rightEncoderChange);
}

float MoveController::getIRFeedback() volatile {
    float error = 0;
    
    if (ir.RS() > MIDR) {
        error = ir.RS_error();
    }   
    else if (ir.LS() > MIDL) {
        error = ir.LS_error();
    }
    
    return error;
}

void MoveController::update() volatile {
    // Note: Positive W error is clockwise
    updateSensorFeedback();
    
    m_posErrorX = 0;
    m_posErrorW = 0;
    
    if (m_useEncoderFeedback) {
        m_posErrorX += m_idealCountsTravelledX - m_countsTravelledX;
        m_posErrorW = m_idealCountsTravelledW - m_countsTravelledW;
    }
    
    if (m_useIRFeedback) {
        m_posErrorW += getIRFeedback() * irScaling;
    }
    
    float kpx, kdx, kpw, kdw;
    if (this->is_turning) {
        kpx = KpX_turn;
        kdx = KdX_turn;
        kpw = KpW_turn;
        kdw = KdW_turn;
    }
    else {
        kpx = KpX;
        kdx = KdX;
        kpw = KpW;
        kdw = KdW;   
    }
    
    m_pwmX = (kpx * m_posErrorX + kdx * (m_posErrorX - m_oldPosErrorX));
    m_pwmW = (kpw * m_posErrorW + kdw * (m_posErrorW - m_oldPosErrorW));

    m_oldPosErrorX = m_posErrorX;
    m_oldPosErrorW = m_posErrorW;

    float tempx, tempw;
    tempx = m_pwmX;
    tempw = m_pwmW;
    this->adjustPwm(&tempx, &tempw);
    m_pwmX = tempx;
    m_pwmW = tempw;

    if (abs(m_pwmX + m_pwmW) <= MIN_SPEED && abs(m_pwmX - m_pwmW) <= MIN_SPEED) {
        m_stopPIDCounter++;
        //return;
    }
    
    if (m_stopPIDCounter < STOP_PID_THRESHOLD) {
        if (m_adjustPwm) {
            motors.setRightPwm(m_pwmX - m_pwmW);
            motors.setLeftPwm(m_pwmX + m_pwmW);
        }
    } else {
        motors.stop();
        m_PIDDone = true;
    }
    
    m_cells = m_countsTravelledX / COUNTS_FOR_CELL;
}

void MoveController::setCountsToMove(int counts) volatile {
    this->m_idealCountsTravelledX = counts;
    this->m_idealCountsTravelledW = 0;
}

void MoveController::setCountsToTurn(int counts) volatile {
    this->m_idealCountsTravelledX = 0;
    this->m_idealCountsTravelledW = counts;
}

void MoveController::adjustPwm(float* pwmx, float* pwmw) volatile {
    if (abs(*pwmx) > MAX_X_SPEED) {
        if (*pwmx > 0) *pwmx = MAX_X_SPEED;
        else *pwmx = -MAX_X_SPEED;
    }
    
    if (abs(*pwmw) > MAX_W_SPEED) {
        if (*pwmw > 0) *pwmw = MAX_W_SPEED;
        else *pwmw = -MAX_W_SPEED;
    }
}

void MoveController::setUseEncoders(bool val) volatile {
    m_useEncoderFeedback = val;
}

void MoveController::setUseGyro(bool val) volatile {
    m_useGyroFeedback = val;
}

void MoveController::setUseIR(bool val) volatile {
    m_useIRFeedback = val;   
}






//////////////
/// Disregard
//////////////

//void MoveController::updateOld() {
//    updateSensorFeedback();
//    updateCurrentSpeed();
//    updateMotorPwm();
//}
//
//void MoveController::updateCurrentSpeed() {
//    if(m_curSpeedX < m_targetSpeedX) {
//        if (m_isStartingToAccelerate) {
//            /*
//            We need this correction because otherwise the Reimann Sum estimation
//            is incorrect (discrete speeds vs. continuous acceleration).
//            */
//            m_curSpeedX += accX_correction;
//            m_isStartingToAccelerate = false;
//        }
//        
//        m_curSpeedX += accX;
//        
//        if(m_curSpeedX > m_targetSpeedX)
//            m_curSpeedX = m_targetSpeedX;
//    } else if(m_curSpeedX > m_targetSpeedX) {
//        if (m_isStartingToDecelerate) {
//            m_curSpeedX -= decX_correction;
//            m_isStartingToDecelerate = false;
//        }
//        
//        m_curSpeedX -= decX;
//        if(m_curSpeedX < m_targetSpeedX)
//            m_curSpeedX = m_targetSpeedX;
//    }
//    
//    m_idealCountsTravelledX += m_curSpeedX;
//    
//    if(m_curSpeedW < m_targetSpeedW) {
//        m_curSpeedW += accW;
//        if(m_curSpeedW > m_targetSpeedW)
//            m_curSpeedW = m_targetSpeedW;
//    } else if(m_curSpeedW > m_targetSpeedW) {
//        m_curSpeedW -= decW;
//        if(m_curSpeedW < m_targetSpeedW)
//            m_curSpeedW = m_targetSpeedW;
//    }
//    
//    m_idealCountsTravelledW += m_curSpeedW;
//}
//
//void MoveController::updateMotorPwm() {
//    int encoderFeedbackX, encoderFeedbackW;
//    float gyroFeedback;
//    int rotationalFeedback;
//    
//    rotationalFeedback = 0;
//        
//    //gyroFeedback = gyro.theta();
//    
//    if (m_useEncoderFeedback) {
//        rotationalFeedback += encoderFeedbackW;
//    }
//    if (m_useGyroFeedback) {
//        //rotationalFeedback += gyroFeedback * gyroScaling;
//    }
//    if (m_useIRFeedback) {
//        
//    }
//        
//    // completely determined by counts travelled
//    m_posErrorX = m_idealCountsTravelledX - m_countsTravelledX;
//    m_posErrorW = m_idealCountsTravelledW - m_countsTravelledW;
//    
//    m_pwmX = (KpX * m_posErrorX + KdX * (m_posErrorX - m_oldPosErrorX));
//    m_pwmW = (KpW * m_posErrorW + KdW * (m_posErrorW - m_oldPosErrorW));
//
//    m_oldPosErrorX = m_posErrorX;
//    m_oldPosErrorW = m_posErrorW;
//
//    if (abs(m_pwmX + m_pwmW) < MIN_SPEED) {
//        m_stopPIDCounter++;
//    }
//    
//    if (m_stopPIDCounter < STOP_PID_THRESHOLD) {
//        //m_stopPIDCounter = 0;
//        motors.setRightPwm(m_pwmX - m_pwmW);
//        motors.setLeftPwm(m_pwmX + m_pwmW);
//    } else {
//        m_posErrorX = 0;
//        m_posErrorW = 0;
//        motors.setRightPwm(0);
//        motors.setLeftPwm(0);
//        
//        m_PIDDone = true;
//    }
//}
//
//void MoveController::setTargetSpeedX(float speedX) {
//    float output = m_s_to_count_ms(speedX);
//    
//    m_isStartingToDecelerate = output < m_targetSpeedX;
//
//    m_targetSpeedX = output;          
//}
//
//void MoveController::setTargetSpeedW(float speedW) {
//    m_targetSpeedW = 0;
//}