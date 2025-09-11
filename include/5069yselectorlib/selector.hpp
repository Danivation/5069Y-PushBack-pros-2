#pragma once
#include <string>
#include <vector>

// Auton selector
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
    void (*callback)();

    auton_descriptor_t(const std::string& n, const std::string& s = "0 points", void (*cb)() = nullptr)
        : name(n), score(s), callback(cb) {}
};

void lvgl_auton_selector(
    const std::vector<auton_descriptor_t> red_right,
    const std::vector<auton_descriptor_t> red_left,
    const std::vector<auton_descriptor_t> blue_right,
    const std::vector<auton_descriptor_t> blue_left,
    const auton_descriptor_t skills
);
std::pair<auton_mode_t, auton_descriptor_t> get_selected_auton();

extern int selected_auton_index;
extern auton_mode_t selected_auton_group;

extern const std::vector<auton_descriptor_t> red_right_autons;
extern const std::vector<auton_descriptor_t> red_left_autons;
extern const std::vector<auton_descriptor_t> blue_right_autons;
extern const std::vector<auton_descriptor_t> blue_left_autons;
extern const auton_descriptor_t skills_auton;
