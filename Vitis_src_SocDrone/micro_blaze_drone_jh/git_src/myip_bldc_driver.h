#ifndef __MYIP_BLDC_DRIVER_H_
#define __MYIP_BLDC_DRIVER_H_

#include "main.h"

#define NUM_BLDC_MOTOR 4

#define MOTOR0_MINDUTY 38
#define MOTOR1_MINDUTY 39
#define MOTOR2_MINDUTY 33
#define MOTOR3_MINDUTY 13

#define BLDC_MOTOR_BASEADDR 	XPAR_MYIP_DRONE_BLDC_MOTO_0_S00_AXI_BASEADDR

s32 bldc_power_arr[NUM_BLDC_MOTOR];
volatile s32* motor_power_reg;


void myip_bldcDriver_init();
void myip_bldcDriver_setPower_float(float *motor_power_value_float_arr);
void myip_bldcDriver_turnAllOff();
void myip_bldcDriver_manualPower(s32 motor_power);

#endif /* __MYIP_BLDC_DRIVER_H_ */
