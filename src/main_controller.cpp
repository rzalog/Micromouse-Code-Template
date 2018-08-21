#include <string>

#include "main_controller.h"
#include "globals.h"
#include "move_controller.h"
#include "straighten_controller.h"
#include "Floodfill.h"
#include "beep.h"

MainController::MainController() {
    this->whichPID = NONE;
    this->m_moveController = new MoveController;
    this->m_straightenController = new StraightenController;
    this->mouse = new Floodfill;
    
    this->straighten_every_wall = false;
    this->straighten_turn_around = false;
}

MainController::~MainController() {
    led2.toggle();
    delete m_moveController;   
}

void MainController::update() {
    switch(whichPID) {
        case DRIVE_STRAIGHT:
        case TURN:
            m_moveController->update();
            break;
        case STRAIGHTEN:
            m_straightenController->update();
            break;
        case NONE:
        default:
            break;
    };
}

void MainController::floodfillOneCell() {
    mouse->resetFloodfill();
    char next = mouse->nextMove();
    
    int counter = 1;
    
    bool going_back = false;
    
    while (next != BACK_TO_START) {
        pc.printf("move %d is %c\n", counter++, next);
        if (next == FORWARD) {
            if (!shortWall()) {
                this->moveOneCell();
            }
        }
        else if (next == RIGHT) {
            if (rightWallOpening()) {
                this->turnRight();
            }
        }        
        else if (next == LEFT) {
            if (leftWallOpening()) {
                this->turnLeft();
            }
        }
        else if (next == TURN_AROUND) {
            if (shortWall()) {
                this->straighten();   
            }
            
            if (isRightWall()) {
                this->turnRight();
                this->straighten();
                this->turnRight();   
            }
            else if (isLeftWall()) {
                this->turnLeft();
                this->straighten();
                this->turnLeft();   
            }
            else {
                this->turnAround();   
            }
        }
        else if (next == DONE) {
            pc.printf("DONE TO CENTER\n");
            going_back = true;
            //beep_long();
            wait(1);
            //beep();
        }
        else if (next == BACK_TO_START) {
            pc.printf("got back to start\n");
        }
        
        this->addWalls();
        if (shortWall()) {
            this->straighten();
        }

        next = mouse->nextMove(going_back);
        if (next == WAIT) {
            next = mouse->nextMove(going_back);
            if (next == WAIT) {
                pc.printf("wait twice done\n");
                beep_long();
                return;
            }
        }
    }
    
    pc.printf("BACK TO START\n");
}

void MainController::floodfillSteady() {
    mouse->resetFloodfill();
    char next = this->mouse->nextMove();
    
    bool is_going_back = false;
    
    float check = next_cell_check_distance;
    int counter = 1;
    this->go();
    while (next != BACK_TO_START) {
        pc.printf("move %d is %c\n", counter++, next);
                
        while (this->cells() < check)
            ;
        float start_cell = this->cells();

        float start = this->cells();
        
        // we reached new check
        this->addWallsSteady();
        
        next = this->mouse->nextMove(is_going_back);
        if (next == WAIT) {
            next = this->mouse->nextMove(is_going_back);
            if (next == WAIT) {
                pc.printf("wait twice... we're done here\n");
                return;   
            }
        }
        
        float diff = this->cells() - start;
                
        float move_to_next_val = 180 * (1 - next_cell_check_distance) - diff;
        move_to_next_val *= move_next_scaling;
        
        if (next == FORWARD) {
            check += 1; 
        }
        else if (next == RIGHT || next == LEFT || next == TURN_AROUND) {
            //beep();
            this->moveDistance(move_to_next_val);
            //beep();
            
            if (shortWall()) {
                this->straighten();   
            }
            
            // turning is complicated
            if (next == RIGHT) {
                this->turnRight();
            }
            else if (next == LEFT) {
                this->turnLeft();
            }
            else if (next == TURN_AROUND) {
                if (this->straighten_every_wall || this->straighten_turn_around) {
                    if (isRightWall()) {
                        this->turnRight();
                        this->straighten();
                        this->turnRight();
                    }   
                    else if (isLeftWall()) {
                        this->turnLeft();
                        this->straighten();
                        this->turnLeft();   
                    }
                    else {
                        this->turnAround();
                    }
                }
                else {
                    this->turnAround();
                }
            }
            pc.printf("start stopping at %f, finish at %f\n", this->cells());
            pc.printf("NEXT MOVE DIST %f\n", move_to_next_val); 
            
            this->go();
            
            // You need to get the "F" out of the queue, b/c you already
            // know you do that every time you turn
            next = this->mouse->nextMove(is_going_back);
            if (next == WAIT) {
                next = this->mouse->nextMove(is_going_back);   
            }
            check = next_cell_check_distance;
        }
        else if (next == DONE) {
            //this->moveDistance(move_to_next_val);
            is_going_back = true;
            pc.printf("GOT TO CENTER\n");
            //this->go();
        }
    }
    
    pc.printf("BACK TO START\n");
}

