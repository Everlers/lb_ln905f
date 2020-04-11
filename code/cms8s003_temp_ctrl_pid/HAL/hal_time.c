#include "hal_time.h"
#include "hal_adc.h"
#include "hal_key.h"
#include "hal_buzzer.h"
#include "delay.h"
#include "fun.h"
#include "tempCtrl.h"
#include "protocol.h"
#include "autoCalibration.h"

u16 TM0Preloading;

void HalTimeInit(u16 us)
{
	TMOD |= 0x01;//定时器0模式1 16位定时/计数器
	CKCON |= 0x00;//定时器0时钟分频 1:Fsys/4 0:Fsys/12
	TH0	=	(65535-(us*(HAL_SYSTEM_CLOCK/12000000)))/256;//设定计数器
	TL0	=	(65535-(us*(HAL_SYSTEM_CLOCK/12000000)));//设定计数器
	TM0Preloading = (65535-(us*(HAL_SYSTEM_CLOCK/12000000)));//设定预装载值
	TCON &= ~0x20;//清除中断标志位
	IE |= 0x02;//开启定时器中断
	TCON |= 0x10;//启动定时器
}

void HalTime2Init (void)
{
	//配置定时器2用作计算峰值电压采集所有信号
	T2CON = 0x00;//停止定时器2
	TH2 = 0;
	TL2 = 0;
	CCEN = 0x00;//禁止所有捕获/比较
	T2IF = 0;//清除定时器2所有中断标志位
	T2IE = T2OVIE;//允许溢出中断
	ET2 = 1;//允许定时器2中断
}

void HalTimeInterrput (void) interrupt TIMER0_VECTOR
{
	TH0	=	TM0Preloading/256;
	TL0	=	TM0Preloading;
	TCON &= ~0x20;//清除中断标志位
	//以下加入定时任务
	#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_ENABLE == 1
	autoCalibrationTimeCallBack();
	#if AUTOC_EXTI_TEMP_COMMON
	ptlTimeCallBack();
	#endif
	#endif
	HalKeyTask();
	delayTask();
	funTimeCallBack();
	tclTimeHook();
	HalBuzzerTask();
	if(adcTimeout)
		adcTimeout--;
}