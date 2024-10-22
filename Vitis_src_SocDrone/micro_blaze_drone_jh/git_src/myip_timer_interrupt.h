#ifndef __MYIP_TIMER_INTERRUPT_H_
#define __MYIP_TIMER_INTERRUPT_H_

#include "main.h"

#define SYSCLK_TO_US 100	// sysclk period (10ns) to 1us ratio

volatile unsigned int *timer0_interrupt_reg;
float internal_motor_power_float[4];

#define TIMER_INTR_VEC_ID XPAR_INTC_0_MYIP_TIMER_INTERRUPT_0_VEC_ID
#define TIMER_INTR_BASEADDR XPAR_MYIP_TIMER_INTERRUPT_0_S00_AXI_BASEADDR


void myip_timerInterrupt_init();
void myip_timerInterrupt_setInterval_us (volatile unsigned int * timer_reg, int interval_us);
void myip_timerInterrupt_start (volatile unsigned int * timer_reg);
void myip_timerInterrupt_stop (volatile unsigned int * timer_reg);
void timer_intr_handler(void *CallBackRef);

#endif /* __TIMER_INTERRUPT_H__ */
