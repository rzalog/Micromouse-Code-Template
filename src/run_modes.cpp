#include "run_modes.h"
#include "sensors.h"
#include "globals.h"
#include "main_controller.h"
#include "move_controller.h"
#include "modes.h"
#include "pins.h"

#include "logging.h"

void waitForStart() {
    ir.frontOn();
    systick.wait(0.25);
    while(!veryShortWall())
        ;
    systick.wait(0.25);
    ir.frontOff();
}

// NS is no systick
void waitForStartNS() {
    wait(0.5);
    ir.frontOn();
    ir.update();
    while (!veryShortWall()) ir.update();
    ir.frontOff();
    wait(0.5);   
}

void modeToggle() {
    systick.start();
    
    set_mode();
    beeps(cur_mode);
    
    if (mode(1)) {
        ffIncremental();
    } 
    else if (mode(2)) {
        testIR();   
    }
    else if (mode(3)) {
        testTurns();   
    }
}

void ffIncremental() {
    pc.printf("all kinds of floodfill\n");
    systick.start();
    waitForStart();
    
    while (1) {
        beep_long();
        set_mode();
        beeps(cur_mode);
        
        waitForStart();
        
        if (mode(1)) {
            mainController.floodfillOneCell();   
        }
        else if (mode(2)) {
            mainController.floodfillSteady();
        }
        
        wait(1);   
        mainController.turnAround();
    }
}

void ffOneCell() {
    pc.printf("floodfill one cell at a time\n");
    systick.start();
    waitForStart();
    
    while(1) {
        mainController.floodfillOneCell();
        mainController.turnAround();
        beep();
        wait(1);
        beep_long();
        waitForStart();
        beep();
        wait(1);
        beep_long();
        waitForStart();
    }
}

void ffSteady() {
    pc.printf("floodfill steady\n");
    systick.start();
    waitForStart();
    
    mainController.straightenTurnAroundOn();
    
    while(1) {
        mainController.floodfillSteady();
        mainController.turnAround();
        beep();
        wait(1);
        beep_long();
        waitForStart();
        beep();
        wait(1);
        beep_long();
        waitForStart();
        mainController.resetFFPos();   
    }
}

void doPath() {
    systick.start();
    while (1) {
        waitForStart();
    
        mainController.speedPath("123T123T");
    }
}

void testSetModes() {
    systick.start();
    
    while (1) {
        waitForStart();
        beep_long();
        wait(1);
        pc.printf("setting mode\n");
        set_mode();
        beep_long();
        beeps(cur_mode);
        pc.printf("cur mode: %d\n", cur_mode);   
    }   
}

void testAddWalls() {
    systick.start();
    
    while (1) {
        mainController.addWalls();
        wait(0.5);   
    }   
}

void testMoveCounts() {
    systick.start();
    
    int counter = 3;
    
    while(1) {
        waitForStart();
        pc.printf("moving %d cells\n", counter);
        led7.toggle();
        mainController.moveDistance(180 * counter);
        led7.toggle();   
        pc.printf("done moving %d cells\n", counter);
    }   
}

void testTurn() {
    systick.start();
    
    float deg = 90;
    while(1) {
        waitForStart();
        led7.toggle();
        mainController.turn(deg);
        led7.toggle();   
    }   
}

void testPath() {
    systick.start();
    
    waitForStart();
    ir.setSideBase();
    led7.on();
    
    pc.printf("going on a journey (path)\n");
    
    while(1) {
        waitForStart();
        led7.off();

        mainController.moveCells(10);
        mainController.turnAround();
        mainController.moveCells(10);
        mainController.straighten();
        mainController.turnAround();
    }
}
void testDriveStraight() {
    systick.start();
    
    while (1) {
        waitForStart();
        ir.setSideBase();
                        
        led7.toggle();
        mainController.driveStraight();
        mainController.turnAround();
        led7.toggle();
    }
}

void testErrorsNoPwm() {
    systick.start();
    while (1) {
        waitForStart();
        ir.setSideBase();
        
        pc.printf("\n---\nNew run\n---\n");        
        
        led7.on();
        mainController.goNoPwm();
        led7.off();   
    }
}

void testTurns() {
    pc.printf("testing turns\n");
    systick.start();
    
    while(1) {
        waitForStart();
        
        mainController.turnRight();
        mainController.turnRight();
        mainController.turnRight();
        mainController.turnRight();
        wait(1);
        mainController.turnAround();
        mainController.turnAround();
        wait(1);
        mainController.turnLeft();
        mainController.turnLeft();
        mainController.turnLeft();
        mainController.turnLeft();
    }   
}

void testStraighten() {
    pc.printf("testing straighten\n");
    systick.start();
    waitForStart();
    
    while(1) {
        mainController.moveOneCell();
        mainController.straighten();
        mainController.turnAround();
        mainController.moveOneCell();
        mainController.turnAround();
        
        waitForStart();
    }
}

