#include <string>

#include "main_controller.h"
#include "pid_controller.h"

MainController::MainController() {
    m_pid = new PIDController;
}

MainController::~MainController() {
    delete m_pid;   
}

void MainController::update() {
    /****
     * Every systick, what do you want to do?
     * Hint: Probably update your PID controller :)
     *****/
}

void MainController::driveStraight() {
    /**
     * Set goals! This is discussed in the lecture slides.
     * For help, see example code: pid/drive-straight.cpp
     **/
}

void MainController::turn(int deg) {
    /**
     * For help, see example code: pid/turning.cpp
     **/
}

void moveCells(float n) {
    /**
     * For help, see example code: pid/full.cpp
     **/
}