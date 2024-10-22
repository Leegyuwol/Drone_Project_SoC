#include "i2c_motion_sensor.h"

float alpha = 0.98;     // gyro - 98% , accel - 2%
float threshold = 0.1;  // gyro offset correction


void MPU6050_Write(u8 reg, u8 data) {
    u8 buffer[2] = {reg, data};
    XIic_Send(iic_instance.BaseAddress, MPU6050_ADDR, buffer, 2, XIIC_STOP);
}


void MPU6050_Read(u8 reg, u8 *buffer, u16 len) {
	XIic_Send(iic_instance.BaseAddress, MPU6050_ADDR, &reg, 1, XIIC_STOP);
    XIic_Recv(iic_instance.BaseAddress, MPU6050_ADDR, buffer, 14, XIIC_STOP);
}

void MPU6050_ReadAccelGyro(s16 *accel_data, s16 *gyro_data) {
    u8 buffer[14];

    MPU6050_Read(0x3B, buffer, 14);

    accel_data[0] = (buffer[0] << 8) | buffer[1];
    accel_data[1] = (buffer[2] << 8) | buffer[3];
    accel_data[2] = (buffer[4] << 8) | buffer[5];

    gyro_data[0] = (buffer[8] << 8) | buffer[9];
    gyro_data[1] = (buffer[10] << 8) | buffer[11];
    gyro_data[2] = (buffer[12] << 8) | buffer[13];
}

void MPU6050_Init(void) {
    MPU6050_Write(0x6B, 0x00);
}


float calculateAccelRoll(s16 accel_x, s16 accel_y, s16 accel_z)
{
    return atan2f(accel_y, sqrtf(accel_x * accel_x + accel_z * accel_z)) * (180 / M_PI);
}


float calculateAccelPitch(s16 accel_x, s16 accel_y, s16 accel_z)
{
    return atan2f(accel_x, sqrtf(accel_y * accel_y + accel_z * accel_z)) * -(180 / M_PI);
}




void calculate_Offset(s16 accel_data[3], s16 gyro_data[3], int samples)
{
    for (int i = 0; i < samples; i++)
    {
    	MPU6050_ReadAccelGyro(accel_data, gyro_data);

    	accel_sum[0] += accel_data[0];
    	accel_sum[1] += accel_data[1];
    	accel_sum[2] += accel_data[2];

    	gyro_sum[0] += gyro_data[0];
    	gyro_sum[1] += gyro_data[1];
    	gyro_sum[2] += gyro_data[2];

    	usleep(10000);
    }

    /*Average*/
    accel_offset[0] = accel_sum[0] / samples;
    accel_offset[1] = accel_sum[1] / samples;
    accel_offset[2] = accel_sum[2] / samples;

    gyro_offset[0] = gyro_sum[0] / samples;
    gyro_offset[1] = gyro_sum[1] / samples;
    gyro_offset[2] = gyro_sum[2] / samples;
}

float complementaryFilter(float angle_accel, float angle_gyro, float alpha, float gyro_rate, float threshold)
{
    // if drone stop
    if (fabs(gyro_rate) < threshold)
    {
        return angle_accel;
    }
    else	// if drone working
    {
    	return alpha * (angle_gyro) + (1.0 - alpha) * angle_accel;
    }
}
