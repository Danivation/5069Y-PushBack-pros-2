#include "main.h"

bool ColorLock = false;

/*
 * PUSH BACK COLOR SORT
 * block will pass by color sensor
 * when it does activate trapdoor after a certain amt of time and for a certain duration (no need for distance sensor, use color sensor's distance)
**/

pros::Color get_color(pros::Optical* sensor) {
    float hue = sensor->get_hue();

    if (hue < 30 || hue > 330) {
        return pros::Color::red;
    } else if (hue < 90) {
        return pros::Color::yellow;
    } else if (hue < 150) {
        return pros::Color::green;
    } else if (hue < 270) {
        return pros::Color::blue;
    } else {
        return pros::Color::white;
    }
}

bool ColorStop;
pros::Color CurrentColor;
int BlockRelease() {
    pros::delay(300);
    if (!ColorLock) {
        std::cout << "releasing\n";
        pros::Color DetectedColor = get_color(&optical_block);
        ColorStop = true;
        intake_front.move_voltage(-12000);
        waitUntil(!optical_block.get_proximity() < 50 || get_color(&optical_block) != DetectedColor);
        pros::delay(400);
        ColorStop = false;
    }
    return 1;
}

pros::Color SortColor = Color::white;
void ColorSort() {
    const int detection_cycle_time = 10;    // 10 ms * 10 cycles = 100 ms between an object being detected and the color being confirmed
    // todo: make it not recheck the color if the same object is still there or do make it recheck or something
    const int detection_threshold = 7;      // 7/10 of the cycles have to be the correct color for it to count
    // todo: make it recheck if its between 3-7 cause its not really sure
    int object_streak = 0;
    int detection_strength = 0;
    while (true) {
        if (optical_block.get_proximity() < 100) {
            // log that an object has been detected
            object_streak += 1;

            // if the object is the color that we want to sort out, log that the correct color has been detected
            pros::Color detected_color = get_color(&optical_block);
            if (detected_color == SortColor) {
                detection_strength += 1;
            } /* else {
                detection_strength = 0;
            } */
        } else {
            object_streak = 0;
            detection_strength = 0;
        }
        // once an object has been detected for 10 cycles, check how strongly the sorting color was found throughout those cycles
        if (object_streak >= 10) {
            if (detection_strength >= 7) {
                printf("block detected with strength %i", detection_strength);
                pros::Task release1(BlockRelease);
            }
            object_streak = 0;
            detection_strength = 0;
        }
        pros::delay(10);
    }
}

pros::Color SortColor2 = Color::white;
int ccount = 0;   // wrong color counter
int ColorSort2()
{
    int tcount = 0;   // detection threshold counter
    //int rcount = 0;   // total block counter
    int ncount = 0;   // no block counter
    
    while (true)
    {
        if (optical_block.get_proximity() < 50)                                                 // TUNE TS
        {
            tcount += 1;
            pros::Color DetectedColor = get_color(&optical_block);

            if (DetectedColor == SortColor)
            {
                ccount += 1;
            }
            
            if (tcount >= 5 && ccount >= 8)    // if its been near an object for more than 9 cycles
            {
                std::cout << "block detected (color) with strength " << ccount << " after " << ncount << "\n";
                ncount = 0;
                tcount = 0;
                pros::Task release1(BlockRelease);
                // REPLACE THIS with something that says when the block has passed,
                // maybe a redundant check for a new color for more than a few cycles
                waitUntil(!optical_block.get_proximity() < 50);                                    // TUNE TS
                ccount = 0;
            }
            pros::delay(3);
        }
        else
        {
            tcount = 0;
            ncount += 1;
            pros::delay(1);
            ccount = 0;
        }
    }
}
