#ifndef __INTC_H_
#define __INTC_H_

#include "main.h"

#define INTC_ID 				XPAR_INTC_SINGLE_DEVICE_ID

#define USB_UART_VEC_ID 		XPAR_INTC_0_UARTLITE_0_VEC_ID
#define BLUETOOTH_UART_VEC_ID 	XPAR_INTC_0_UARTLITE_1_VEC_ID

XIntc intc_instance;

#endif
