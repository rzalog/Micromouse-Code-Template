#pragma once

#include <string>
#include "Floodfill.h"

class MoveController;
class StraightenController;

const float base_move_speed = 0.8f; // m/s

const int COUNTS_PER_TURN = 3090; //3161;

// This is the number of cells after the middle of a cell
// that the mouse checks the netx cell
const float next_cell_check_distance = 0.7f;
const float move_next_scaling = 1.0f;//1.02f / 1.05f;

enum WhichPID {
    NONE,
    DRIVE_STRAIGHT,
    TURN,
    STRAIGHTEN,
    DRIVE_STRAIGHT_ALT
};

const char FORWARD = 'F';
const char RIGHT = 'R';
const char LEFT = 'L';
const char TURN_AROUND = 'T';
const char DONE = 'D';
const char BACK_TO_START = 'S';
const char WAIT = 'W';
const char CONFUSION = '?';

class MainController {
public:
    MainController();
    ~MainController();
    
    void update();
    
    // Let's solve the maze
    void floodfillOneCell();
    void floodfillSteady();
    void resetFFPos();
    void addWalls();
    void addWallsSteady();
    void go();
    
    void speedPath(std::string path);
    
    void driveStraight();
    void stop();

    void moveCells(float n) { moveDistance(n * 180); }
    void moveDistance(float mm);
    void moveOneCell();
    void setCellsToMove(float n);
    float cells();

    void straighten();

    void turn(int deg);
    void turnRight() { turn(90); }
    void turnLeft() { turn(-90); }
    void turnAround() { turn(180); }

    void goNoPwm();
    
    void straightenEveryWallOn() { this->straighten_every_wall = true; }
    void straightenEveryWallOff() { this->straighten_every_wall = false; }
    void straightenTurnAroundOn() { this->straighten_turn_around = true; }
    void straightenTurnAroundOff() { this->straighten_turn_around = false; }

//private:
    void setUseEncoders(bool val);
    void setUseGyro(bool val);
    void setUseIR(bool val);
    
    MoveController* m_moveController;
    StraightenController* m_straightenController;
    Floodfill* mouse;
    WhichPID whichPID;
    
    bool straighten_every_wall;
    bool straighten_turn_around;
    
    // deprecated
};

int calculateDecelerationDistance(float curSpeed, float dec);