#pragma once
#include "main.h"

enum auton_mode_t {
    NONE = 0,
    RED_LEFT = 1,
    RED_RIGHT = 2,
    BLUE_LEFT = 3,
    BLUE_RIGHT = 4,
    SKILLS = 5,
};
struct auton_descriptor_t {
    std::string name;
    std::string score;
    std::string path_file;
    std::string callback_name;

    auton_descriptor_t(const std::string& n, const std::string& s, const std::string& p = "none.txt", const std::string& cb = "")
        : name(n), score(s), path_file(p), callback_name(cb) {}
};

extern int selected_auton_index;
extern auton_mode_t selected_auton_group;
extern const std::vector<auton_descriptor_t> red_right_autons;
extern const std::vector<auton_descriptor_t> red_left_autons;
extern const std::vector<auton_descriptor_t> blue_right_autons;
extern const std::vector<auton_descriptor_t> blue_left_autons;
extern const auton_descriptor_t skills_auton;

//extern std::vector<std::vector<VelocityLayout>> mp_paths;

void lvgl_auton_selector();

// Auton functions
void autonNone();