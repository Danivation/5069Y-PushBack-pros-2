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
            intake_bottom.move_voltage(12000);
            if (!ColorStop) intake_front.move_voltage(12000);
            intake_back.brake();
        } else if (INTAKE_TO_LOW_GOAL) {
            intake_bottom.move_voltage(-12000);
            intake_front.move_voltage(-12000);
            if (StorageDrain) intake_back.move_voltage(12000);
            else intake_back.brake();
        } else if (INTAKE_TO_MID_GOAL) {
            // figure out some way to get it to go to high goal if it gets color sorted here
            intake_bottom.move_voltage(12000);
            if (!ColorStop) intake_front.move_voltage(-12000);
            if (StorageDrain) intake_back.move_voltage(12000);
            else intake_back.brake();
        } else if (INTAKE_TO_HIGH_GOAL) {
            intake_bottom.move_voltage(12000);
            if (!ColorStop) intake_front.move_voltage(12000);
            if (StorageDrain) intake_back.move_voltage(12000);
            else intake_back.brake();
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