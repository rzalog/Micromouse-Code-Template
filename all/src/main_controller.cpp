#include <string>

#include "main_controller.h"
#include "pid_controller.h"

MainController::MainController() {
    m_pid = new PIDController;
}

MainController::~MainController() {
    delete m_pid;
}

/***
 * Assignment 3
 * 
 * Implement PID! Most of this work is done in "pid_controller".
 ***/
void MainController::update() {
    /****
     * Every systick, what do you want to do?
     * Hint: Probably update your PID controller :)
     *****/
}

/***
 * Assignment 3: Part 1
 * 
 * Get your mouse driving straight.
 ***/
void MainController::driveStraight() {
    /**
     * Set an X goal and W goal correctly! This is discussed in the lecture slides.
     * For help, see example code: pid/drive-straight.cpp
     **/
}

/***
 * Assignment 3: Part 2
 * 
 * Get your mouse to turn, and drive a specific distance.
 ***/
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