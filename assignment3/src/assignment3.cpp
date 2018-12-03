#include "assignment3.h"

#include "globals.h"

/***
 * Assignment 3, Part A
 * 
 * PID! First get your mouse driving straight endlessly (you
 * will have to pick it up to stop it).
 * Then, get your mouse to turn.
 * 
 * You should not be modifying any of the following
 * functions! They are essentially test cases. If your rat does
 * not achieve the specified behavior, then something is wrong
 * in either your "mainController" class or your
 * "pid_controller" class.
 * 
 * Part 1: "test_systick". Again, do not modify this function.
 * Instead, modify your systick function to use the "logger"
 * object to "log" (NOT print!) messages, which will be printed
 * every 100ms. You'll also need to initialize your systick
 * properly by modifying its "start()" function.
 * 
 * Part 2: "drive_straight". Your mouse should drive straight
 * forever, until you pick it up and stop it.
 ***/

void test_systick() {
    systick.start();

    while (true) {
        wait(1);
        pc.printf("Current time (ms): %d\n", millis);
    }
}

void drive_straight() {
    systick.start();

    wait(1);
    mainController.driveStraight();
}

/*** Assignment 3, Part B
 * 
 * Even more PID! Once again, do not modify these functions, as
 * they act as test cases. For all these test cases, the mouse
 * should finish in its original position.
 * 
 * Part 2a: "do_turnsX". Your mouse will execute a series of turns.
 * Each of these turns should be 90 or 180 degrees, and your
 * rat should end up somewhat close to its original position.
 * Also, there are various stages. If you can pass Stage 1 and 2,
 * you are fine for this assignment. If you can pass Stage 3,
 * your rat has extremely good turns.
 * 
 * Part 2b: "move_cells": Your rat will move three cells,
 * first one at a time, then two at a time. Put your rat in the
 * maze to verify the distances travelled are correct.
 * 
 * Part 2c: "move_path": This simply combines the previous two
 * functions, simulating your mouse navigating a real path.
 ***/

void test_turns1() {
    systick.start();

    // STAGE 1: Turn left, then right.
    mainController.turn(90);
    mainController.turn(-90);
}

void test_turns2() {
    systick.start();

    // STAGE 2: Now with 180's.
    mainController.turn(90);
    mainController.turn(90);
    mainController.turn(-180);
    mainController.turn(-90);
    mainController.turn(-90);
    mainController.turn(180);
}

void test_turns3() {
    systick.start();

    // STAGE 3: The real (optional) test.
    mainController.turn(90);
    mainController.turn(90);
    mainController.turn(180);

    mainController.turn(-90);
    mainController.turn(-90);
    mainController.turn(-180);

    mainController.turn(90);
    mainController.turn(90);
    mainController.turn(-90);
    mainController.turn(-90);
}

void move_cells() {
    systick.start();

    mainController.moveCells(1);
    mainController.moveCells(2);
}

void move_path() {
    systick.start();

    mainController.moveCells(1);
    mainController.moveCells(1);
    mainController.turn(180);
    mainController.moveCells(2);
    mainController.turn(180);
}
