#ifndef __PID_CONTROL_H_
#define __PID_CONTROL_H_

float Kp_roll, Ki_roll, Kd_roll;
float Kp_pitch, Ki_pitch, Kd_pitch;
float target_roll, target_pitch;

float roll_integral, pitch_integral;


float PID_Control(float target_angle, float current_angle, float* integral, float Kp, float Ki, float Kd, float dt);

#endif
