#include "main.h"

#define THROTTLE_AXIS           master.get_analog(ANALOG_LEFT_Y)
#define TURN_AXIS               master.get_analog(ANALOG_RIGHT_X)
#define INTAKE_TO_STORAGE       master.get_digital(DIGITAL_R1)
#define INTAKE_TO_LOW_GOAL      master.get_digital(DIGITAL_R2)
#define INTAKE_TO_MID_GOAL      master.get_digital(DIGITAL_L2)
#define INTAKE_TO_HIGH_GOAL     master.get_digital(DIGITAL_L1)
#define STORAGE_DRAIN_TOGGLE    master.get_digital(DIGITAL_B)
#define STORAGE_REVERSE_TOGGLE  master.get_digital(DIGITAL_A)
#define LOADER_TOGGLE           master.get_digital(DIGITAL_DOWN)

void DrivetrainControl() {
    float throttle;
    float turn;
    while (true) {
        throttle = DeadBand(THROTTLE_AXIS, 2);
        turn = DeadBand(TURN_AXIS, 2);
        left_mg.move(throttle + turn);
        right_mg.move(throttle - turn);
        pros::delay(10);
    }
}

bool StorageDrain = false;
void IntakeControl() {
    while (true) {
        if (INTAKE_TO_STORAGE) {
            // storage: spin intake forwards, spin front forwards, do not drain, set hood to storage
            intake_bottom.move(127);
            if (!ColorStop) intake_front.move(127);
            if (false) intake_back.move(-127);
            else intake_back.brake();
            if (!hood_piston.is_extended()) hood_piston.extend();
        } else if (INTAKE_TO_LOW_GOAL) {
            // low goal: spin intake reverse, spin front reverse, drain if draining, set hood to storage
            intake_bottom.move(-127);
            intake_front.move(-127);
            if (StorageDrain) intake_back.move(127);
            else intake_back.brake();
            //if (!hood_piston.is_extended()) hood_piston.extend();
        } else if (INTAKE_TO_MID_GOAL) {
            // mid goal: spin intake forwards, spin front reverse, drain if draining, set hood to storage
            // figure out some way to get it to go to high goal if it gets color sorted here
            intake_bottom.move(127);
            if (!ColorStop) intake_front.move(-127);
            if (StorageDrain) intake_back.move(127);
            else intake_back.brake();
            //if (!hood_piston.is_extended()) hood_piston.extend();
        } else if (INTAKE_TO_HIGH_GOAL) {
            // high goal: spin intake forwards, spin front forwards, drain if draining, set hood to high goal
            intake_bottom.move(127);
            if (!ColorStop) intake_front.move(127);
            if (StorageDrain) intake_back.move(127);
            else intake_back.brake();
            if (hood_piston.is_extended()) hood_piston.retract();
        } else {
            intake_bottom.brake();
            intake_front.brake();
            intake_back.brake();
        }
        pros::delay(10);
    }
}

void StorageControl() {
    while (true) {
        waitUntil(!STORAGE_DRAIN_TOGGLE);
        waitUntil(STORAGE_DRAIN_TOGGLE);
        StorageDrain = !StorageDrain;
    }
}

void LoaderControl() {
    while (true) {
        waitUntil(!LOADER_TOGGLE);
        waitUntil(LOADER_TOGGLE);
        loader_piston.toggle();
    }
}