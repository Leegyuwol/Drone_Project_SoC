#include "uart.h"
#include "xuartlite_l.h"

u8 rx_buffer_index = 0;
u8 is_uart_receiving = 0;

#define PARSE_BUFFER_SIZE 20
static char8 parse_buffer[PARSE_BUFFER_SIZE];

void usb_SendHandler(void *CallBackRef, unsigned int EventData)
{
	// Intentially left blank
}

void usb_RecvHandler(void *CallBackRef, unsigned int EventData)
{
	// Intentially left blank
}

void bluetooth_SendHandler(void *CallBackRef, unsigned int ByteCount)
{
	while (XUartLite_IsSending(&bluetooth_uart_instance));		// TODO: seems not affecting anything
}

void bluetooth_RecvHandler(void *CallBackRef, unsigned int ByteCount)
{
	// Process character
	rx_buffer[rx_buffer_index++] = rx_byte_buffer;		// use "rx_buffer_index" first then increment it
	if ((rx_buffer[rx_buffer_index-1] == '\r' ||  rx_buffer[rx_buffer_index-1] == '\n') &&  (rx_buffer[rx_buffer_index-2] == '\r' ||  rx_buffer[rx_buffer_index-2] == '\n') )
	{
		process_command(rx_buffer);
		rx_buffer_index = 0;
	}
	if (rx_buffer_index >= RX_BUFFER_SIZE-1) rx_buffer_index = 0;	// buffer overflow prevention

	XUartLite_Recv(&bluetooth_uart_instance, &rx_byte_buffer, 1);
}

// NOTE: *str_ptr++ is same as *(str_ptr++), not (*str_ptr)++
void uart_print(XUartLite *uart_inst_ptr, const char8* str_ptr)
{
	while (*str_ptr != 0)
	{
		XUartLite_SendByte(uart_inst_ptr->RegBaseAddress, *str_ptr++);
	}
}

void process_command (char8 *str_ptr)
{
	str_ptr = parse_command(str_ptr, parse_buffer);	// parse first command
	str_ptr++;							// increment str_handle pointer to skip space character

	if (is_str_equal(parse_buffer, "off"))
	{
		// Turn off motor
		motor_mode_var = MOTOR_OFF;
		uart_print(&bluetooth_uart_instance, "Motor off\n");
	}
	else if (is_str_equal(parse_buffer, "manual"))
	{
		// Set motor power manually
		u32 input_integer = parse_integer(str_ptr);
		motor_power_manual = (u8) input_integer;
		motor_mode_var = MOTOR_MANUAL;
		uart_print(&bluetooth_uart_instance, "Motor power set to: ");
		print_integer(&bluetooth_uart_instance, input_integer);
		uart_print(&bluetooth_uart_instance, "\n");
	}
	else if (is_str_equal(parse_buffer, "pid"))
	{
		// Turn on PID mode
		motor_mode_var = MOTOR_PID;
		uart_print(&bluetooth_uart_instance, "Motor PID mode\n");
	}
	else if (is_str_equal(parse_buffer, "set"))
	{
		str_ptr = parse_command(str_ptr, parse_buffer);
		str_ptr++;
		float input_float = parse_float(str_ptr);
		if (is_str_equal(parse_buffer, "kp"))
		{
			Kp_roll = input_float;
			Kp_pitch = input_float;
			uart_print(&bluetooth_uart_instance, "OK: Kp changed\n");
		}
		else if (is_str_equal(parse_buffer, "ki"))
		{
			Ki_roll = input_float;
			Ki_pitch = input_float;
			uart_print(&bluetooth_uart_instance, "OK: Ki changed\n");
		}
		else if (is_str_equal(parse_buffer, "kd"))
		{
			Kd_roll = input_float;
			Kd_pitch = input_float;
			uart_print(&bluetooth_uart_instance, "OK: Kd changed\n");
		}
		else
		{
			uart_print(&bluetooth_uart_instance, "Error\n");
		}
	}
	else if (is_str_equal(parse_buffer, "show"))
	{
		str_ptr = parse_command(str_ptr, parse_buffer);
		str_ptr++;
		if (is_str_equal(parse_buffer, "pid"))
		{
			// TODO: Send all pid constant values
		}
		else
		{
			uart_print(&bluetooth_uart_instance, "Error\n");
		}
	}
	else if (is_str_equal(parse_buffer, "help"))
	{
		uart_print(&bluetooth_uart_instance, "List of available commands:\n");
		uart_print(&bluetooth_uart_instance, "off: turn off motor\n");
		uart_print(&bluetooth_uart_instance, "manual <0..255>: set motor power\n");
		uart_print(&bluetooth_uart_instance, "pid: set motor to pid mode\n");
		uart_print(&bluetooth_uart_instance, "set (kp|ki|kd) <float>: set pid constant\n");
		uart_print(&bluetooth_uart_instance, "reset: reset PID integral accumulated value\n");
		uart_print(&bluetooth_uart_instance, "sample <float>: change sampling period (in second)\n");
		uart_print(&bluetooth_uart_instance, "help: display this message\n");
	}
	else if (is_str_equal(parse_buffer, "reset"))
	{
		internal_motor_power_float[0] = 0;
		internal_motor_power_float[1] = 0;
		internal_motor_power_float[2] = 0;
		internal_motor_power_float[3] = 0;
		uart_print(&bluetooth_uart_instance, "Motor power reset done\n");
	}
	else if (is_str_equal(parse_buffer, "sample"))
	{
		myip_timerInterrupt_stop (timer0_interrupt_reg);
		sampling_period_s = parse_float(str_ptr);
	    myip_timerInterrupt_setInterval_us (timer0_interrupt_reg, sampling_period_s * 1000000.0 );
	    uart_print(&bluetooth_uart_instance, "Sampling period changed to ");
	    print_float(&bluetooth_uart_instance, sampling_period_s);
	    myip_timerInterrupt_start (timer0_interrupt_reg);
	}
	else
	{
		uart_print(&bluetooth_uart_instance, "Error\n");
	}

}

