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
pros::Rotation vertical_rotation(16);

lemlib::TrackingWheel horizontal_tracker(&horizontal_rotation, lemlib::Omniwheel::NEW_275, -4-0.55-0.82);
lemlib::TrackingWheel vertical_tracker(&vertical_rotation, lemlib::Omniwheel::NEW_275, -0.6-0.29-0.28);
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
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
// create the chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    //lvgl_auton_selector();
    pros::lcd::initialize();
    imu_1.set_data_rate(5);
    imu_2.set_data_rate(5);
    right_mg.set_brake_mode_all(pros::MotorBrake::hold);
    left_mg.set_brake_mode_all(pros::MotorBrake::hold);
    intake_bottom.set_brake_mode(pros::MotorBrake::hold);
    intake_back.set_brake_mode(pros::MotorBrake::hold);
    intake_front.set_brake_mode(pros::MotorBrake::hold);
    intake_top.set_brake_mode(pros::MotorBrake::hold);

    chassis.calibrate(); // calibrate sensors
    pros::delay(3000);
    chassis.setPose(0, 0, imu_1.get_heading());

    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            //pros::lcd::print(3, "Horiz: %f", horizontal_rotation.get_position());
            //pros::lcd::print(4, "Vert: %f", vertical_rotation.get_position());
            printf("(%f,%f),", chassis.getPose().x, chassis.getPose().y);
            // delay to save resources
            pros::delay(20);
        }
    });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    left_mg.move(127);
    right_mg.move(-127);
    pros::delay(3000);
    left_mg.brake();
    right_mg.brake();
    pros::delay(500);
    left_mg.move(-127);
    right_mg.move(127);
    pros::delay(3000);
    left_mg.brake();
    right_mg.brake();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    //pros::Task d_drivetrain_control (DrivetrainControl);
    //pros::Task d_intake_control     (IntakeControl);

    pros::delay(500);
    autonomous();
}