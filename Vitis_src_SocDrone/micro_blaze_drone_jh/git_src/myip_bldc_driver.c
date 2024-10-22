#include "myip_bldc_driver.h"

void myip_bldcDriver_init()
{
    motor_power_reg = (volatile s32*) BLDC_MOTOR_BASEADDR;
}

void myip_bldcDriver_setPower_float(float *motor_power_value_float_arr)
{
    for (u32 i=0; i<NUM_BLDC_MOTOR; ++i)
    {
        motor_power_reg[i] = (s32) motor_power_value_float_arr[i];
    }
}

void myip_bldcDriver_turnAllOff()
{
    for (u32 i=0; i<NUM_BLDC_MOTOR; ++i)
    {
        motor_power_reg[i] = 0;
    }
}

void myip_bldcDriver_manualPower(s32 motor_power)
{
    for (u32 i=0; i<NUM_BLDC_MOTOR; ++i)
    {
        motor_power_reg[i] = motor_power;
    }
}
