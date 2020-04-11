#ifndef _HAL_UART_H_
#define _HAL_UART_H_
#include "hal_sys.h"
#include "stdio.h"

#define debug			if(1)printf

typedef enum{
	HAL_UART_0,
	HAL_UART_1,
}HalUartPort;

void HalUARTInit(HalUartPort port,u32 baud);
void HalUARTWrite(HalUartPort port,u8 *dat,u8 len);

#endif