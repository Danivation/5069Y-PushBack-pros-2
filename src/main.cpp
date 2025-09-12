#include "main.h"
const int program_mode = 1; // 0 = default (competition mode, driver first), 1 = testing (auton first)

/**
 * auton selector flow:
 * initialize() runs as soon as the program starts, show the auton selector
 * competition_initialize() runs when the robot is plugged in, hide auton selector, calibrate inertial, and show the selected auton
**/

void screen_print() {
    //pros::lcd::initialize();
    while (true) {
        // odom position
        pros::lcd::print(0, "X: %f", chassis.getPose().x);
        pros::lcd::print(1, "Y: %f", chassis.getPose().y);
        pros::lcd::print(2, "Theta: %.2f", reduce_0_to_360(chassis.getPose().theta));

        // optical sensor data
        pros::lcd::print(3, "Proximity: %i", (int)optical_block.get_proximity());
        pros::lcd::print(4, "Hue: %f", optical_block.get_hue());
        pros::lcd::print(5, "RGB: (%.2f, %.2f, %.2f, %.2f)", 
            optical_block.get_rgb().red,
            optical_block.get_rgb().green,
            optical_block.get_rgb().blue,
            optical_block.get_rgb().brightness
        );

        // delay to save resources
        pros::delay(100);
    }
}

void controller_print() {
    master.clear();
    while (true) {
        master.print(0, 0, "I: %.2f", reduce_0_to_360(chassis.getPose().theta));
        pros::delay(50);
        master.print(1, 0, "%s", StorageDrain ? "Draining" : "Intaking");
        pros::delay(50);
        master.print(2, 0, "B: %d%%", pros::battery::get_capacity());
        pros::delay(50);
    }
}

void initialize() {
    imu_1.set_data_rate(5);
    right_mg.set_brake_mode_all(pros::MotorBrake::coast);
    left_mg.set_brake_mode_all(pros::MotorBrake::coast);
    intake_bottom.set_brake_mode(pros::MotorBrake::brake);
    intake_back.set_brake_mode(pros::MotorBrake::brake);
    intake_front.set_brake_mode(pros::MotorBrake::brake);
    optical_block.set_led_pwm(100);
    optical_block.set_integration_time(5);

    if (program_mode == 0) {
        // set up lvgl auton selector
        lvgl_auton_selector(red_right_autons, red_left_autons, blue_right_autons, blue_left_autons, skills_auton);
    } else if (program_mode == 1) {
        // set up llemu screen
        lv_obj_clean(lv_screen_active()); // get rid of lvgl
        pros::lcd::initialize(); // initialze llemu
        master.clear();
    
        pros::lcd::print(0, "Calibrating...");
        chassis.calibrate();
        chassis.setPose(0, 0, reduce_0_to_360(imu_1.get_rotation()));

        // print stuff to the brain screen and controller
        pros::Task screen_task(screen_print);
        pros::Task controller_task(controller_print);
    }
}

std::pair<auton_mode_t, auton_descriptor_t> selected_auton = { auton_mode_t::NONE, {""} };
void competition_initialize() {
    lv_obj_clean(lv_screen_active()); // get rid of lvgl
    pros::lcd::initialize(); // initialze llemu

    pros::lcd::print(0, "Calibrating...");
    chassis.calibrate();

    if (program_mode == 0) {
        selected_auton = get_selected_auton();
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
    } else if (program_mode == 1) {
        pros::lcd::print(0, "Program is in testing mode.");
        pros::lcd::print(1, "Switch to competition mode to work with field control!");
    }
}

void disabled() {}

void autonomous() {
    pros::lcd::shutdown();
    auton_descriptor_t selected_auton_new = get_selected_auton().second;
    if (selected_auton_new.callback) selected_auton_new.callback();
}

ASSET(curve_txt);

void opcontrol() {
    if (program_mode == 0) {
        SortColor = [&] -> pros::Color {
            if (selected_auton.first == RED_RIGHT || selected_auton.first == RED_LEFT) {
                return pros::Color::blue;
            } else if (selected_auton.first == BLUE_RIGHT || selected_auton.first == BLUE_LEFT) {
                return pros::Color::red;
            } else {
                return pros::Color::black; // sort nothing
            }
        }();
        //pros::Task d_color_sort         (ColorSort);

        pros::Task d_drivetrain_control (DrivetrainControl);
        pros::Task d_intake_control     (IntakeControl);
        pros::Task d_storage_control    (StorageControl);
        pros::Task d_loader_control     (LoaderControl);

        pros::Task d_controller_task    (controller_print);
    } else if (program_mode == 1) {
        chassis.setPose(16, -48, 14.5);

        intake_bottom.move(127);
        intake_back.move(-127);
        chassis.moveToPose(14, -14, 315, 2500, {.lead = 0.5, .maxSpeed = 90});
        chassis.waitUntilDone();

        intake_bottom.move(-127);
        intake_back.move(127);
    }
}