#include "main.h"

void initialize() {
    lvgl_auton_selector();
    imu_1.set_data_rate(5);
    imu_2.set_data_rate(5);
    right_mg.set_brake_mode_all(pros::MotorBrake::coast);
    left_mg.set_brake_mode_all(pros::MotorBrake::coast);
    intake_bottom.set_brake_mode(pros::MotorBrake::brake);
    intake_back.set_brake_mode(pros::MotorBrake::brake);
    intake_front.set_brake_mode(pros::MotorBrake::brake);
    intake_top.set_brake_mode(pros::MotorBrake::brake);
    optical_block.set_led_pwm(100);

    chassis.calibrate(); // calibrate sensors
    pros::delay(2000);
    chassis.setPose(0, 0, imu_1.get_heading());

    // print position to brain screen
/*     pros::Task screen_task([&]() {
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
    }); */
}

void disabled() {}

void competition_initialize() {
    lv_deinit();
    pros::lcd::initialize();
}

void autonomous() {
    chassis.setPose(0, 0, imu_1.get_heading());
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(180, 100000);
}

void opcontrol() {
    SortColor = Color::red;
    pros::Task d_color_sort         (ColorSort);

    pros::Task d_drivetrain_control (DrivetrainControl);
    pros::Task d_intake_control     (IntakeControl);
    pros::Task d_storage_control    (StorageControl);

    //autonomous();
}