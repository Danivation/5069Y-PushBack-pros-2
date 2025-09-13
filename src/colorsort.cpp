#include "main.h"

/**
 * COLOR SORT MODES
 * 
 * 1 (driver default): sorted blocks go out of the middle goal area
 * 2 (driver long goal): sorted blocks go out of the long goal area, for when we are scoring in the middle goal
 * 3 (auton): sorted blocks go back into storage
 */
int ColorSortMode = 1;
bool ColorSortEnabled = true;
bool ColorStop = false;
pros::Color SortColor = Color::white;

pros::Color get_color(pros::Optical* sensor) {
    float hue = sensor->get_hue();

    if (hue < 30 || hue > 330) {        // 330 to 360, 0 to 30
        return pros::Color::red;
    } else if (hue < 90) {              // 30 to 90
        return pros::Color::yellow;
    } else if (hue < 150) {             // 90 to 150
        return pros::Color::green;
    } else if (hue < 270) {             // 150 to 270
        return pros::Color::blue;
    } else {                            // 270 to 330
        return pros::Color::white;
    }
}

void BlockRelease() {
    printf("releasing block\n");
    if (ColorSortEnabled) {
        ColorStop = true;
        pros::Color DetectedColor = get_color(&optical_block);
        pros::delay(10);
        if (ColorSortMode == 1) {    // mode 1
            pros::delay(100);
            intake_front.move(-127);
        } else if (ColorSortMode == 2) {
            intake_front.move(127);
            pros::delay(400);
            hood_piston.retract();
        } else if (ColorSortMode == 3) {
            intake_front.move(127);
            pros::delay(400);
            hood_piston.extend();
        }
        //waitUntilCondition(optical_block.get_proximity() < 50 || get_color(&optical_block) != DetectedColor);
        pros::delay(200);
        ColorStop = false;
    }
}

/**
 * COLOR SORT MODES
 * 
 * 1 (driver default): sorted blocks go out of the middle goal area
 * 2 (driver long goal): sorted blocks go out of the long goal area, for when we are scoring in the middle goal
 * 3 (auton): sorted blocks go back into storage
 */
void ColorSort() {
    const int detection_cycle_time = 20;    // 10 ms * 10 cycles = 100 ms between an object being detected and the color being confirmed
    // todo: make it not recheck the color if the same object is still there or do make it recheck or something
    const int detection_threshold = 10;      // 7/10 of the cycles have to be the correct color for it to count
    // todo: make it recheck if its between 3-7 cause its not really sure
    const int proximity_threshold = 50;     // closer object -> higher proximity
    int object_streak = 0;
    int detection_strength = 0;
    while (true) {
        
        // PROXIMITY IS HIGHER AS IT GETS CLOSER!
        if ((int)optical_block.get_proximity() >= proximity_threshold) {
            // log that an object has been detected
            object_streak += 1;

            // if the object is the color that we want to sort out, log that the correct color has been detected
            pros::Color detected_color = get_color(&optical_block);
            if (detected_color == SortColor) {
                detection_strength += 1;
            }
        } else {
            object_streak = 0;
            detection_strength = 0;
        }
        // once an object has been detected for 20 cycles nonstop, check how strongly the sorting color was found throughout those cycles
        if (object_streak >= detection_cycle_time) {
            if (detection_strength >= detection_threshold) {
                printf("block detected with strength %i\n", detection_strength);
                pros::Task release1(BlockRelease);
            } else {
                printf("block detected with strength %i but not released\n", detection_strength);
            }
            object_streak = 0;
            detection_strength = 0;
        }
        pros::delay(10);
    }
}
