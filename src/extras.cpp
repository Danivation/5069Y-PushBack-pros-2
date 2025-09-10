#include "main.h"

// wheel diameter in mm (52.62 before) (50.375 actual)
// lower number = overshoot (600 actually reports as bigger than 600)
const float TrackerWheelSize = 2 * 25.4;
const float in_to_m = 0.0254;

// Extra functions
float DeadBand(float input, float width) {
    if (fabs(input) < width) {
        return 0;
    }
    return input;
}

float ToVolt(float percent) {
    return (percent*12.0/100.0);
}

float ToMM(float degrees) {
    float TrackerRatio = (M_PI*TrackerWheelSize)/360;
    return (degrees / TrackerRatio);  // drivetrain gear ratio
}

float toRadians(float angle) {
    // degrees to radians
    return angle * (M_PI / 180);
}

float toDegrees(float angle) {
    // radians to degrees
    return angle * (180 / M_PI);
}

float reduce_0_to_360(float angle) {
    // reduce to 0-360
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}

float reduce_radians(float angle) {
    // reduce to 0-2pi
    while (angle < 0) angle += 2.0f * M_PI;
    while (angle >= 2.0f * M_PI) angle -= 2.0f * M_PI;
    return angle;
}

float convert_mps_to_volts(float velocity, float wheelSizeIn, float maxRPM) {
    float wheelSizeM = wheelSizeIn * in_to_m;
    return (60 * velocity)/(wheelSizeM * M_PI * maxRPM) * 12;
}

float convert_volts_to_mps(float volts, float wheelSizeIn) {
    return convert_rpm_to_mps(volts * 50, wheelSizeIn);
}

float convert_rpm_to_mps(float rpm, float wheelSizeIn) {
    const float gearRatio = 36.0f/48.0f;
    float wheelSizeM = wheelSizeIn * in_to_m;
    return (M_PI * wheelSizeM * rpm * gearRatio)/60.0f;
}

float convert_mps_to_rpm() {
    return 1;
}

void setInertial(float angle) {
    imu_1.set_rotation(angle);
    imu_1.set_heading(angle);
}

float trueHeading() {
    // rotation is -inf to +inf, so averaging them will not have issues of 360-0 rollover
    // reducing back to 0-360 afterwards makes sure its in a good format for everything else
    // if (imu_1.is_installed() && imu_2.is_installed()) return reduce_0_to_360((imu_1.get_rotation() + imu_2.get_rotation()) / 2);
    // if (!imu_1.is_installed()) return reduce_0_to_360(imu_2.get_rotation());
    // if (!imu_2.is_installed()) return reduce_0_to_360(imu_1.get_rotation());
    // else return 0.0f;
    return 0;
}

float trueRotation() {
    // returns -inf to +inf true rotation
    // return ((imu_1.get_rotation() + imu_2.get_rotation()) / 2);
    return 0;
}
