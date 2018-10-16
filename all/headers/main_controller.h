#pragma once

class PIDController;

class MainController {
public:
    MainController();
    ~MainController();
    
    void update();
    
    void driveStraight();
    void turn(int deg);
    void moveCells(float n);

    void stop();
private:
    PIDController* m_pid;
};