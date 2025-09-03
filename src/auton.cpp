#include "main.h"

ASSET(testpath_txt);

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