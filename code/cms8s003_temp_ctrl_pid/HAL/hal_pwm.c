#include "hal_pwm.h"
#include "hal_adc.h"

void HalPwmInit(void)
{
	HalMultiplexingConfig(1,7,HAL_SYS_PM_P17_PG4);
	PWMCON = 0x00;//独立模式 
	//PWMOE = BV(4);//使能PWM通道n的输出
	PWM45PSC = 0x01;//fsys/(PWMxxPSC+1)
	PWM4DIV = 0x00;//PWM通道n时钟分频 Fpwmn-PSC/2
	PWMPINV = 0x00;//输出极性 0:正常输出 1:反相输出
	PWMLOADEN = BV(4);//PWM0LE=1 使能加载周期，占空比数据(PERIODn、 CMPn、 CMPDn)。
	PWMCNTM = BV(4);//PWMn自动加载模式
	PWMCNTCLR = BV(4);//PWM通道n计数器清零控制位(n=0-5)
	
	PWMP4L = LO_UINT16(HAL_PWM_EPREIOD_VAL);//PWMn周期
	PWMP4H = HI_UINT16(HAL_PWM_EPREIOD_VAL);
	
	PWMD4H = HI_UINT16(HAL_PWM_EPREIOD_VAL/2);//占空比
	PWMD4L = LO_UINT16(HAL_PWM_EPREIOD_VAL/2);
	
	PWMDTE = 0x00;//PWM 死区使能控制寄存器 
	PWM45DT = 50;//死区时间(PWM01DT+1)*TPWM0
	PWMMASKE = 0x00;//PWM通道n掩码控制使能位 (n=0-5)
	PWMMASKD = 0x00;//PWM通道n掩码数据位 1= PWMn通道输出高 0= PWMn通道输出低
	PWMFBKD = 0x00;
	PWMZIF = 0;//清除零点中断标志位
	PWMUIF = 0;//清除向上比较中断标志位
	PWMDIF = 0;//清除向下比较中断标志位
	PWMZIE = 0;//使能通道n零点中断
	PWMUIE = 0;//使能通道n向上比较中断
	PWMDIE = 0;//使能通道n向下比较中断
	//EIE2 |= PWMIE;//使能PWM中断
	PWMCNTE = BV(4);//通道n计数器开启
}

//设置占空比
void HalPwmSetDutyCycle(u16 duty)
{
	if(duty)
	{
		PWMD4H = duty>>8;
		PWMD4L = duty;
		PWMLOADEN = BV(4);//PWM0LE=1 使能加载周期，占空比数据(PERIODn、 CMPn、 CMPDn)。
		PWMOE |= BV(4);//PWM输出
	}
	else
	{
		PWMOE &= ~BV(4);
		P1_7 = 0;
	}
}
extern bit enCurrent;

void HalPWMInterrupt(void) interrupt PWM_VECTOR
{
	/*u8 channel;
	if((PWMZIE && PWMZIF && fun.curDuty >= HAL_PWM_EPREIOD_VAL) || //PWM零点中断
		 (PWMDIE && PWMDIF && fun.curDuty  < HAL_PWM_EPREIOD_VAL))//PWM向下比较中断
	{
		while(ADCON0 & ADGO);//等待上个采集完成
		channel = ADCON1 & 0x0F;//保存上次采集通道
		
		bms.ADC1_2Volt = HalAdcRead(ADC_1_2V);//采集MCU电压 用于延迟
		bms.current = HalAdcRead(CURRENT_CHANNEL);//采集电机电流
		
		EIF2 &= ~ADCIF;//清除中断标志位
		HalAdcStart(channel);//恢复采集
		
		PWMDIF = 0;
		PWMZIF = 0;
	}
	if((fun.hState & HSTA_PROTECT) && (bms.current >= SHORTCURRENT))//如果到达短路电流
	{
		bms.shortTime ++;
		if(bms.shortTime >= SHORT_TIME){//如果短路时间超出
			//短路事件处理
			PWMOE = 0x00;//关闭PWM通道的输出
			P2_4 = 0;//关闭主电机
			MOTOR = 0;//关闭小电机
			fun.hState |= HSTA_SHORT;//标志短路
		}
	}
	else
		bms.shortTime = 0;*/
}