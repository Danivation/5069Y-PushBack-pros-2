#include "main.h"

const std::vector<auton_descriptor_t> red_right_autons = {
    {"None", "0 points", auton_none},
    {"Red Right 1", "1 point", auton_red_right_1},
    {"Auton 2", "5 points", auton_red_right_2},
    {"Auton 3", "12 points"}
};
const std::vector<auton_descriptor_t> red_left_autons = {
    {"None", "0 points"},
    {"Red Left 1", "1 point"},
    {"Auton 2", "5 points"},
    {"Auton 3", "12 points"}
};
const std::vector<auton_descriptor_t> blue_right_autons = {
    {"None", "0 points"},
    {"Blue Right 1", "1 point"},
    {"Auton 2", "5 points"},
    {"Auton 3", "12 points"}
};
const std::vector<auton_descriptor_t> blue_left_autons = {
    {"None", "0 points"},
    {"Blue Left 1", "1 point"},
    {"Auton 2", "5 points"},
    {"Auton 3", "12 points"}
};
const auton_descriptor_t skills_auton = {"Skills", "0 points", auton_skills};

ASSET(testpath_txt);

void odom_test() {
    chassis.setPose(0, 0, 0);
}

void pure_pursuit_test() {
    // set chassis pose
    chassis.setPose(0, 0, 0);
    // lookahead distance: 15 inches
    // timeout: 2000 ms
    chassis.follow(testpath_txt, 15, 2000);               // WILL NOT WORK if the path contains both forwards and backwards parts!!!!
    // follow the next path, but with the robot going backwards
    //chassis.follow(example2_txt, 15, 2000, false);
}

void move_to_pose_test() {
    chassis.moveToPose(
        0, 0, 0,                            // (0, 0, 0) pose
        4000,                               // timeout of 4000ms
        {.horizontalDrift = 2, .lead = 0.3} // horizontal drift of 2 with omni wheels
    );
}

void turn_tuning() {
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);
}

void auton_none() {
    pros::lcd::print(0, "hi from auton_none!");
}

void auton_red_right_1() {
    pros::lcd::print(0, "hi from red right 1");
}

void auton_red_right_2() {
    pros::lcd::print(0, "hi from red right 2");
}

void auton_skills() {
    pros::lcd::print(0, "hi from skills!");
}