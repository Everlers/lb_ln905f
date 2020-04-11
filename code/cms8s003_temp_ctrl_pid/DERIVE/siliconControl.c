#include "siliconControl.h"
#include "hal_uart.h"
#include "hal_adc.h"
#include "delay.h"
#include "tempCtrl.h"
#include "autoCalibration.h"

//*******************************2020-03-12********************************
//修复state未清除的问题。(上电半波控制)
//添加交流频率动态扫描来决定下一个导通时间。
//添加可控硅打开后的交流峰值回调接口。
//*************************************************************************

static siliconControl_t xdata sc;

void siliconControlInit (void)
{
	u8 i;
	for(i=0;i<SC_CTRL_NUM;i++){
		sc.duty[i] = 0;
	}
	sc.period = SC_DEFAULT_PERIOD;
	sc.wf = SC_AC_FREQ_NOT;
	sc.wv = SC_AC_VOLT_NOT;
	sc.state = 0;
	sc.zeroTime = 0;
	sc.count = 0;

	//关闭可控硅
  SCHEAT = 1;
	
	//外部中断初始化
	HalMultiplexingConfig(0,4,HAL_SYS_PM_DEFAULT);
	HalExtiCfg(0,4,HAL_EXTI_DOUBLE);//配置双边沿中断
	HalExtiIntEn(0,4);//配置中断使能
	
	//用于下次过零信号计数的定时器3
	T34MOD = (1<<3)|(0<<2)|(1<<0);//不开启定时器3 Fsys/12 模式1 16位定时器
	TH3 = 0;
	TL3 = 0;
	EIE2 |= ET3;//允许定时器3中断
	
	//用于触发结束计数的定时器4
	T34MOD |= (0<<7)|(0<<6)|(1<<4);//不开启定时器4 Fsys/12 模式1 16位定时器
	TH4	=	0;//清除计数器
	TL4	=	0;
	EIE2 |= ET4;//允许定时器4中断

	//用于动态检测交流频率的定时器2 2Mhz 
	T2CON = 0x00;//停止定时器2
	TH2 = 0x00;
	TL2 = 0x00;
	CCEN = 0x00;//禁止所有捕获/比较
	T2IF = 0;//清除定时器2所有中断标志位
	T2IE = T2OVIE;//允许溢出中断
	ET2 = 1;//允许定时器2中断
	T2CON = 0x01;//开启定时器2

	delay_ms(500);//丢去10个中断
	debug("[siliconControl] power check...\r\n");
	scPowerCheck();//检测AC电源
}

//电源检测
void scPowerCheck(void)
{
	float volt = 0;
	float mcuVolt = 0;
	float hz;
	u8 i = 0;
	for(i=0;i<20;i++)//丢弃20个旋波
	{
		sc.wf = SC_AC_FREQ_NOT;
		while(sc.wf == SC_AC_FREQ_NOT);//等待零点
	}
	wait:
	sc.wf = SC_AC_FREQ_NOT;
	while(sc.wf == SC_AC_FREQ_NOT);//等待零点
	if(!ZERO_IO)goto wait;//如果是正旋波(对MCU来说是负电压，不可采集)
	if(sc.wf == SC_AC_FREQ_60HZ)//等待电压到最高点
		delay_ms(4);
	else
		delay_ms(5);
	volt = HalAdcRead(SC_VOLT_CHANNEL);//读取电压值
	mcuVolt = HalAdcRead(ADC15);//读取MCU电压
	mcuVolt = 1.2 * 4096.0 / mcuVolt;//计算MCU电压
	volt = mcuVolt * volt / 4096.0;//计算IO电压
	volt = (mcuVolt - volt) / SC_DOWN_RES * SC_UP_RES;//计算电源电压
	if(volt >= 220)//220V
		sc.wv = SC_AC_VOLT_220V;
	else
		sc.wv = SC_AC_VOLT_110V;
	hz = 1000000.0 / (float)sc.zeroTime / 2.0;
	debug("[siliconControl] mcuVolt: %0.2fv\r\n",mcuVolt);
	debug("[siliconControl] acVolt: %0.2fv\r\n",volt);
	debug("[siliconControl] acFreq: %0.2fHz\r\n",hz);
}


powerVoltage scGetVolt(void)
{
	return sc.wv;
}

powerFrequency scGetFreq(void)
{
	return sc.wf;
}

