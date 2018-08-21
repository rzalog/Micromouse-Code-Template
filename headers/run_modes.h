#pragma once

void modeToggle();

// Solve the maze
void ffIncremental();
void ffOneCell();
void ffSteady();
void doPath();

void testAddWalls();

// Testing with driving
void testSetModes();
void navigatePath();
void testMoveCounts();
void testMoveCountsAlt();
void testTurn();
void testPath();
void testDriveStraight();
void testErrorsNoPwm();
void testTurns();
void testStraighten();

// Testing without driving
void testMotors();
void testMoveController();
void testAngularPwm();
void testStraightPwm();
void testMinSpeed();
void testEncoders();
void testEncodersNoSystick();
void testGyro();
void testIR();
void testIRError();
void testIREncError();
void testLed();
void testLeds();
void testBuzzer();

void testLogging();
void testIRCharging();