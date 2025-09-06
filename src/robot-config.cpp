#include "main.h"

/**
 * Robot configuration
 */
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Rotation vertical_rotation(-1);
pros::Rotation horizontal_rotation(2);
pros::Imu imu_1(3);
pros::MotorGroup left_mg({-4, 5, 6}, pros::MotorGearset::blue);
pros::MotorGroup right_mg({7, -8, -9}, pros::MotorGearset::blue);
pros::Optical optical_block(10);
pros::Motor intake_back(17);
pros::Motor intake_bottom(-18);
pros::Motor intake_front(-19);
pros::Motor intake_top(20);
pros::Imu imu_2(11);    // unused

lemlib::TrackingWheel horizontal_tracker(&horizontal_rotation, lemlib::Omniwheel::NEW_275, -3.2);
lemlib::TrackingWheel vertical_tracker(&vertical_rotation, lemlib::Omniwheel::NEW_275, -0.4);
// drivetrain configuration:   left      right   track width       wheel size         rpm  horizontal drift (2 for omnis)
lemlib::Drivetrain drivetrain(&left_mg, &right_mg, 11.75, lemlib::Omniwheel::NEW_325, 450, 2);
// tracker wheel configuration
lemlib::OdomSensors sensors(&vertical_tracker, nullptr, &horizontal_tracker, nullptr, &imu_1);
// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);
// angular PID controller
lemlib::ControllerSettings angular_controller(1.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              7, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
// create the chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);