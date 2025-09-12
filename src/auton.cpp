#include "main.h"

const std::vector<auton_descriptor_t> red_right_autons = {
    {"None", "0 points", auton_none},
    {"Red Right 1", "1 point", auton_red_right_1},
    {"Auton 2", "5 points", auton_red_right_2},
    {"Auton 3", "12 points", auton_red_right_3}
};
const std::vector<auton_descriptor_t> red_left_autons = {
    {"None", "0 points", auton_none},
    {"Red Left 1", "1 point", auton_red_left_1},
    {"Auton 2", "5 points", auton_red_left_2},
    {"Auton 3", "12 points", auton_red_left_3}
};
const std::vector<auton_descriptor_t> blue_right_autons = {
    {"None", "0 points", auton_none},
    {"Blue Right 1", "1 point", auton_blue_right_1},
    {"Auton 2", "5 points", auton_blue_right_2},
    {"Auton 3", "12 points", auton_blue_right_3}
};
const std::vector<auton_descriptor_t> blue_left_autons = {
    {"None", "0 points", auton_none},
    {"Blue Left 1", "1 point", auton_blue_left_1},
    {"Auton 2", "5 points", auton_blue_left_2},
    {"Auton 3", "12 points", auton_blue_left_3}
};
const auton_descriptor_t skills_auton = {"Skills", "0 points", auton_skills};

void auton_none() {
    chassis.setPose(16.5, -48.5, 13);

    intake_bottom.move(127);
    intake_back.move(-127);
    chassis.moveToPose(14, -14, 315, 2500, {.lead = 0.5, .maxSpeed = 90});

    chassis.waitUntilDone();
    intake_bottom.move(-127);
    intake_back.move(127);
}

void auton_red_right_1() {
    pros::lcd::print(0, "hi from red right 1");
}

void auton_red_right_2() {
    pros::lcd::print(0, "hi from red right 2");
}

void auton_red_right_3() {
    pros::lcd::print(0, "hi from red right 3");
}

void auton_red_left_1() {
    pros::lcd::print(0, "hi from red left 1");
}

void auton_red_left_2() {
    pros::lcd::print(0, "hi from red left 2");
}

void auton_red_left_3() {
    pros::lcd::print(0, "hi from red left 3");
}

void auton_blue_right_1() {
    pros::lcd::print(0, "hi from blue right 1");
}

void auton_blue_right_2() {
    pros::lcd::print(0, "hi from blue right 2");
}

void auton_blue_right_3() {
    pros::lcd::print(0, "hi from blue right 3");
}

void auton_blue_left_1() {
    pros::lcd::print(0, "hi from blue left 1");
}

void auton_blue_left_2() {
    pros::lcd::print(0, "hi from blue left 2");
}

void auton_blue_left_3() {
    pros::lcd::print(0, "hi from blue left 3");
}

void auton_skills() {
    // SKILLS: we can collect the blocks and when we want to get blocks of only one color we can use color sort so that instead of ejecting the wrong color, it makes the wrong color go back into storage
    pros::lcd::print(0, "hi from skills!");
}