void scPeriodSet (u16 period)
{
	sc.period = period;
}

u16 scPeriodGet (void)
{
	return sc.period;
}

void scDutySet (u8 port,u16 duty)
{
	if(duty >= sc.period)duty = sc.period;
	sc.duty[port] = duty;
}

u16 scDutyGet (u8 port)
{
  return sc.duty[port];
}

//1：全波控制 0：半波控制
void scWaveConfig(bit sw)
{
	if(sw)sc.state |= SC_STA_ALL_SAVE;
	else sc.state &= ~SC_STA_ALL_SAVE;
}

//外部中断
void EXTI0_IRQHandler(void)  interrupt P0EI_VECTOR
{
	u16 cnt;
  HalExtiIntClr(0,4);//清除外部中断标志位
  T34MOD &= ~TR3;//停止定时器3计数
	cnt = ((u16)TH2)<<8|TL2;//读取电源频率
	TH2 = TL2 = 0x00;//清除计数器
	cnt >>= 1;//计算AC频率
	if(cnt >= 7000 && cnt <= 11000)//如果频率采集正确
	{
		sc.zeroTime = cnt;//赋值电源频率
	  if(sc.wf == SC_AC_FREQ_NOT)//如果没有初始化电源频率
	  {
			if(sc.zeroTime >= 9150)
				sc.wf = SC_AC_FREQ_50HZ;
			else
				sc.wf = SC_AC_FREQ_60HZ;
	  }
	}
	if(sc.wf != SC_AC_FREQ_NOT)//如果已经初始化电源
	{
		if(!ZERO_IO)
	  	cnt = 65535-(sc.zeroTime + SC_TRIGG_PLACE_P)*2;//设定下一次提前开启的时间
	  else
	  	cnt = 65535-(sc.zeroTime + SC_TRIGG_PLACE_R)*2;//设定下一次提前开启的时间
		TH3	=	HI_UINT16(cnt);
		TL3	=	LO_UINT16(cnt);
		T34MOD |= TR3;//开启定时器
	}
}

//定时器2溢出中断 注意：此中断不可进入，否则交流频率检测(外部中断)不正常。
void TIM2_IRQHandler(void) interrupt TIMER2_VECTOR
{
	T2IF = 0;//清除所有中断标志位
	TH2 = TL2 = 0xFF;
}

//定时器3溢出中断
void TIM3_IRQHandler(void) interrupt TIMER3_VECTOR
{
	//EIF2 &= ~TF3;//清除中断标志位
	T34MOD &= ~TR3;//关闭定时器3
	
	sc.count++;
	
	if(sc.duty[0] >= sc.count){//如果需要触发
		if(sc.state & SC_STA_ALL_SAVE)//如果全波控制
			SCHEAT = 0;//打开可控硅
		else if(!ZERO_IO)//如果是负旋波
			SCHEAT = 0;//打开可控硅
		else//正旋波不导通
			sc.count--;
	}
	
	if(SCHEAT == 0)//如果有触发
	{
		TH4	=	HI_UINT16(65535-SC_TRIGG_TIME*2);//设定导通时间
		TL4	=	LO_UINT16(65535-SC_TRIGG_TIME*2);//设定计数器
		T34MOD |= TR4;//开始脉冲宽度计数
	}
	
	if(sc.count >= sc.period)//如果周期完成
		sc.count = 0;

	SC_AC_FREQ_HOOK();//统一采集AD
}

//定时器4溢出中断
void TIM4_IRQHandler(void) interrupt TIMER4_VECTOR
{
	//EIF2 &= ~TF4;//清除中断标志位
	T34MOD &= ~TR4;//关闭定时器4

	if(SCHEAT == 0)//如果需要关闭可控硅
	{
		//关闭可控硅
		SCHEAT = 1;
		//计算交流电峰值
		if(ZERO_IO)//如果是负选波
		{
			TH4	=	HI_UINT16(65535-((sc.zeroTime/2)-SC_TRIGG_TIME)*2);//设定采集时间
			TL4	=	LO_UINT16(65535-((sc.zeroTime/2)-SC_TRIGG_TIME)*2);//设定计数器
			T34MOD |= TR4;//开始交流峰值计数
		}
	}
	#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
	else//到达交流峰值(采集峰值电压电流)
	{
		SC_AC_PEAK_VALUE_HOOK();//开始计算采集时间
	}
	#endif
}
