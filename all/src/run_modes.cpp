/***
 * Assignment 2
 ***/

/***
 * The first function you fill in, to make your motors do the steps
 * detailed in the check-off list.
 ***/
void basic_motor_movement() {
    // TODO: Move forwards

    wait(3);

    // TODO: Move backwards

    wait(3);

    // TODO: Turn
}

/***
 * The second function is done for you, but you will need to make
 * the encoders actually work and read the correct values, and verify
 * with the steps detailed in the check-off list.
 ***/
void read_encoders() {
    while (true) {
        encoders.printValues();
        wait(0.5);
    }
}

/***
 * Assignment 3, Part 1
 * 
 * PID! First get your mouse driving straight endlessly (you
 * will have to pick it up to stop it).
 * Then, get your mouse to turn.
 * 
 * You should not be modifying any of the three following
 * functions! They are essentially test cases. If your rat does
 * not achieve the specified behavior, then something is wrong
 * in either your "main_controller" class or your
 * "pid_controller" class.
 * 
 * Part 1: "drive_straight". Your mouse should drive straight
 * forever, until you pick it up and stop it.
 ***/

void drive_straight() {
    wait(1);
    main_controller.driveStraight();
}

/*** Assignment 3, Part 2
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

void do_turns1() {
    // STAGE 1: Turn left, then right.
    mainController.turn(90);
    mainController.turn(-90);
}

void do_turns2() {
    // STAGE 2: Now with 180's.
    mainController.turn(90);
    mainController.turn(90);
    mainController.turn(-180);
    mainController.turn(-90);
    mainController.turn(-90);
    mainController.turn(180);
}

void do_turns3() {
    // STAGE 3: The real test.
    mainController.turn(90);
    mainController.turn(90);
    mainControlller.turn(180);

    mainController.turn(-90);
    mainController.turn(-90);
    mainController.turn(-180);

    mainController.turn(90);
    mainController.turn(90);
    mainController.turn(-90);
    mainController.turn(-90);
}

void move_cells() {
    mainController.moveCells(1);
    mainController.moveCells(2);
}

void move_path() {
    mainController.moveCells(1);
    mainController.moveCells(1);
    mainController.turn(180);
    mainController.moveCells(2);
    mainController.turn(180);
}

/***
 * Assignment 4
 * 
 * Have your rat "navigate" a path. So, when your rat sees a wall
 * in front of itself, it should see if there is a left or right
 * opening, and then turn that way.
 * 
 * Feel free to do this one cell at a time, or more if you are
 * ambitious :).
 * 
 * Also, you should now be adding IR PID to help your rat go
 * straight along straight paths.
 ***/
void navigate_path() {
    while(true) {
        mainController.moveCells(1);

        bool is_wall_in_front = false;
        if (is_wall_in_front) {
            // TODO: If you see a wall, do something!
        }
    }
}