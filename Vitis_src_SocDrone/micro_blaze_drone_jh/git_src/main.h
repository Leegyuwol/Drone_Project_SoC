/*
 * Drone Overview
 *
 *		-pitch
 * 		 0   1
 * +roll	 x sensor	-roll
 * 		 3   2
 * 		 +pitch
 *
 *			min duty	direction
 * Motor 0:	38			CCW
 * Motor 1:	39			CW
 * Motor 2:	33			CCW
 * Motor 3:	13			CW
 */

#ifndef __MAIN_H_
#define __MAIN_H_

// standard include
#include <stdio.h>

// Xilinx include
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xiic.h"
#include "math.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xuartlite.h"
#include "sleep.h"

// custom include
#include "myip_timer_interrupt.h"
#include "myip_bldc_driver.h"
#include "uart.h"
#include "intc.h"
#include "i2c_motion_sensor.h"
#include "pid_control.h"

float sampling_period_s;	//sampling period in second


#endif /* __MAIN_H_ */

