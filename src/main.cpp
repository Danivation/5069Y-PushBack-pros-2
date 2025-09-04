#include "main.h"

/**
 * Robot configuration
 */
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({18, 19, -20}, pros::MotorGearset::blue);
pros::MotorGroup right_mg({-8, -9, 10}, pros::MotorGearset::blue);
pros::Imu imu_1(1);
pros::Imu imu_2(20);
pros::Motor intake_bottom(-2);
pros::Motor intake_front(-3);
pros::Motor intake_back(4);
pros::Motor intake_top(5);
pros::Optical optical_block(13);
pros::Rotation horizontal_rotation(15);
pros::Rotation vertical_rotation(-16);

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

void initialize() {
    //lvgl_auton_selector();
    pros::lcd::initialize();
    imu_1.set_data_rate(5);
    imu_2.set_data_rate(5);
    right_mg.set_brake_mode_all(pros::MotorBrake::brake);
    left_mg.set_brake_mode_all(pros::MotorBrake::brake);
    intake_bottom.set_brake_mode(pros::MotorBrake::hold);
    intake_back.set_brake_mode(pros::MotorBrake::hold);
    intake_front.set_brake_mode(pros::MotorBrake::hold);
    intake_top.set_brake_mode(pros::MotorBrake::hold);

    chassis.calibrate(); // calibrate sensors
    pros::delay(2000);
    chassis.setPose(0, 0, imu_1.get_heading());

    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", reduce_0_to_360(chassis.getPose().theta)); // heading
            //pros::lcd::print(3, "Horiz: %f", horizontal_rotation.get_position());
            //pros::lcd::print(4, "Vert: %f", vertical_rotation.get_position());
            //printf("(%f,%f),", chassis.getPose().x, chassis.getPose().y);
            // delay to save resources
            pros::delay(20);
        }
    });
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
    chassis.setPose(0, 0, imu_1.get_heading());
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(180, 100000);
}

void opcontrol() {
    //pros::Task d_drivetrain_control (DrivetrainControl);
    //pros::Task d_intake_control     (IntakeControl);

    autonomous();
}