void MainController::resetFFPos() {
    this->mouse->resetFloodfill();   
}

void MainController::speedPath(string path) {
    int index = 0;
    while (index != path.size()) {
        char move = path[index];
        pc.printf("move: %c\n", move);
        index++;
        
        if (move == 'R') {
            this->turnRight(); 
        }
        else if (move == 'L') {
            this->turnLeft(); 
        }
        else if (move == 'T') {
            this->turnAround();
        }
        else {
            int distance = move - '0';
            this->moveDistance(180 * distance);
        }
        
        if (shortWall()) {
            this->straighten();   
        }
    }   
}

void MainController::addWalls() {
    if (shortWall()) {
        this->mouse->addWallFront();
    }
    else {
        this->mouse->removeWallFront();   
    }

    if (!leftWallOpening()) {
        this->mouse->addWallLeft();
    }
    else {
        this->mouse->removeWallLeft();   
    }
    
    if (!rightWallOpening()) {
        this->mouse->addWallRight();
    }
    else {
        this->mouse->removeWallRight();
    }
}

void MainController::addWallsSteady() {
//    ir.flashDiag();
//    systick.wait(0.001);
    if (longWall()) {
        //pc.printf("long wall\n");
        this->mouse->addWallFront();
    }

    if (!leftWallOpening()) {
        //pc.printf("left diag\n");
        this->mouse->addWallLeft();
    }
    
    if (!rightWallOpening()) {
        //pc.printf("right diag\n");
        this->mouse->addWallRight();
    }
}

void MainController::go() {
    // used in floodfill
    this->setCellsToMove(50);   
}

void MainController::driveStraight() {
    whichPID = DRIVE_STRAIGHT;
    m_moveController->reset();
    
    m_moveController->setUseEncoders(true);
    m_moveController->setUseIR(true);
    
    float cells_to_move = 30;
    float nextCellCheck = 1;
    this->setCellsToMove(cells_to_move);
    while (!m_moveController->isDone()) {
        if (m_moveController->cells() > nextCellCheck) {
            // New cell
            nextCellCheck++;
            
            if (longWall()) {
                this->moveCells(1);
                this->straighten();
                this->turnAround();
                this->setCellsToMove(cells_to_move);
            }
        }
    }
}

void MainController::moveDistance(float mm) {    
    whichPID = DRIVE_STRAIGHT;
    m_moveController->setUseEncoders(true);
    m_moveController->setUseIR(true);
    m_moveController->reset();
    
    m_moveController->setCountsToMove(mm_to_count(mm));
    
    while (!m_moveController->isDone()) {
//        pc.printf("errorx: %f\terrorw: %f\tpwmx: %f\tpwmw: %f\trse: %f\tlse: %f\tence: %d\n",
//            m_moveController->m_posErrorX,
//            m_moveController->m_posErrorW,
//            m_moveController->m_pwmX,
//            m_moveController->m_pwmW,
//            ir.RS_error(),
//            ir.LS_error(),
//            encoders.getPulsesL() - encoders.getPulsesR()
//        );
        //systick.wait(0.1);
    }
    
    pc.printf("DONE MOVE x: %f\tidealx: %f\tdelta: %f\tw: %f\terrorx: %f\terrorw: %f\t\n",
        m_moveController->m_countsTravelledX,
        m_moveController->m_idealCountsTravelledX,
        m_moveController->m_idealCountsTravelledX - m_moveController->m_countsTravelledX,
        m_moveController->m_countsTravelledW,
        m_moveController->m_posErrorX,
        m_moveController->m_posErrorW
    );
}

void MainController::moveOneCell() {
    this->moveDistance(180);
}

void MainController::straighten() {
    m_straightenController->reset();
    
    whichPID = STRAIGHTEN;
    setUseIR(true);
    setUseEncoders(false);
    
    pc.printf("about to straighten...\n");
    while (!m_straightenController->isDone()) {
//        pc.printf("re: %f\tle: %f\trpwm: %f\tlpwm: %f\t\n",
//            m_straightenController->right_error,
//            m_straightenController->left_error,
//            m_straightenController->rpwm,
//            m_straightenController->lpwm
//        );
        //systick.wait(0.1);
    }
    
    pc.printf("DONE STRAIGHTEN re: %f\tle: %f\t\t\n",
        m_straightenController->right_error,
        m_straightenController->left_error
    );
    pc.printf("rf: %f\trfb: %f\tlf: %f\tlfb: %f\t\n",
        ir.RF(),
        ir.RF_base(),
        ir.LF(),
        ir.LF_base());
}

