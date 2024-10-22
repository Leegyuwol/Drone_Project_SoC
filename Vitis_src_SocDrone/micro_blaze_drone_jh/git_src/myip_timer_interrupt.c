#include "myip_timer_interrupt.h"

float internal_motor_power_float[4] = {0, };

/*
 * Link timer interrupt module control register
 */
void myip_timerInterrupt_init()
{
	timer0_interrupt_reg = (volatile unsigned int *) TIMER_INTR_BASEADDR;
}

/*
 * Set interrupt interval
 * @param interval_us: desired interrupt interval in us
 */
void myip_timerInterrupt_setInterval_us (volatile unsigned int * timer_reg, int interval_us)
{
	timer_reg[1] = interval_us * SYSCLK_TO_US;
}

/*
 * Turn on timer interrupt module
 */
void myip_timerInterrupt_start (volatile unsigned int * timer_reg)
{
	timer_reg[0] = 1;
}

/*
 * Turn off timer interrupt module
 */
void myip_timerInterrupt_stop (volatile unsigned int * timer_reg)
{
	timer_reg[0] = 0;
}

void timer_intr_handler(void *CallBackRef)
{
	static float integral_roll, integral_pitch;

	if (motor_mode_var == MOTOR_OFF)
	{
		myip_bldcDriver_turnAllOff();
	}
	else if (motor_mode_var == MOTOR_MANUAL)
	{
		myip_bldcDriver_manualPower(motor_power_manual);
	}
	else if (motor_mode_var == MOTOR_PID)
	{
		/******** JH code  *********/

	   	// read accel, gyro data
	   	MPU6050_ReadAccelGyro(accel_data, gyro_data);

	   	// accel - Roll,Pitch
		roll_accel = calculateAccelRoll(accel_data[0], accel_data[1], accel_data[2]);
		pitch_accel = calculateAccelPitch(accel_data[0], accel_data[1], accel_data[2]);

		// TODO: remove gyro sensor error to remove roll/pitch value error accumulation
		// gyro - Roll,Pitch
		// roll_gyro += (gyro_data[0]) / 131.0 * dt;
		// pitch_gyro += (gyro_data[1]) / 131.0 * dt;

		// TODO: verify complimentary filter viability
	   	// Complementary Filter
		// roll_filtered = complementaryFilter(roll_accel, roll_gyro, alpha);
		// pitch_filtered = complementaryFilter(pitch_accel, pitch_gyro, alpha);

	   	float error_roll, error_pitch;
	   	error_roll = PID_Control(0, roll_accel, &integral_roll, Kp_roll, Ki_roll, Kd_roll, sampling_period_s);
	   	error_pitch = PID_Control(0, pitch_accel, &integral_pitch, Kp_pitch, Ki_pitch, Kd_pitch, sampling_period_s);

	   	internal_motor_power_float[0] = MOTOR0_MINDUTY - error_roll + error_pitch;
	   	internal_motor_power_float[1] = MOTOR1_MINDUTY + error_roll + error_pitch;
	   	internal_motor_power_float[2] = MOTOR2_MINDUTY + error_roll - error_pitch;
	   	internal_motor_power_float[3] = MOTOR3_MINDUTY - error_roll - error_pitch;

		for (u8 i=0; i<4; ++i)
		{
			if (internal_motor_power_float[i] > 255.0) internal_motor_power_float[i] = 255.0;
			else if (internal_motor_power_float[i] < 0.0) internal_motor_power_float[i] = 0.0;
		}

		myip_bldcDriver_setPower_float(internal_motor_power_float);

		// printf("%03.3f %03.3f / %03.3f %03.3f / %3d %3d %3d %3d\n", roll_filtered, pitch_filtered, error_roll, error_pitch, motor_power_reg[0], motor_power_reg[1], motor_power_reg[2], motor_power_reg[3]);
	}

}
