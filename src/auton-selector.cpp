#include "main.h"
#include "images/push_back_field_400.c"
#include "images/push_back_skills_field_400.c"

//pros::Task confirm_task;

lv_obj_t * field_image;
lv_obj_t * red_right_button;
lv_obj_t * red_left_button;
lv_obj_t * blue_right_button;
lv_obj_t * blue_left_button;
lv_obj_t * skills_button;
lv_obj_t * skills_label;

static lv_style_t style_button_bg;
static lv_style_t style_button_red_press;
static lv_style_t style_button_blue_press;
static lv_style_t style_text;

lv_obj_t * back_button;
lv_obj_t * back_label;
lv_obj_t * confirm_button;
lv_obj_t * confirm_label;
lv_obj_t * prev_auton_button;
lv_obj_t * prev_auton_label;
lv_obj_t * next_auton_button;
lv_obj_t * next_auton_label;
lv_obj_t * current_auton_score_label;
lv_obj_t * current_auton_name_label;

const std::vector<auton_descriptor_t> red_right_autons = {
    {"mp test 3", "0 points", "path-points-3.txt", "autonNone"},
    {"Red Right 1", "1 point"},
    {"Auton 2", "5 points"},
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
const auton_descriptor_t skills_auton = {"Skills", "0 points", "skills.txt"};
auton_mode_t selected_auton_group = NONE;
int selected_auton_index = 0;
// std::vector<std::vector<VelocityLayout>> mp_paths;

// pros::Task confirm_clicked{[] {
//     // calculate motion profile
//     printf("calculating motion profile\n");
//     std::vector<std::vector<Point>> controlPoints;
//     std::vector<std::vector<KeyframeVelocitiesXandY>> keyFrameVelocityList = {
//         {{-0.026, -0.605, 0}, {0.717, 0.094, 100}},
//         {{0.717, 0.094, 100}, {0.043, 1.158, 0}}
//     };
//     if (selected_auton_group == RED_RIGHT) {
//         loadPaths(red_right_autons[selected_auton_index].path_file, controlPoints);
//     } else if (selected_auton_group == RED_LEFT) {
//         loadPaths(red_left_autons[selected_auton_index].path_file, controlPoints);
//     } else if (selected_auton_group == BLUE_RIGHT) {
//         loadPaths(blue_right_autons[selected_auton_index].path_file, controlPoints);
//     } else if (selected_auton_group == BLUE_LEFT) {
//         loadPaths(blue_left_autons[selected_auton_index].path_file, controlPoints);
//     } else if (selected_auton_group == SKILLS) {
//         loadPaths(skills_auton.path_file, controlPoints);
//     }
//     mp_paths = printVels(controlPoints, keyFrameVelocityList, true);            // use keyframes
//     printVoltageVector("v_L = ", getVoltageLayout(mp_paths), "left");
//     printVoltageVector("v_R = ", getVoltageLayout(mp_paths), "right");
//     printf("motion profile calculated, calibrating inertial\n");

//     imu_1.reset();
//     imu_2.reset();
//     delay(3000);
//     setInertial(0);
//     printf("inertial calibrated\n");

//     while (true) {
//         if (trueHeading() > 359.5 || trueHeading() < 0.5) {
//             lv_obj_set_style_bg_color(lv_screen_active(), lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
//             lv_obj_set_style_bg_color(confirm_button, lv_palette_darken(LV_PALETTE_GREEN, 1), LV_PART_MAIN);
//             lv_obj_set_style_text_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
//         } else {
//             lv_obj_set_style_bg_color(lv_screen_active(), lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
//             lv_obj_set_style_bg_color(confirm_button, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
//             lv_obj_set_style_text_color(lv_screen_active(), lv_color_white(), LV_PART_MAIN);
//         }
//         delay(10);
//     }
// } };

static void back_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "back clicked\n";
        selected_auton_group = NONE;
        lv_obj_clean(lv_screen_active());
        lvgl_auton_selector();
    }
}