void MainController::setCellsToMove(float n) {
    m_moveController->reset();
    m_moveController->setCountsToMove(mm_to_count(n * 180));
    setUseEncoders(true);
    setUseIR(true);
    whichPID = DRIVE_STRAIGHT; 
}

float MainController::cells() {
    return m_moveController->cells();
}

void MainController::turn(int deg) {
    m_moveController->reset();
    m_moveController->setUseEncoders(true);
    m_moveController->setUseIR(false);
    
    int counts_per_turn = COUNTS_PER_TURN;
    int counts = (deg / 90) * counts_per_turn;
    
    whichPID = TURN;
    m_moveController->reset();
    
    m_moveController->setCountsToTurn(counts);
    
    while (!m_moveController->isDone()) {
        //systick.wait(0.1);
       // pc.printf("countsW: %f\tideal: %f\tpwmx: %f\tpwmw: %f\t\n",
//            m_moveController->m_countsTravelledW,
//            m_moveController->m_idealCountsTravelledW,
//            m_moveController->m_pwmX,
//            m_moveController->m_pwmW
//        );
    }
    
//    pc.printf("DONE: countsW: %f\tideal: %f\t\n",
//        m_moveController->m_countsTravelledW,
//        m_moveController->m_idealCountsTravelledW
//    );
}

void MainController::setUseEncoders(bool val) {
    m_moveController->setUseEncoders(val);
}

void MainController::setUseGyro(bool val) {
    m_moveController->setUseGyro(val);
}

void MainController::setUseIR(bool val) {
    m_moveController->setUseIR(val);
}

void MainController::goNoPwm() {    
    setUseEncoders(true);
    setUseIR(true);
    m_moveController->setAdjustPwm(false);
    m_moveController->reset();
    
    setCellsToMove(4);
    
    whichPID = DRIVE_STRAIGHT;
    
    while (!m_moveController->isDone()) {
        encoders.update();
        pc.printf("errorx: %f\terrorw: %f\tpwmx: %f\tpwmw: %f\trse: %f\tlse: %f\tence: %d\n",
            m_moveController->m_posErrorX,
            m_moveController->m_posErrorW,
            m_moveController->m_pwmX,
            m_moveController->m_pwmW,
            ir.RS_error(),
            ir.LS_error(),
            encoders.getPulsesL() - encoders.getPulsesR()
        );
        wait(1.0f/3);
    }
    
    pc.printf("errorx: %f\terrorw: %f\tpwmx: %f\tpwmw: %f\t\n",
        m_moveController->m_posErrorX,
        m_moveController->m_posErrorW,
        m_moveController->m_pwmX,
        m_moveController->m_pwmW
    ); 
}





/////////////
// Disregard
/////////////
//int calculateDecelerationDistance(float curSpeed, float dec) {
//    /*
//    Calculate the distance that will be travelled until a full stop if
//    moving at "curSpeed" and decelerating at "dec".
//    */
//    int distance = static_cast<int>( abs( (curSpeed * curSpeed) / (2 * decX) ) );
//    return distance;
//}
//
//void MainController::moveDistanceBad(float mm) {
//    float countsToTravel = mm_to_count(mm);
//    
//    pc.printf("\nGoing to move %.3f counts.\n", countsToTravel);
//    
//    whichPID = DRIVE_STRAIGHT;
//    m_moveController->reset();
//    
//    m_moveController->setTargetSpeedX(base_move_speed);
//    m_moveController->setTargetSpeedW(0);
//    m_moveController->setUseEncoders(true);
//    m_moveController->setUseGyro(true);
//    m_moveController->setUseIR(true);
//    
//    bool hasStartedDecelerate = false;
//    
//    int init = millis;
//    
//    do {
//        float distanceLeft = countsToTravel - m_moveController->idealCountsTravelledX();
//
////        pc.printf("Travelled: %d Left: %d CurSpeed: %.5f PwmX: %.3f PosErrorX: %.3f Ideal count: %.3f Ms: %d\n",
////                m_moveController->countsTravelled(), 
////                distanceLeft,
////                m_moveController->m_curSpeedX,
////                m_moveController->m_pwmX,
////                m_moveController->m_posErrorX,
////                m_moveController->m_idealCountsTravelled,
////                millis - init);
//        
//        if (!hasStartedDecelerate && calculateDecelerationDistance(m_moveController->m_curSpeedX, decX) > distanceLeft) {
//            m_moveController->setTargetSpeedX(0);
//            hasStartedDecelerate = true;
//            
//            pc.printf("START DECELERATING Distance left: %.3f CurSpeed: %.3f DecX: %.3f\n",
//                      distanceLeft,
//                      m_moveController->m_curSpeedX,
//                      decX);
//        }
//    } while (//m_moveController->countsTravelled() < counts &&
//                !m_moveController->isDone());
//                
//    pc.printf("All done moving %.3f counts (actual counts: %.3f, \"ideal\": %.3f) in %.3f (ideal: %.3f) actual pulse: %d\n",
//              countsToTravel,
//              m_moveController->countsTravelledX(),
//              m_moveController->idealCountsTravelledX(),
//              (millis - init) / 1000.0f,
//              (mm / 1000) / base_move_speed,
//              encoders.getPulsesR());
//    pc.printf("Calculated ideal - real ideal: %.3f\n", m_moveController->idealCountsTravelledX() - countsToTravel);
//        
//    led2.toggle();
//}
//
//void MainController::stop() {
//    m_moveController->setTargetSpeedX(0);
//    m_moveController->setTargetSpeedW(0);
//    
//    while (!m_moveController->isDone())
//        ;
//}


