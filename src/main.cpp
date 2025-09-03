#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    //lvgl_auton_selector();
    imu_1.set_data_rate(5);
    imu_2.set_data_rate(5);
    left_mg.set_brake_mode_all(pros::MotorBrake::coast);
    right_mg.set_brake_mode_all(pros::MotorBrake::coast);
    intake_bottom.set_brake_mode(pros::MotorBrake::hold);
    intake_back.set_brake_mode(pros::MotorBrake::hold);
    intake_front.set_brake_mode(pros::MotorBrake::hold);
    intake_top.set_brake_mode(pros::MotorBrake::hold);
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
void autonomous() {}

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

/**
 * Robot configuration
 */
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({8, -9, -10});
pros::MotorGroup right_mg({-18, 19, 20});
pros::Imu imu_1(1);
pros::Imu imu_2(20);
pros::Motor intake_bottom(-9);
pros::Motor intake_front(-10);
pros::Motor intake_back(11);
pros::Motor intake_top(12);
pros::Optical optical_block(13);

void opcontrol() {
    pros::Task d_drivetrain_control (DrivetrainControl);
    pros::Task d_intake_control     (IntakeControl);
}