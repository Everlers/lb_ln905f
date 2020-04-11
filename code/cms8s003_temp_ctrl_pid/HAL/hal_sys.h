#ifndef _HAL_SYS_H_
#define _HAL_SYS_H_
#include "CMS8S003.h"

#define HAL_SYSTEM_CLOCK	24000000uL
#define CLRWDT()					TA=0xAA, TA=0x55, WDTCON=0x01;
#define BV(n)							(1<<(n))
#define MIN_VALUE(n,n1)		(((n)<(n1)) ? (n):(n1))
#define MAX_VALUE(n,n1)   (((n)>(n1)) ? (n):(n1))
#define HI_UINT16(a) 			(((a) >> 8) & 0xFF)
#define LO_UINT16(a) 			((a) & 0xFF)
#define UINT16(hi,lo) 		(((u16)(hi) << 8) | lo)
#define Q15(n)						(long)((n)*32767.0L)
#define Q31(n)						(long)((n)*2147483647.0L)

#define HalMultiplexingConfig(port,pin,fun)				(P##port##pin##CFG) = fun			//配置为:通用IO/外设使用
#define HalExtiCfg(port,pin,cfg)									(P##port##pin##EICFG) = cfg		//外部中断边沿配置
#define HalExtiIntEn(port,pin)  									(P##port##EXTIE) |= BV(pin)		//使能端口中断
#define HalExtiIntDs(port,pin)										(P##port##EXTIE) &= ~BV(pin)	//失能端口中断
#define HalExtiIntClr(port,pin)										(P##port##EXTIF) &= ~BV(pin)	//清除外部中断标志位

//@HalExtiCfg
#define HAL_EXTI_NOT					0x00//禁用
#define HAL_EXTI_UP						0x01//上升沿
#define HAL_EXTI_DOWN					0x02//下降沿
#define HAL_EXTI_DOUBLE				0x03//双边沿

//@HalMultiplexingConfig
#define HAL_SYS_PM_DEFAULT		0
#define HAL_SYS_PM_AN0_C0P1		1
#define HAL_SYS_PM_AN1_C0P2		1
#define HAL_SYS_PM_AN2_C1P2		1
#define HAL_SYS_PM_AN3_C1P1 	1
#define HAL_SYS_PM_AN4_C1P0		1
#define HAL_SYS_PM_AN5_C1N		1
#define HAL_SYS_PM_AN6_C0P0		1
#define HAL_SYS_PM_AN7_C0N		1
#define HAL_SYS_PM_AN8_OP1_P	1
#define HAL_SYS_PM_AN9_OP1_N	1
#define HAL_SYS_PM_AN10_OP1_O	1
#define HAL_SYS_PM_AN11				1
#define HAL_SYS_PM_AN12				1
#define HAL_SYS_PM_AN13_OP0_N	1
#define HAL_SYS_PM_AN14_OP0_P	1
#define HAL_SYS_PM_P15_PG0		5
#define HAL_SYS_PM_P16_PG1		5
#define HAL_SYS_PM_P17_PG4		6
#define HAL_SYS_PM_P24_PG2		6
#define HAL_SYS_PM_TXD				2
#define HAL_SYS_PM_RXD				2
#define HAL_SYS_PM_TXD1				2
#define HAL_SYS_PM_RXD1				2

typedef	short			q15_t;
typedef long 			q31_t;
typedef long int 	q63_t;

void HalSysInit(void);
void HalSysEnterInt(void);
void HalSysExtiInt(void);
#endif