//void MainController::justGo() {
//    ir.frontOn();
//    while (1) {
//        pc.printf("back at it\n");
//        systick.wait(0.2);
//        setCellsToMove(30);
//        
//        int counterWhole = 0;
//        int counterHalf = 0.5;
//        bool cont = true;
//        while (cont) {
//            if (m_moveController->cells() > counterWhole) {
//                // new cell bitches
//                counterWhole++;
//                if (longWall()) {
//                    //pc.printf("found a long wall rf: %f\tlf: %f\t\n", ir.RF(), ir.LF());
//                    cont = false;
//                    this->moveDistance(180);
//                    this->straighten();
//                    if (rightWallOpening()) {
//                        turnRight();
//                        this->moveDistance(180);
//                    }
//                    else if (leftWallOpening()) {
//                        turnLeft();
//                        this->moveDistance(180);
//                    }
//                    else {
//                        turnAround();   
//                    }
//                }
//                else {
//                    if (leftDiagOpening()) {
//                        //pc.printf("found a left turn\n");
//                        this->moveDistance(180);
//                        if (leftWallOpening()) {
//                            if (encoders.getPulsesR() % 2 == 0) {
//                                turnLeft();
//                                this->moveDistance(180);
//                            }
//                        }
//                        else {
//                            //pc.printf("no left ls: %f\n", ir.LS());   
//                        }
//                        cont = false;
//                    }
//                    else if (rightDiagOpening()) {
//                        //pc.printf("found a right turn\n");
//                        this->moveDistance(180);
//                        if (rightWallOpening()) {
//                            if (encoders.getPulsesR() % 2 == 0) {
//                                turnRight();
//                                this->moveDistance(180);
//                            }
//                        }
//                        else {
//                            //pc.printf("no right rs: %f\n");   
//                        }
//                        cont = false;
//                    }
//                }                
//            }
//            
//            if (m_moveController->cells() > counterHalf) {
//                counterHalf += 1;   
//                
//                ir.flashDiag();
//
//            }
//        }
//    }
//}

//void MainController::moveForward(int cells) {
//    float cellsLeft = cells;
//    
//    pc.printf("moving forward %f cells\n", cellsLeft);
//    
//    whichPID = DRIVE_STRAIGHT;
//    while (cellsLeft > 1 && !longWall()) {
//        this->setCellsToMove(cellsLeft);
//        
//        while (this->m_moveController->cells() < 1)
//            ;
//        
//        float start = this->m_moveController->cells();
//        mouse->moveForward();
//        this->addWalls();
//        pc.printf("getting path...\n");
//        string path = getPath();
//        pc.printf("new path: %s\n", path.c_str());
//        float diff = this->m_moveController->cells() - start;
//        
//        char move = path[0];
//        if (move == 'R' || move == 'L' || move == 'T') {
//            cellsLeft = 0;
//        }
//        else {
//            cellsLeft = (path[0] - '0') - diff;
//        }
//    }
//    
//    pc.printf("done with the loop\n");
//    this->moveDistance(180 * cellsLeft);
//    this->mouse->moveForward();
//    this->addWalls();
//    pc.printf("done moving forward\n");
//}

//void MainController::oneStep() {
//    while (1) {
//        int n = encoders.getPulsesR() % 4;
//        
//        if (!shortWall() && n == 1 || n == 2) {
//            this->moveDistance(180);
//        }
//        else if (leftWallOpening() && n == 3 || n == 4) {
//            this->turnLeft();
//            this->moveDistance(180);
//        }
//        else if (rightWallOpening()) {
//            this->turnRight();
//            this->moveDistance(180);
//        }
//        else if (!shortWall()) {
//            this->moveDistance(180);
//        }
//        else if (leftWallOpening()) {
//            this->turnLeft();
//            this->moveDistance(180);
//        }
//        else {
//            this->turnAround();   
//        }
//        
//        if (shortWall()) {
//            //this->straighten();
//        }
//    }   
//}