void testMotors() {
    systick.start();
    
    while (1) {
        while (!shortWall())
            ;

        //motors.setRightPwm(0.2);
        //motors.setLeftPwm(0.4);
        motors.startBaseSpeed();
        systick.wait(1);
        motors.stop();
    }
}


void testAngularPwm() {
       // For the transfer function
    float pwms[3];
    pwms[0] = 20.0;
    pwms[1] = 30.0;
    pwms[2] = 40.0;
    
    int n = 1000;
    
    double measurements[3][1000];
    
    for (int j = 0; j < 3; j++) {
        led7.on();
        waitForStartNS();
        led7.off();
        
        for (int i = 0; i < n; i++) {
               measurements[j][i] = 0;
        }
        
        Timer t;
        encoders.reset();
        
        t.start();
        motors.setRightPwm(-pwms[j]);
        motors.setLeftPwm(pwms[j]);
        
        int oldR = 0;
        int oldL = 0;
        
        for (int i = 1; i < n; i++) {
            int start = t.read_us();
            while (t.read_us() - start < 1000)
                ;
            encoders.update();
            measurements[j][i] = ((encoders.getPulsesL() - oldL) - (encoders.getPulsesR() - oldR)) * 1000;
            oldL = encoders.getPulsesL();
            oldR = encoders.getPulsesR();
        }
        t.stop();
        motors.stop();
    }
    
    while (1) {
        for (int j = 0; j < 3; j++) {
            waitForStartNS();
            pc.printf("PWM: %f\n", pwms[j]);
    
            for (int i = 0; i < n; i++) {
                pc.printf("%f,", measurements[j][i]);
            }
            pc.printf("\n");
        }
    }
}

void testStraightPwm() {
       // For the transfer function
    float pwms[3];
    pwms[0] = 20.0f;
    pwms[1] = 30.0f;
    pwms[2] = 40.0f;
    
    int n = 1000;
    
    double measurements[3][1000];
    
    for (int j = 0; j < 3; j++) {
        led7.on();
        waitForStartNS();
        led7.off();
        
        for (int i = 0; i < n; i++) {
               measurements[j][i] = 0;
        }
        
        Timer t;
        encoders.reset();
        
        t.start();
        motors.setRightPwm(pwms[j]);
        motors.setLeftPwm(pwms[j]);
        
        int oldR = 0;
        int oldL = 0;
        
        for (int i = 1; i < n; i++) {
            int start = t.read_us();
            while (t.read_us() - start < 1000)
                ;
            encoders.update();
            measurements[j][i] = ((encoders.getPulsesL() - oldL) + (encoders.getPulsesR() - oldR)) * 500;
            oldL = encoders.getPulsesL();
            oldR = encoders.getPulsesR();
        }
        t.stop();
        motors.stop();
    }
    
    while (1) {
        for (int j = 0; j < 3; j++) {
            waitForStartNS();
            pc.printf("PWM: %f\n", pwms[j]);
    
            for (int i = 0; i < n; i++) {
                pc.printf("%f,", measurements[j][i]);
            }
            pc.printf("\n");
        }
    }
}

void testMinSpeed() {
    systick.start();
    
    float curPwm = 0.035;
    
    while(1) {
        waitForStart();
        pc.printf("Current pwm: %.3f\n", curPwm);
        
        motors.setLeftPwm(curPwm);
        motors.setRightPwm(curPwm);
        
        curPwm += 0.001f;
    }
       
}

void testEncoders() {
    systick.start();
    while (1) {
        encoders.printValues();
        if (shortWall()) encoders.reset();
        systick.wait(0.5);
    }
}

void testEncodersNoSystick() {
    systick.start();
    while (1) {
        encoders.update();
        encoders.printValues();
        //ir.update();
        if (shortWall()) encoders.reset();
        wait(0.5);   
    }   
}

void testGyro() {
    wait(0.5);
    systick.start();

    pc.printf("raw: %f\n", gyro.rawReading());
    beep();

    Timer t;
    t.start();
    while (1) {
        pc.printf("%f, %f\n", t.read(), gyro.theta());

        if (shortWall()) gyro.reset();
        
        systick.wait(1);
    }
}

void testIR() {
    systick.start();

    while (1) {
        ir.printValues();
        systick.wait(0.5);
    }   
}

void testIRError() {
    ir.setSideBase();
    while (1) {
        ir.update();
        pc.printf("rsbase: %f\tlsbase: %f\trserror: %f\tlserror: %f\t\n",
            ir.RS_base(), ir.LS_base(), ir.RS_error(), ir.LS_error());
            
        if (shortWall()) {
            ir.setSideBase();
        }
        wait(0.5);
    }   
}