static void confirm_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "confirm clicked\n";
        lv_obj_set_style_text_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
        lv_obj_set_style_bg_color(confirm_button, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
        lv_obj_set_style_bg_color(lv_screen_active(), lv_palette_darken(LV_PALETTE_ORANGE, 1), LV_PART_MAIN);
        //confirm_task = Task::create(confirm_clicked);
    }
}

static void prev_auton_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        if (selected_auton_group == RED_RIGHT) {
            std::cout << "red right previous\n";
            selected_auton_index = (selected_auton_index - 1 + red_right_autons.size()) % red_right_autons.size();
            lv_label_set_text(current_auton_name_label, red_right_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, red_right_autons[selected_auton_index].score.c_str());
        } else if (selected_auton_group == RED_LEFT) {
            std::cout << "red left previous\n";
            selected_auton_index = (selected_auton_index - 1 + red_left_autons.size()) % red_left_autons.size();
            lv_label_set_text(current_auton_name_label, red_left_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, red_left_autons[selected_auton_index].score.c_str());
        } else if (selected_auton_group == BLUE_RIGHT) {
            std::cout << "blue right previous\n";
            selected_auton_index = (selected_auton_index - 1 + blue_right_autons.size()) % blue_right_autons.size();
            lv_label_set_text(current_auton_name_label, blue_right_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, blue_right_autons[selected_auton_index].score.c_str());
        } else if (selected_auton_group == BLUE_LEFT) {
            std::cout << "blue left previous\n";
            selected_auton_index = (selected_auton_index - 1 + blue_left_autons.size()) % blue_left_autons.size();
            lv_label_set_text(current_auton_name_label, blue_left_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, blue_left_autons[selected_auton_index].score.c_str());
        }
    }
}

static void next_auton_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        if (selected_auton_group == RED_RIGHT) {
            std::cout << "red right next\n";
            selected_auton_index = (selected_auton_index + 1) % red_right_autons.size();
            lv_label_set_text(current_auton_name_label, red_right_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, red_right_autons[selected_auton_index].score.c_str());
        } else if (selected_auton_group == RED_LEFT) {
            std::cout << "red left next\n";
            selected_auton_index = (selected_auton_index + 1) % red_left_autons.size();
            lv_label_set_text(current_auton_name_label, red_left_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, red_left_autons[selected_auton_index].score.c_str());
        } else if (selected_auton_group == BLUE_RIGHT) {
            std::cout << "blue right next\n";
            selected_auton_index = (selected_auton_index + 1) % blue_right_autons.size();
            lv_label_set_text(current_auton_name_label, blue_right_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, blue_right_autons[selected_auton_index].score.c_str());
        } else if (selected_auton_group == BLUE_LEFT) {
            std::cout << "blue left next\n";
            selected_auton_index = (selected_auton_index + 1) % blue_left_autons.size();
            lv_label_set_text(current_auton_name_label, blue_left_autons[selected_auton_index].name.c_str());
            lv_label_set_text(current_auton_score_label, blue_left_autons[selected_auton_index].score.c_str());
        }
    }
}

