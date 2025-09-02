#pragma once

// Extra functions
float DeadBand(float input, float width);
float ToVolt(float percent);
float ToMM(float degrees);
float toDegrees(float angle);
float toRadians(float angle);
float reduce_radians(float angle);
float reduce_0_to_360(float angle);
void setInertial(float angle);
float trueHeading();
float trueRotation();

float convert_mps_to_volts(float velocity, float wheelSizeIn, float maxRPM);
float convert_volts_to_mps(float volts, float wheelSizeIn);
float convert_rpm_to_mps(float rpm, float wheelSizeIn);
float convert_mps_to_rpm();