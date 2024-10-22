#include "pid_control.h"


float PID_Control(float target_angle, float current_angle, float* integral, float Kp, float Ki, float Kd, float dt)
{
	static float prev_angle;

    float error = target_angle - current_angle;
    float angle_diff = current_angle - prev_angle;
    prev_angle = current_angle;

    *integral += error * dt;

    float output = (Kp * error) + (Ki * (*integral)) - (Kd * angle_diff / dt);

    return output;
}