void create_secondary_screen() {
    // hide old buttons
    lv_obj_delete(red_left_button);
    lv_obj_delete(red_right_button);
    lv_obj_delete(blue_left_button);
    lv_obj_delete(blue_right_button);
    lv_obj_delete(skills_button);

    // resize image
    //lv_image_set_size_mode(field_image, lv_image_SIZE_MODE_REAL);
    lv_obj_align(field_image, LV_ALIGN_LEFT_MID, 20, 0);
    lv_image_set_scale(field_image, 256);

    // create back button
    back_button = lv_button_create(lv_screen_active());
    back_label = lv_label_create(back_button);
    lv_obj_add_event_cb(back_button, back_action, LV_EVENT_ALL, NULL);
    lv_obj_set_size(back_button, 80, 40);
    lv_obj_align(back_button, LV_ALIGN_RIGHT_MID, -155, -75);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Back");
    lv_obj_set_style_text_color(back_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_center(back_label);

    // create confirm button
    confirm_button = lv_button_create(lv_screen_active());
    confirm_label = lv_label_create(confirm_button);
    lv_obj_add_event_cb(confirm_button, confirm_action, LV_EVENT_ALL, NULL);
    lv_obj_set_size(confirm_button, 100, 40);
    lv_obj_set_style_bg_color(confirm_button, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    lv_obj_align(confirm_button, LV_ALIGN_RIGHT_MID, -20, -75);
    lv_label_set_text(confirm_label, LV_SYMBOL_OK " Confirm");
    lv_obj_set_style_text_color(confirm_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_center(confirm_label);

    // create previous auton button
    prev_auton_button = lv_button_create(lv_screen_active());
    prev_auton_label = lv_label_create(prev_auton_button);
    lv_obj_add_event_cb(prev_auton_button, prev_auton_action, LV_EVENT_ALL, NULL);
    lv_obj_set_size(prev_auton_button, 90, 40);
    lv_obj_align(prev_auton_button, LV_ALIGN_BOTTOM_RIGHT, -140, -20);
    lv_label_set_text(prev_auton_label, "Previous");
    lv_obj_set_style_text_color(prev_auton_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_center(prev_auton_label);

    // create next auton button
    next_auton_button = lv_button_create(lv_screen_active());
    next_auton_label = lv_label_create(next_auton_button);
    lv_obj_add_event_cb(next_auton_button, next_auton_action, LV_EVENT_ALL, NULL);
    lv_obj_set_size(next_auton_button, 90, 40);
    lv_obj_align(next_auton_button, LV_ALIGN_BOTTOM_RIGHT, -30, -20);
    lv_label_set_text(next_auton_label, "Next");
    lv_obj_set_style_text_color(next_auton_label, lv_color_black(), LV_PART_MAIN);
    lv_obj_center(next_auton_label);

    // create auton labels
    current_auton_score_label = lv_label_create(lv_screen_active());
    lv_obj_align(current_auton_score_label, LV_ALIGN_CENTER, 110, 25);
    lv_obj_set_style_text_font(current_auton_score_label, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_label_set_text(current_auton_score_label, "0 points");

    current_auton_name_label = lv_label_create(lv_screen_active());
    lv_obj_align(current_auton_name_label, LV_ALIGN_CENTER, 110, -5);
    lv_obj_set_style_text_font(current_auton_name_label, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_label_set_text(current_auton_name_label, "None");
}

static void red_right_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "red right clicked \n";
        selected_auton_group = RED_RIGHT;

        // set image offset
        lv_obj_set_size(field_image, 200, 200);
        lv_image_set_offset_y(field_image, -100);
        lv_image_set_offset_x(field_image, 100);

        // create buttons and labels
        create_secondary_screen();
    }
}

static void red_left_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "red left clicked \n";
        selected_auton_group = RED_LEFT;

        // set image offset
        lv_obj_set_size(field_image, 200, 200);
        lv_image_set_offset_y(field_image, 100);
        lv_image_set_offset_x(field_image, 100);
        
        // create buttons and labels
        create_secondary_screen();
    }
}

static void blue_right_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "blue right clicked \n";
        selected_auton_group = BLUE_RIGHT;

        // set image offset
        lv_obj_set_size(field_image, 200, 200);
        lv_image_set_offset_y(field_image, -100);
        lv_image_set_offset_x(field_image, -100);

        // create buttons and labels
        create_secondary_screen();
    }
}

static void blue_left_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "blue left clicked \n";
        selected_auton_group = BLUE_LEFT;
        
        // set image offset
        lv_obj_set_size(field_image, 200, 200);
        lv_image_set_offset_y(field_image, 100);
        lv_image_set_offset_x(field_image, -100);

        // create buttons and labels
        create_secondary_screen();
    }
}

static void skills_action(lv_event_t * event) {
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED) {
        std::cout << "skills clicked\n";
        selected_auton_group == SKILLS;

        // create buttons and labels
        create_secondary_screen();
        
        // resize image
        //lv_obj_delete(field_image);
        //field_image = lv_image_create(lv_screen_active());
        LV_IMAGE_DECLARE(push_back_skills_field_400);
        lv_image_set_src(field_image, &push_back_skills_field_400);
        lv_image_set_scale(field_image, 128);
        //lv_image_set_size_mode(field_image, lv_image_SIZE_MODE_VIRTUAL);
        lv_obj_set_size(field_image, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        //lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);
        lv_obj_align(field_image, LV_ALIGN_LEFT_MID, -80, 0);
    }
}

