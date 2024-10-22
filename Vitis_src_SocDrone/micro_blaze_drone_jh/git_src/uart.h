#ifndef __UART_H_
#define __UART_H_

#include "main.h"

extern float internal_motor_power_float[4];

#define USB_UART_ID 			XPAR_UARTLITE_0_DEVICE_ID
#define BLUETOOTH_UART_ID 		XPAR_UARTLITE_1_DEVICE_ID

#define RX_BUFFER_SIZE 20

XUartLite 	usb_uart_instance;
XUartLite 	bluetooth_uart_instance;

u8 rx_byte_buffer;
char8 rx_buffer[RX_BUFFER_SIZE];
u8 rx_buffer_index;
u8 is_uart_receiving;

typedef enum
{
	MOTOR_OFF,
	MOTOR_MANUAL,
	MOTOR_PID
} Motor_Mode;

Motor_Mode motor_mode_var;
u8 motor_power_manual;


void usb_SendHandler(void *CallBackRef, unsigned int EventData);
void usb_RecvHandler(void *CallBackRef, unsigned int EventData);
void bluetooth_SendHandler(void *CallBackRef, unsigned int EventData);
void bluetooth_RecvHandler(void *CallBackRef, unsigned int EventData);
void uart_print(XUartLite *uart_inst_ptr, const char8* str_ptr);

void process_command (char8 *str_ptr);
char8 * parse_command(char8 *input_buffer, char8 *output_buffer);
u32 parse_integer(const char8 *str_ptr);
float parse_float(const char8 *str_ptr);
u8 is_str_equal (const char8 *str1_ptr, const char8 *str2_ptr);
void print_integer (XUartLite *uart_inst_ptr, u32 int_input);
void print_float (XUartLite *uart_inst_ptr, float float_input);

#endif
