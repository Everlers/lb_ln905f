#include "test.h"
#include "siliconControl.h"
#include "fun.h"
#include "encryption.h"
#include "hal_key.h"
#include "delay.h"
#include "hal_buzzer.h"

void test (void)
{
	u8 i;
	//encryptionCancelEmpower();//取消授权
	for(i=0;i<20;i++)//检查是否放入测试架
	{
		EP_TX = !EP_TX;
		_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();
		_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();
		_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();
		_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();
		_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();
		_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();_nop6_();
		if(EP_RX != EP_TX)
			break;
	}
	if(i == 20)//(放入测试架)
	{
		encryptionEmpower();//将此芯片授权
		LED3 = 0;
		scWaveConfig(1);//全波控制
		HalBuzzSetPeriod(200);
		while(!HalKeyGetShortPress(HAL_KEY_POWER))//等待按下电源键
		{
			if(HalKeyGetShortPress(HAL_KEY_SET))//按下设定键
			{
				scDutySet(SC_PORT_HEAT,scPeriodGet());//打开可控硅
				LED1 = LED2 = LED3 = LED4 = LED5 = 0;//打开所有指示灯
			}
			if(HalKeyGetShortPress(HAL_KEY_LOCK))//按下锁定键
			{
				scDutySet(SC_PORT_HEAT,0);//关闭可控硅
				LED1 = LED2 = LED3 = LED4 = LED5 = 1;//关闭所有指示灯
			}
		}
		
		//测试 零点-电压
		if(scGetFreq() != SC_AC_FREQ_50HZ)//检查电源频率
		{
			LED2 = 0;
			while(1);
		}
		if(scGetVolt() != SC_AC_VOLT_220V)
		{
			LED3 = 0;
			while(1);
		}
		HalBuzzerSet(3,150,70);
	}
}