void lvgl_auton_selector() {
    // colors
    // confirm_task.suspend();
    // confirm_task.join();
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
    selected_auton_group = NONE;
    selected_auton_index = 0;

    // create styles
    lv_style_init(&style_button_bg);
    lv_style_set_bg_color(&style_button_bg, lv_color_hex(0xffffff));
    lv_style_set_bg_opa(&style_button_bg, LV_OPA_0);
    lv_style_set_radius(&style_button_bg, 0);

    lv_style_init(&style_button_red_press);
    lv_style_set_bg_color(&style_button_red_press, lv_palette_darken(LV_PALETTE_RED, 2));
    lv_style_set_bg_opa(&style_button_red_press, LV_OPA_50);
    lv_style_set_transform_height(&style_button_red_press, 0);
    lv_style_set_transform_width(&style_button_red_press, 0);
    
    lv_style_init(&style_button_blue_press);
    lv_style_set_bg_color(&style_button_blue_press, lv_palette_darken(LV_PALETTE_BLUE, 2));
    lv_style_set_bg_opa(&style_button_blue_press, LV_OPA_50);
    lv_style_set_transform_height(&style_button_blue_press, 0);
    lv_style_set_transform_width(&style_button_blue_press, 0);

    // create field image
    field_image = lv_image_create(lv_screen_active());
    LV_IMAGE_DECLARE(push_back_field_400);
    lv_image_set_src(field_image, &push_back_field_400);
    lv_image_set_scale(field_image, 128);
    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(field_image, LV_ALIGN_CENTER, 0, 0);

    // create red right button
    red_right_button = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(red_right_button, red_right_action, LV_EVENT_ALL, NULL);
    lv_obj_add_style(red_right_button, &style_button_bg, LV_PART_MAIN);
    lv_obj_add_style(red_right_button, &style_button_red_press, LV_STATE_PRESSED);
    lv_obj_set_size(red_right_button, 100, 100);
    lv_obj_align(red_right_button, LV_ALIGN_CENTER, -50, 50);

    // create red left button
    red_left_button = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(red_left_button, red_left_action, LV_EVENT_ALL, NULL);
    lv_obj_add_style(red_left_button, &style_button_bg, LV_PART_MAIN);
    lv_obj_add_style(red_left_button, &style_button_red_press, LV_STATE_PRESSED);
    lv_obj_set_size(red_left_button, 100, 100);
    lv_obj_align(red_left_button, LV_ALIGN_CENTER, -50, -50);

    // create blue right button
    blue_right_button = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(blue_right_button, blue_right_action, LV_EVENT_ALL, NULL);
    lv_obj_add_style(blue_right_button, &style_button_bg, LV_PART_MAIN);
    lv_obj_add_style(blue_right_button, &style_button_blue_press, LV_STATE_PRESSED);
    lv_obj_set_size(blue_right_button, 100, 100);
    lv_obj_align(blue_right_button, LV_ALIGN_CENTER, 50, -50);

    // create blue left button
    blue_left_button = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(blue_left_button, blue_left_action, LV_EVENT_ALL, NULL);
    lv_obj_add_style(blue_left_button, &style_button_bg, LV_PART_MAIN);
    lv_obj_add_style(blue_left_button, &style_button_blue_press, LV_STATE_PRESSED);
    lv_obj_set_size(blue_left_button, 100, 100);
    lv_obj_align(blue_left_button, LV_ALIGN_CENTER, 50, 50);

    // create skills button
    skills_button = lv_button_create(lv_screen_active());
    skills_label = lv_label_create(skills_button);
    lv_obj_add_event_cb(skills_button, skills_action, LV_EVENT_ALL, NULL);
    lv_obj_set_size(skills_button, 100, 100);
    lv_obj_align(skills_button, LV_ALIGN_RIGHT_MID, -20, 0);
    lv_label_set_text(skills_label, "Skills");
    lv_obj_center(skills_label);
}
