#include "hal_uart.h"
#include "stdio.h"
#include "protocol.h"
#include "fun.h"

void HalUARTInit(HalUartPort port,u32 baud)
{
	if(port == HAL_UART_0)
	{
		HalMultiplexingConfig(2,5,HAL_SYS_PM_TXD);
		//HalMultiplexingConfig(2,6,HAL_SYS_PM_RXD);
		SCON0 = 0x40;//打开通讯 
		//SCON0 |= 0x10;//使能接收
		PCON	|= 0x80;//波特率加倍
		FUNCCR 	&= 0xfe;//选择定时器1溢出时钟
		//ES0 = 1;//使能串口中断
	}
	else if(port == HAL_UART_1)
	{
		HalMultiplexingConfig(3,5,HAL_SYS_PM_TXD1);
		//HalMultiplexingConfig(2,1,HAL_SYS_PM_RXD1);
		SCON1 = 0x40;//打开通讯 
		//SCON1 |= 0x10;//使能接收
		PCON |= 0x04;//波特率加倍
		FUNCCR &= 0xFD;//选择定时器1溢出时钟
		//ES1 = 1;//使能串口中断
	}
	CKCON	|= 0x10;//定时器1 （Fsys/4） 4分频
	TMOD	&= 0x0F;//清除定时器1
	TMOD 	|= 0x20;//计数模式
	TH1		= 256-(HAL_SYSTEM_CLOCK*(((PCON&0x80)/0x80)+1)/32/4/baud);		//if selected fsys/12 ,4---12
	TL1 	= 256-(HAL_SYSTEM_CLOCK*(((PCON&0x80)/0x80)+1)/32/4/baud);
	TR1		= 1;							//开启定时器1
	TF1 	= 0;							//清除标志位
}

char putchar (char c)
{
	/*SBUF1 = c;//发送数据
	while(!(SCON1 & TI1));//等待发送完成
	SCON1 &= ~TI1;*/
	SBUF0 = c;//发送数据
	while(!TI0);//等待发送完成
	TI0 = 0;
	return c;
}

void HalUARTWrite(HalUartPort port,u8 *dat,u8 len)
{
	u8 i;
	if(port == HAL_UART_0){
		for(i=0;i<len;i++)
		{
			SBUF0 = dat[i];//发送数据
			while(!TI0);//等待发送完成
			TI0 = 0;
		}
	}
	else if(port == HAL_UART_1){
		for(i=0;i<len;i++)
		{
			SBUF1 = dat[i];//发送数据
			while(!(SCON1 & TI1));//等待发送完成
			SCON1 &= ~TI1;
		}
	}
}

void HalUART0Int(void)interrupt UART0_VECTOR
{
	if(TI0)
	{
		//TI0 = 0;
	}
	if(RI0)
	{
		RI0 = 0;
		#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_EXTI_TEMP_COMMON == 1
		ptlUartCallBack(SBUF0);
		#endif
	}
}

void HalUART1Int(void)interrupt UART1_VECTOR
{
	if(SCON1 & TI1)
	{
		//SCON1 &= ~TI1;
	}
	if(SCON1 & RI1)
	{
		SCON1 &= ~RI1;
	}
}