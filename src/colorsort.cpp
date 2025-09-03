#include "main.h"

bool ColorLock = false;

/*
 * PUSH BACK COLOR SORT
 * block will pass by color sensor
 * when it does activate trapdoor after a certain amt of time and for a certain duration (no need for distance sensor, use color sensor's distance)
**/

const pros::Color SortColor = Color::white;
int ccount = 0;   // wrong color counter
int ColorSort()
{
    optical_block.set_integration_time(3);
    int tcount = 0;   // detection threshold counter
    //int rcount = 0;   // total block counter
    int ncount = 0;   // no block counter
    
    while (true)
    {
        if (optical_block.get_proximity() < 50)                                                 // TUNE TS
        {
            tcount += 1;
            pros::Color DetectedColor = Color::white;
            if (optical_block.get_hue() < 100) DetectedColor = Color::red;
            if (optical_block.get_hue() > 100) DetectedColor = Color::blue;

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
                // REPLACE THIS with something that says when the block has passed, maybe a redundant check for a new color for more than a few cycles
                waitUntil(!BlockColor.isNearObject());
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

bool ColorStop;
pros::Color CurrentColor;
int BlockRelease() {
    pros::delay(300);
    if (!ColorLock) {
        std::cout << "releasing\n";
        pros::Color DetectedColor = Color::white;
        if (optical_block.get_hue() < 100) DetectedColor = Color::red;
        if (optical_block.get_hue() > 100) DetectedColor = Color::blue;
        ColorStop = true;
        Intake3.spin(reverse, 12, volt);
        waitUntil(!BlockColor.isNearObject() || BlockColor.color() != DetectedColor);
        wait(0.4, sec);
        ColorStop = false;
    }
    return 1;
}