// NOTE: *str_ptr++ is same as *(str_ptr++), not (*str_ptr)++
char8 * parse_command(char8 *input_buffer, char8 *output_buffer)
{
	// reset buffer
	for (int i=0; i<PARSE_BUFFER_SIZE; ++i) output_buffer[i] = 0;
	
	while( (*input_buffer >= 'A' && *input_buffer <= 'Z') || (*input_buffer >= 'a' && *input_buffer <= 'z') )
	{
		*output_buffer++ = *input_buffer++;
	}

	return input_buffer;
}

// only process positive integer
// NOTE: *str_ptr++ is same as *(str_ptr++), not (*str_ptr)++
u32 parse_integer(const char8 *str_ptr)
{
	u32 result = 0;
	while (*str_ptr >= '0' && *str_ptr <= '9')
	{
		result = result * 10 + (*str_ptr++ - '0');
	}

	return result;
}

// NOTE: *str_ptr++ is same as *(str_ptr++), not (*str_ptr)++
float parse_float(const char8 *str_ptr)
{
	float result = 0;
	while (*str_ptr >= '0' && *str_ptr <= '9')
	{
		result = result * 10 + (*str_ptr++ - '0');
	}
	if (*str_ptr++ == '.')
	{
		u16 decimal_num_index = 1;
		while (*str_ptr >= '0' && *str_ptr <= '9')
		{
			u32 ten_powers = 1;
			for (int i=0; i<decimal_num_index; ++i) ten_powers *= 10;
			result = result + (float) (*str_ptr++ - '0') / (float) ten_powers;
			++decimal_num_index;
		}
	}
	return result;
}

// compare two string equality
// Does not check for string index overflow
u8 is_str_equal (const char8 *str1_ptr, const char8 *str2_ptr)
{
	u8 result = 1;
	u8 index = 0;

	do
	{
		if (str1_ptr[index] != str2_ptr[index])
		{
			result = 0;
			break;
		}
		index++;
	}
	while (str1_ptr[index] && str2_ptr[index]);

	return result;
}

