#include "main.h"

/**
 * Robot configuration
 */
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Rotation vertical_rotation(-1);
pros::Rotation horizontal_rotation(2);
pros::Imu imu_1(3);
pros::Imu imu_2(11);    // unused
pros::MotorGroup left_mg({-4, 5, 6}, pros::MotorGearset::blue);
pros::MotorGroup right_mg({7, -8, -9}, pros::MotorGearset::blue);
pros::Optical optical_block(10);
pros::Motor intake_back(17);
pros::Motor intake_bottom(-18);
pros::Motor intake_front(-19);
pros::Motor intake_top(20);
pros::adi::Pneumatics loader_piston('a', false);

/**
 * Lemlib configuration
 */
lemlib::TrackingWheel horizontal_tracker(&horizontal_rotation, lemlib::Omniwheel::NEW_275, -5.5);
lemlib::TrackingWheel vertical_tracker(&vertical_rotation, lemlib::Omniwheel::NEW_275, -0.8);
// drivetrain configuration:   left      right   track width       wheel size         rpm  horizontal drift (2 for omnis)
lemlib::Drivetrain drivetrain(&left_mg, &right_mg, 11.75, lemlib::Omniwheel::NEW_325, 450, 2);
// tracker wheel configuration
lemlib::OdomSensors sensors(&vertical_tracker, nullptr, &horizontal_tracker, nullptr, &imu_1);
// lateral PID controller
lemlib::ControllerSettings lateral_controller_small(7.5, // proportional gain (kP)
                                                    0.1, // integral gain (kI)
                                                    18, // derivative gain (kD)
                                                    1, // anti windup
                                                    0, // small error range, in inches
                                                    0, // small error range timeout, in milliseconds
                                                    0, // large error range, in inches
                                                    0, // large error range timeout, in milliseconds
                                                    80 // maximum acceleration (slew)
);
// lateral PID controller
lemlib::ControllerSettings lateral_controller_big  (7.5, // proportional gain (kP)
                                                    0.1, // integral gain (kI)
                                                    40, // derivative gain (kD)
                                                    1, // anti windup
                                                    0, // small error range, in inches
                                                    0, // small error range timeout, in milliseconds
                                                    0, // large error range, in inches
                                                    0, // large error range timeout, in milliseconds
                                                    80 // maximum acceleration (slew)
);
// angular PID controller
lemlib::ControllerSettings angular_controller_small(2.3, // proportional gain (kP)
                                                    0.2, // integral gain (kI)
                                                    13.7, // derivative gain (kD)
                                                    2, // anti windup
                                                    1, // small error range, in degrees
                                                    100, // small error range timeout, in milliseconds
                                                    3, // large error range, in degrees
                                                    1000, // large error range timeout, in milliseconds
                                                    0 // maximum acceleration (slew)
);
// angular PID controller
lemlib::ControllerSettings angular_controller_big  (1.9, // proportional gain (kP)
                                                    0.2, // integral gain (kI)
                                                    13, // derivative gain (kD)
                                                    5, // anti windup
                                                    1, // small error range, in degrees
                                                    200, // small error range timeout, in milliseconds
                                                    5, // large error range, in degrees
                                                    1000, // large error range timeout, in milliseconds
                                                    0 // maximum acceleration (slew)
);
// small linear & angular
lemlib::Chassis chassis_small(drivetrain, lateral_controller_small, angular_controller_small, sensors);
// big linear & angular
lemlib::Chassis chassis_big(drivetrain, lateral_controller_big, angular_controller_big, sensors);
// big linear & small angular
lemlib::Chassis chassis_mtp(drivetrain, lateral_controller_big, angular_controller_small, sensors);
CustomChassis chassis(&chassis_small, &chassis_big, &chassis_mtp, 100, 10);
