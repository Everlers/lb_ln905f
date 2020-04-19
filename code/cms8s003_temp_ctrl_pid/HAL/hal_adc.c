#include "hal_adc.h"
#include "hal_pwm.h"

u8 adcTimeout; 
static u8 channel;
static u16 adcValue;
static u8 con0 = 0,con1 = 0,con2 = 0;

//Vref:选择参考电压源
//vrefSel:选择参考电压 
//Channel:typedef enum initChannel
//intEn:使能中断模式
//adsEn:使能采集方式
void HalAdcInit(halAdcCLK clock,HalAdcIntState intEn)
{
	//配置IO为ADC外设使用
	HalMultiplexingConfig(2,6,HAL_SYS_PM_AN12);
	HalMultiplexingConfig(2,4,HAL_SYS_PM_AN10_OP1_O);
	HalMultiplexingConfig(2,2,HAL_SYS_PM_AN8_OP1_P);

	ADCON0 = BV(6);//向右对齐 通道15 1.2V	
  ADCON1 = BV(7)|clock<<4;//使能ADC 设定ADC时钟
  
  //ADCON2 = BV(7)|(1<<4)|(3<<2);//开启硬件触发AD|PG2触发源|PWM周期的终点触发
	//ADDLYL = 1;//延迟触发时间

	con0 = ADCON0;
	con1 = ADCON1;
	con2 = ADCON2;
	
  EIF2 &= ~ADCIF;//清除中断标志位
  EIE2 |= intEn << 4;//设定中断允许位
}


//开始ADC采集 
void HalAdcStart(u8 channel)
{
	ADCON1 &= 0xF0;
	ADCON1 |= channel;
	ADCON0 |= ADGO;
}

//切换采集通道
void HalAdcChangeChannel(u8 channel)
{
	ADCON1 &= 0xF0;
	ADCON1 |= channel;
}

u16 HalAdcRead(u8 channel)
{
	START:
	ADCON1 &= 0xF0;
	ADCON1 |= channel;
	ADCON0 |= ADGO;
	adcTimeout = 5;
	while(ADCON0 & ADGO && adcTimeout);
	if(adcTimeout == 0)//芯片BUG 超时重新初始化AD
	{
		ADCON0 = ADCON1 = ADCON2 = 0;//清除AD
		//重新初始化ADC
		ADCON0 = con0;
		ADCON1 = con1;
		ADCON2 = con2;
		goto START;
	}
	return ADRESL|(u16)ADRESH<<8;    
}

u16 HalAdcReadNum(u8 channel,u8 num)
{
	u32 adcValue = 0;
	u8 i;
	for(i=0;i<num;i++){
		adcValue += HalAdcRead(channel);
	}
	return (adcValue / num);
}


//整个采集380us左右
void ADC_IRQHandler(void)  interrupt ADC_VECTOR
{
	EIF2 &= ~ADCIF;//清除中断标志位
	channel = ADCON1 & 0x0F;
	adcValue = ADRESL|(u16)ADRESH<<8;
	switch (channel)
	{
		default:
			//HalAdcChangeChannel(ADC_1_2V);
		break;
	}
}