void testIREncError() {
    ir.setSideBase();
    
    while (1) {
        ir.update();
        encoders.update();
        
        int enc_error = encoders.getPulsesR() - encoders.getPulsesL();
        float ir_error = 0;
        
        if (ir.RS() < IR_PID_THRESHOLD) {
            ir_error = ir.RS_error();
        }
        else if (ir.LS() < IR_PID_THRESHOLD) {
            ir_error = ir.LS_error();   
        }
        
        if (shortWall()) {
            encoders.reset();
            ir.setSideBase();
        }   
        
        pc.printf("enc: %d\tir: %f\t\n", enc_error, ir_error);
        wait(0.5);
    }   
}

void testLed() {
    while (1) {
        led2.toggle();
        wait(1);
    }
}

void toggle_delay(Led led, int n) {
    pc.printf("testing led %d\n", n);
    led.on();
    wait(1);
    led.off();
    wait(1);
}

void testLeds() {
    while (true) {
        toggle_delay(led1, 1);
        toggle_delay(led2, 2);
//        toggle_delay(led3, 3);
//        toggle_delay(led4, 4);
        toggle_delay(led5, 5);
        toggle_delay(led6, 6);
        toggle_delay(led8, 8);
    }
}

void testBuzzer() {
    while (true) {
        toggle_delay(led5, 5);
        //buzzer.beep(300, .1);
    }
}

void goStraight() {
    Timer t;
    t.start();
    
    bool run_motors = true;
    
    if (run_motors)
        motors.startBaseSpeed();
    
    float Kp = 3;
    float Kd = 10;
    
    ir.update();
    float base_ls = ir.LS();
    float base_rs = ir.RS();
    
    int old_time = t.read_us();
    float old_error = 0;
    
    while (true) {
        ir.update();
        
        if (shortWall()) {
            motors.stop();
            break;
        }
        
        float diff_ls = ir.LS() - base_ls;
        float diff_rs = ir.RS() - base_rs;
        
        float diff;
        bool bad_ls = ir.LS() > base_ls;
        bool bad_rs = ir.RS() > base_rs;
        if (bad_ls)
            diff = -diff_rs;
        else if (bad_rs)
            diff = diff_ls;
        else if (bad_ls && bad_rs)
            diff = 0;
        else {
            diff = diff_ls;
        }
        
        float error = diff;
        
        // P
        float p_error = error * Kp;
        
        // D
        float cur_time = t.read_us();
        int dt = cur_time - old_time;
        
        float d_error = (error - old_error) * Kd / dt;
        
        old_time = cur_time;
        old_error = error;
        
        // Put it all together
        float correction = p_error + d_error;
        
        if (!run_motors)
            pc.printf("p_error: %.5f\td_error: %.5f\tcorrection: %.5f\tdt: %d\n",
                p_error, d_error, correction, dt);
        if (run_motors)
            motors.incrementPwm(correction);
    }
}

void turn(int dir) {
    encoders.reset();
    int counts_to_turn = 1500;
    
    // 0 is right, 1 is left, 2 is 180
    float rpwm, lpwm;

    rpwm = BASE_SPEED;
    lpwm = -rpwm;
    
    if (dir == 0) {
        rpwm = -rpwm;
        lpwm = -lpwm;
    }
    
    motors.setRightPwm(rpwm);
    motors.setLeftPwm(lpwm);
    
    int right, left;
    do {
        encoders.update();
        right = abs(encoders.getPulsesR());
        left = abs(encoders.getPulsesL());
    } while (right < counts_to_turn and left < counts_to_turn);
    
    motors.stop();
}

void navigatePath() {
    while (true) {
        goStraight();
        
        led8.on();
        wait(1);
        if (ir.RS() > ir.LS()) {
            turn(0);
        } else {
            turn(1);
        }
        led8.off();
        
        wait(1);
    }
}

void testMoveCountsBad() {
    systick.start();
    
    int counter = 3;
    
    while (1) {
        waitForStart();
        led2.toggle();
        mainController.moveDistance(180 * counter);
        //counter++;
    }
}

void testLogging() {
    systick.start();
    
    Logging logger;

    logger.printf("hello, world!");
    int counter = 0;
    
    while (counter < 10) {
        logger.printf("counter: %d", counter++);
        wait(1);
    }
    
    pc.printf("ALL\n");
    logger.print_all();
}

void testIRCharging() {
    DigitalOut ir_RF(IR_Rfront);
    AnalogIn rec_RF(DET_Rfront);

    Timer t;
    
    
    float init_rf = rec_RF.read();
    int size = 200;
    float vals[200];
    for (int i=0; i < size; i++) {
        vals[i] = 0;   
    }
 
    ir_RF = 1;
    t.start();
   
    int us = t.read_us();
    while (us < size) {
        vals[us] = rec_RF.read();
        us = t.read_us();
        
        if (us > size / 2) ir_RF = 0;
    }
    ir_RF = 0;
    
    for (int i = 0; i < size; i++) {
        if (i == 0 || vals[i] > 0.001f) {
            pc.printf("%d,%.3f\n", i, vals[i]);   
        }
    }
}