/*
 * Print integer to uart
 * 
 * fill output buffer starting with least significant digit
 * at the end of print_buffer except last bit, which is NULL character
 * and then use the first actual number character in the buffer.
 * 
 * EX: int_input is 5142
 * print_buffer is x x x ... x x 5 1 4 2 \0
 *                               ^ print_buffer_handle indicates here at the end
 *
 * NOTE 1: operator '--' precedes operator '*'.
 * NOTE 2: increment/decrement postfix (ex: var++) uses the original variable value
 *			first, and then increment/decrement its value.
 *
 */
void print_integer (XUartLite *uart_inst_ptr, u32 int_input)
{
	char8 print_buffer[PARSE_BUFFER_SIZE];
	print_buffer[PARSE_BUFFER_SIZE-1] = '\0';		// add NULL character at the end
	char8 *print_buffer_handle = print_buffer + (PARSE_BUFFER_SIZE-2);	// start filling buffer from the end, except NULL at the very end
	while (int_input)
	{
		*print_buffer_handle-- = (int_input % 10) + '0';	// read NOTE above for explanation
		int_input /= 10;
		if (print_buffer_handle == print_buffer) break;		// if the buffer is about to overflow, stop parsing and print
	}
	uart_print(uart_inst_ptr, ++print_buffer_handle);
}


/*
 * Print float to uart
 *
 * First, shift decimal point to the left until the float is less than 1. 
 *		Record number of shifts
 * Second, shift decimal point to the right one by one, and put the ones into buffer.
 *		The digit that was put to the buffer will be substracted to be removed.
 *		When this was repeated recorded number of decimal point shifts, add decimal point to print buffer.
 * 		Iteration stops when the float is zeroed out.
 *
 * NOTE: integer digits are fixed, and can be used with for loop, but
 *			decimal digits are not fixed since it potentially
 *
 *
 * Example:
 *		Suppose float of 314.9523 is to be printed:
 *
 *	At the end of first step:
 *		the float = 0.3149523 / num_shift = 3
 *
 *	Second step iteration 1:
 *		the float = 3.149523 -> 0.149523
 *		print_buffer 3 x x x x x x x x ...
 *
 *	Second step iteration 2:
 *		the float = 1.49523 -> 0.49523
 *		print_buffer 3 1 x x x x x x x ...
 *
 *	Second step iteration 3 (decimal point added):
 *		the float = 4.9523 -> 0.9523
 *		print_buffer 3 1 4 . x x x x x ...
 *
 *	Second step iteration 4:
 *		the float = 9.523 -> 0.523
 *		print_buffer 3 1 4 . 9 x x x x ...
 *
 *	...rest iteration skipped...
 *
 *	Second step iteration 7 (last):
 *		the float = 3 -> 0 (end condition)
 *		print_buffer 3 1 4 . 9 5 2 3 x ...
 *
 *
 */
void print_float (XUartLite *uart_inst_ptr, float float_input)
{
	// get num digits of integer part 
	u32 num_shift = 0;
	while (float_input > 1.0)
	{
		float_input /= 10.0;
		++num_shift;
	}

	// put the float to the buffer
	char8 print_buffer[PARSE_BUFFER_SIZE];
	char8 *print_buffer_handle = print_buffer;	// assign handle to the start of the buffer
	u32 index = 0;	// to determine decimal point insertion index
	while (float_input)
	{
		if (index != num_shift) 
		{
			float_input *= 10.0;
			u8 float_input_integer_part = (u8) float_input;		// only one digit of integer part at a time
			*print_buffer_handle++ = float_input_integer_part + '0';	// truncate the float to ones digit
			float_input -= (float) float_input_integer_part;	// remove the digit that was put to the buffer
		}
		else /* index == num_shift */
		{
			if (!num_shift) *print_buffer_handle++ = '0';	// if there is no integer part, add zero
			*print_buffer_handle++ = '.';
		}
		++index;	// unnecessary increment operation does exist
		if (print_buffer_handle == print_buffer+(PARSE_BUFFER_SIZE-1)) break; // if the buffer is about to overflow, stop parsing and print
	}
	print_buffer_handle = '\0';	// put NULL character at the end

	uart_print(uart_inst_ptr, print_buffer);
}
