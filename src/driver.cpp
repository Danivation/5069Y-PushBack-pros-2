#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)
#define INTAKE_TO_STORAGE       master.get_digital(DIGITAL_R1)
#define INTAKE_TO_LOW_GOAL      master.get_digital(DIGITAL_R2)
#define INTAKE_TO_HIGH_GOAL     master.get_digital(DIGITAL_L1)
#define INTAKE_TO_MID_GOAL      master.get_digital(DIGITAL_L2)
#define STORAGE_DRAIN_TOGGLE    master.get_digital(DIGITAL_B)
#define STORAGE_REVERSE_TOGGLE  master.get_digital(DIGITAL_DOWN)
#define LOADER_TOGGLE           master.get_digital(DIGITAL_A)

void DrivetrainControl() {
    float throttle;
    float turn;
    while (true) {
        throttle = DeadBand(THROTTLE_AXIS, 5);
        turn = DeadBand(TURN_AXIS, 5);
        left_mg.move(throttle - turn);
        right_mg.move(throttle + turn);
        pros::delay(10);
    }
}

void IntakeControl() {
    while (true) {
        if (INTAKE_TO_STORAGE) {
            
        }
        pros::delay(10);
    }
}