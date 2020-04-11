#include "hal_sys.h"

bit eaF;

void HalSysInit(void)
{
	P0 = BV(0);
	P1 = BV(3)|BV(4)|BV(5)|BV(6)|BV(7);
	P2 = BV(3);
	P3 = 0x00;
	//模拟功能使能 0:digital IO 			1:analogy
	P0ANS		=	0x00;
	P1ANS		=	0x00;
	P2ANS		=	BV(2)|BV(4)|BV(6);
	P3ANS		=	0x00;
	//配置输入输出 0:input mode 			1:output mode
	P0TRIS		=	BV(0); 
	P1TRIS		=	BV(3)|BV(4)|BV(5)|BV(6)|BV(7);
	P2TRIS		=	BV(3)|BV(5);
	P3TRIS		=	BV(5);
	//开漏控制 0:normal output 		1:opendrain output
	P0OD		=	0x00;
	P1OD		=	0x00;
	P2OD		=	0x00;
	P3OD		=	0x00;
	//上拉控制 0:disable	pull high	1:enable pull high
	P0UP		=	BV(1)|BV(2)|BV(3);
	P1UP		=	0x00;
	P2UP		=	BV(1);
	P3UP		=	0x00;
	//下拉控制 0:disbale pull low	1:enable pull low
	P0RD		=	0x00;
	P1RD		=	0x00;
	P2RD		=	0x00;
	P3RD		=	0x00;
	//驱动电流控制 0:strong driving		1:weak driving
	P0DR		=	0x00;
	P1DR		=	0x00;
	P2DR		=	0x00;
	P3DR		=	0x00;
	//斜率控制 0:Fast slope			1:Slow slope
	P0SR		=	0x00;
	P1SR		=	0x00;
	P2SR		=	0x00;
	P3SR		=	0x00;
	
	EA = 1;//允许中断
}

void HalSysEnterInt(void)
{
	eaF = EA;
	EA = 0;
}

void HalSysExtiInt(void)
{
	EA = eaF;
}
