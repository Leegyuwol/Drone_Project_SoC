#include "main.h"

int main() {

    init_platform();
    myip_timerInterrupt_init();
    print("Start!\n");

    myip_bldcDriver_init();

    XUartLite_Initialize(&usb_uart_instance, USB_UART_ID);
    XUartLite_Initialize(&bluetooth_uart_instance, BLUETOOTH_UART_ID);

    XIic_Initialize(&iic_instance, IIC_ID);

	// Interrupt init
	XIntc_Initialize(&intc_instance, INTC_ID);
    XIntc_Connect(&intc_instance, TIMER_INTR_VEC_ID, (XInterruptHandler) timer_intr_handler, (void *) NULL);
    XIntc_Enable(&intc_instance, TIMER_INTR_VEC_ID);
    XIntc_Connect(&intc_instance, USB_UART_VEC_ID, (XInterruptHandler)XUartLite_InterruptHandler, (void *)&usb_uart_instance);
    XIntc_Enable(&intc_instance, USB_UART_VEC_ID);
    XIntc_Connect(&intc_instance, BLUETOOTH_UART_VEC_ID, (XInterruptHandler)XUartLite_InterruptHandler, (void *)&bluetooth_uart_instance);
    XIntc_Enable(&intc_instance, BLUETOOTH_UART_VEC_ID);
    XIntc_Start(&intc_instance, XIN_REAL_MODE);

    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XIntc_InterruptHandler, (void *) &intc_instance);
    Xil_ExceptionEnable();

    XUartLite_SetRecvHandler(&usb_uart_instance, usb_RecvHandler, &usb_uart_instance);
    XUartLite_SetSendHandler(&usb_uart_instance, usb_SendHandler, &usb_uart_instance);
    XUartLite_EnableInterrupt(&usb_uart_instance);
    XUartLite_SetRecvHandler(&bluetooth_uart_instance, bluetooth_RecvHandler, &bluetooth_uart_instance);
    XUartLite_SetSendHandler(&bluetooth_uart_instance, bluetooth_SendHandler, &bluetooth_uart_instance);
    XUartLite_EnableInterrupt(&bluetooth_uart_instance);

    uart_print(&bluetooth_uart_instance, "\n\nDrone (Rev. D) initializing, please wait...\n");

    MPU6050_Init();

    usleep(3000000);	// for motor to startup

    sampling_period_s = 0.01;
    myip_timerInterrupt_setInterval_us (timer0_interrupt_reg, sampling_period_s * 1000000.0 );
    myip_timerInterrupt_start (timer0_interrupt_reg);

    // Start receiving bluetooth byte
    XUartLite_Recv(&bluetooth_uart_instance, &rx_byte_buffer, 1);
    uart_print(&bluetooth_uart_instance, "Drone Ready. \"help\" for more command.\n\n");

    while(1);

    cleanup_platform();
    return 0;
}
