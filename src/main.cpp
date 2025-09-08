#include "main.h"

/**
 * auton selector flow:
 * initialize() runs as soon as the program starts, show the auton selector
 * competition_initialize() runs when the robot is plugged in, hide auton selector, calibrate inertial, and show the selected auton
**/

void screen_print() {    
    pros::lcd::initialize();
    master.clear();
    pros::Task screen_task([&]() {
        while (true) {
            // optical sensor data
            pros::lcd::print(3, "Proximity: %i", (int)optical_block.get_proximity());
            pros::lcd::print(4, "Hue: %f", optical_block.get_hue());
            pros::lcd::print(5, "RGB: %f %f %f %f", 
                optical_block.get_rgb().red, 
                optical_block.get_rgb().green, 
                optical_block.get_rgb().blue, 
                optical_block.get_rgb().brightness
            );

            // odom position
            pros::lcd::print(0, "X: %f", chassis.getPose().x);
            pros::lcd::print(1, "Y: %f", chassis.getPose().y);
            pros::lcd::print(2, "Theta: %.2f", reduce_0_to_360(imu_1.get_rotation()));

            master.print(0, 0, "I: %f", reduce_0_to_360(imu_1.get_rotation()));
            // delay to save resources
            pros::delay(20);
        }
    });
}

void initialize() {
    //lvgl_auton_selector(red_right_autons, red_left_autons, blue_right_autons, blue_left_autons, skills_auton);
    pros::delay(500);
    lv_obj_clean(lv_screen_active());
    pros::delay(200);

    pros::lcd::initialize();
    pros::delay(200);
    master.clear();
    pros::Task screen_task(screen_print);


    imu_1.set_data_rate(5);
    imu_2.set_data_rate(5);
    right_mg.set_brake_mode_all(pros::MotorBrake::coast);
    left_mg.set_brake_mode_all(pros::MotorBrake::coast);
    intake_bottom.set_brake_mode(pros::MotorBrake::brake);
    intake_back.set_brake_mode(pros::MotorBrake::brake);
    intake_front.set_brake_mode(pros::MotorBrake::brake);
    intake_top.set_brake_mode(pros::MotorBrake::brake);
    optical_block.set_led_pwm(100);
    optical_block.set_integration_time(5);
}

void competition_initialize() {
    lv_obj_clean(lv_screen_active());
    pros::lcd::initialize();

    pros::lcd::print(0, "Calibrating...");
    chassis.calibrate(); // calibrate sensors

    std::pair<auton_mode_t, auton_descriptor_t> selected_auton = get_selected_auton();
    std::string auton_name;
    if (selected_auton.first == RED_RIGHT) {
        auton_name = std::format("Red Right {} ({})", selected_auton.second.name, selected_auton.second.score);
    } else if (selected_auton.first == RED_LEFT) {
        auton_name = std::format("Red Left {} ({})", selected_auton.second.name, selected_auton.second.score);
    } else if (selected_auton.first == BLUE_RIGHT) {
        auton_name = std::format("Blue Right {} ({})", selected_auton.second.name, selected_auton.second.score);
    } else if (selected_auton.first == BLUE_LEFT) {
        auton_name = std::format("Blue Left {} ({})", selected_auton.second.name, selected_auton.second.score);
    } else if (selected_auton.first == SKILLS) {
        auton_name = std::format("Skills ({})", selected_auton.second.score);
    }
    pros::lcd::print(0, "Auton: %s", auton_name);
}

void disabled() {}

void autonomous() {
    pros::lcd::shutdown();
    auton_descriptor_t selected_auton = get_selected_auton().second;
    if (selected_auton.callback) selected_auton.callback();
}

void opcontrol() {
    SortColor = Color::blue;
    //pros::Task d_color_sort         (ColorSort);

    // pros::Task d_drivetrain_control (DrivetrainControl);
    // pros::Task d_intake_control     (IntakeControl);
    // pros::Task d_storage_control    (StorageControl);
    // pros::Task d_loader_control     (LoaderControl);

    
    chassis.calibrate(); // calibrate sensors
    pros::delay(1000);
    
    chassis.turnToHeading(180, 3000);
    chassis.turnToHeading(0, 3000);
    chassis.turnToHeading(90, 3000);
    chassis.turnToHeading(0, 3000);
    pros::delay(1000);
}