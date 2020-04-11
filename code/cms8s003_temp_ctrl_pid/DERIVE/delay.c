#include "delay.h"

u32 xdata delayMs;

void delayTask(void)
{
	if(delayMs)
		delayMs--;
}

void delay_ms(u32 ms)
{
	delayMs = ms;
	while(delayMs);
}
