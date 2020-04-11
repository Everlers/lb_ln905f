#include "fun.h"
#include "hal_key.h"
#include "hal_adc.h"
#include "hal_uart.h"
#include "hal_flash.h"
#include "hal_buzzer.h"
#include "siliconControl.h"
#include "math.h"
#include "mch.h"
#include "ntc.h"
#include "pid.h"
#include "delay.h"
#include "tempCtrl.h"
#include "autoCaliBration.h"

fun_t xdata fun;
code u16 showTempGearListDC[FUN_MAX_TEMP_GEAR] = {160,180,200,220};//显示摄氏度
code u16 showTempGearListFC[FUN_MAX_TEMP_GEAR] = {320,360,400,440};//显示华氏度
code u16 setTempGearList[FUN_MAX_TEMP_GEAR]    = {143,170,200,220};//实际设定温度

void funInit (void)
{
	fun.state = 0;
	fun.tempGear = FUN_DEFAULT_TEMP_GEAR;
	fun.tempUnit = FUN_UNIT_FC;
	
	fun.tempSetTime = 0;
	fun.autoCloseTime = FUN_AUTO_CLOSE_TIME;
	fun.ledBlankTime = 0;
	fun.lowPowerTime = FUN_LOW_POWER_TIME;
	fun.ulTime = 0;
	
	if(scGetVolt() == SC_AC_VOLT_110V)
		fun.tempUnit = FUN_UNIT_FC;
	else
		fun.tempUnit = FUN_UNIT_DC;
	
	if(HalFlashDefault() != HAL_FLASH_DONE)//如果刚出厂未设定默认值
		funSaveFlash();//保存默认值
	funReadFlash();//读取保存的值
	
	#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
	if(!(HAL_KEY_PORT & HAL_KEY_SW_1))//如果上电按下电源键
	{
		delay_ms(20);
		if(!(HAL_KEY_PORT & HAL_KEY_SW_1))
		{
			autoc.state |= AUTOC_STA_RUN;//运行自动校准
			fun.state |= (FUN_STA_POWER|FUN_STA_ADJUST_MODE);//进入调节模式
			tcl.setTemp = FUN_ADJUST_TEMP;//设定校准温度
			mch.adjTemp = MCH_DEFAULT_ADJ_TEMP_OFFSET;//设置默认
			LED1 = 1;LED2 = 0;LED3 = 0;LED4 = 0;LED5 = 0;
			HalBuzzSetPeriod(150);
			while(!(HAL_KEY_PORT & HAL_KEY_SW_1));//等待按键松开
			delay_ms(200);
			HalKeyGetShortPress(HAL_KEY_ALL);
			HalKeyGetLongPress(HAL_KEY_ALL);
			HalBuzzerSet(2,150,70);
		}
	}
	#endif
}

void funReadFlash (void)
{
	//HalFlashRead(HAL_FLASH_DATA,HAL_FLASH_SHOW_UNIT_ADDR,(u8 *)&fun.tempUnit,sizeof(fun.tempUnit));//读取显示单位
	//HalFlashRead(HAL_FLASH_DATA,HAL_FLASH_SET_TEMP_ADDR,(u8 *)&fun.tempGear,sizeof(fun.tempGear));//读取温度挡位
}

void funSaveFlash (void)
{
	//HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_SHOW_UNIT_ADDR,(u8 *)&fun.tempUnit,sizeof(fun.tempUnit));//写入显示单位
	//HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_SET_TEMP_ADDR,(u8 *)&fun.tempGear,sizeof(fun.tempGear));//写入温度挡位
}

//显示管理
void funShowManage (void)
{
	if(tcl.state & TCL_STA_COLLECT_ERR)//如果温度采集故障
	{
		//温度采集故障提示
		if(fun.ledBlankTime == 0)
		{
			fun.ledStep = fun.ledStep ? 0:8;
			fun.ledBlankTime = 250;
		}
	}
	else if(fun.state & FUN_STA_POWER)//如果打开电源
	{
		if(fun.state & FUN_STA_ADJUST_MODE)//调节模式
		{
			#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_ENABLE == 1
			if(autoc.state & AUTOC_STA_RUN)//自动调节模式
			{
				#if AUTOC_EXTI_TEMP_COMMON == 0//内部NTC校准
				if(autoc.state & AUTOC_STA_NTC_HITCH)//如果NTC故障
				#else
				if(autoc.state & AUTOC_STA_COM_TIMEOUT)//如果通讯超时
				#endif
				{
					if(fun.ledBlankTime == 0)//全部灯闪烁
					{
						fun.ledBlankTime = 200;
						fun.ledStep = fun.ledStep ? 0:8;
					}
				}
				else if(autoc.state & AUTOC_STA_CALIBRATION_END)//如果结束校准(精确校准)
				{
					if(fun.ledBlankTime == 0)
						fun.ledStep = 8;//全亮
				}
				else if(autoc.state & AUTOC_STA_CALIBRATION_DONE)//如果校准完成
				{
					if(fun.ledBlankTime == 0)//循环流水灯
					{
						fun.ledBlankTime = 100;
						fun.ledStep++;
						if(fun.ledStep > 8)
							fun.ledStep = 5;
					}
				}
				else
				{
					if(fun.ledBlankTime == 0)//校准进度指示
					{
						if(autoc.state & AUTOC_STA_TEMP_CONT)	//进入恒温状态(快闪)
							fun.ledBlankTime = 200;
						else																	//等待温度稳定(慢闪)
							fun.ledBlankTime = 400;
						fun.ledStep = fun.ledStep ? 0:MIN_VALUE(autoc.CalibrationNum,4);
					}
				}
			}
			else//手动调节模式
			#endif
			{
				if(fun.ledBlankTime == 0)
				{
					fun.ledStep++;
					if(fun.ledStep > 4)
						fun.ledStep = 0;
					fun.ledBlankTime = 150;
				}
			}
		}
		else//正常模式
		{
			if(abs((int)tcl.curTemp - (int)tcl.setTemp) <= 8)
			{
				if(fun.showTempLockTime == 0 && fun.state & (FUN_STA_LOCK_DOWN|FUN_STA_LOCK_UP))
				{
					fun.state &= ~(FUN_STA_LOCK_DOWN|FUN_STA_LOCK_UP);
				}
			}
			else
				fun.showTempLockTime = 5000;
			
			if(fun.state & (FUN_STA_LOCK_DOWN|FUN_STA_LOCK_UP))//如果锁定温度
			{
				if(fun.ledBlankTime == 0)
				{
					if(fun.state & FUN_STA_LED_FLAG)
					{
						fun.ledStep = fun.tempGear+1;//显示当前挡位
						fun.state &= ~FUN_STA_LED_FLAG;
					}
					else
					{
						fun.ledStep = 0;
						fun.state |= FUN_STA_LED_FLAG;
					}
					fun.ledBlankTime = 300;
				}
			}
			else
			{
				fun.ledStep = fun.tempGear+1;//显示当前挡位
			}
			
			if(fun.state & FUN_STA_LOCK_SET)//如果锁定设置
				LED1 = 0;
			else
				LED1 = 1;
		}
	}
	else//电源关闭 显示OFF
	{
		fun.ledStep = 0;//关闭显示
		LED1 = 1;//关闭
	}

	switch (fun.ledStep)
	{
		case 0:LED2 = LED3 = LED4 = LED5 = 1;break;
		case 1:LED2 = 0;LED3 = LED4 = LED5 = 1;break;
		case 2:LED3 = 0;LED2 = LED4 = LED5 = 1;break;
		case 3:LED4 = 0;LED3 = LED2 = LED5 = 1;break;
		case 4:LED5 = 0;LED4 = LED2 = LED3 =  1;break;
		
		case 5:LED2 = 1;LED3 = 1;LED4 = 1;LED5 = 1;break;
		case 6:LED2 = 0;LED3 = 1;LED4 = 1;LED5 = 1;break;
		case 7:LED2 = 0;LED3 = 0;LED4 = 0;LED5 = 1;break;
		case 8:LED2 = 0;LED3 = 0;LED4 = 0;LED5 = 0;break;
	}
}

//控制管理
void funCtrlManage (void)
{
	if(HalKeyGetShortPress(HAL_KEY_POWER))//短按电源键
	{
		if(!(fun.state & FUN_STA_POWER))//如果未开启电源的状态(开启电源)
		{
			#if FUN_TEST_TEMP_GEAR == 1
			fun.testGearTime = FUN_TEST_GEAR_TIME;//刷新挡位停留时间
			fun.tempGear = 0;//第一档开始测试
			#endif
			fun.ledBlankTime = 0;//清除闪烁等待
			fun.state &= ~FUN_STA_LOCK_SET;//清除锁定设置
			fun.state |= FUN_STA_POWER|FUN_STA_LED_FLAG;//开启电源
			fun.ledStep = 0;//清除LED
			fun.tempGear = FUN_DEFAULT_TEMP_GEAR;//默认温度挡位
			fun.autoCloseTime = FUN_AUTO_CLOSE_TIME;//刷新自动关机时间
			tcl.setTemp = setTempGearList[fun.tempGear];//设定温度
			funShowUnlock();//解除显示锁定
			HalBuzzerSet(1,300,150);
		}
		else if(!(fun.state & FUN_STA_LOCK_SET)//电源开启的状态(关闭电源)
		#if AUTOC_ENABLE
			&& ((fun.state & FUN_STA_ADJUST_MODE && autoc.state & AUTOC_STA_CALIBRATION_DONE) || !(fun.state & FUN_STA_ADJUST_MODE))//如果开启了校准必须等校准完成才可关机
		#endif 
		)
		{
			if(fun.state & FUN_STA_ADJUST_MODE)//如果在调节模式
			{
				HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_ADJ_TEMP_ADDR,(u8 *)&mch.adjTemp,sizeof(mch.adjTemp));//写入调节温度
				debug("\r[manual calibration] Save adjust value: %f\r\n",mchAdjustValueGet());
			}
			fun.state &= ~(FUN_STA_POWER|FUN_STA_ADJUST_MODE);//关闭电源
			autoc.state &= ~AUTOC_STA_RUN;//关闭校准
			HalBuzzerSet(1,300,150);
		}
	}
	if(!(fun.state & FUN_STA_ADJUST_MODE))//非调整模式
	{
		if((HalKeyGetShortPress(HAL_KEY_SET) && !(fun.state & FUN_STA_LOCK_SET))//短按设置键 (切换温度挡位)
		#if FUN_TEST_TEMP_GEAR == 1
			|| (fun.state & FUN_STA_POWER && fun.testGearTime == 0 && !(fun.state & FUN_STA_ADJUST_MODE))
		#endif
		)
		{
			#if FUN_TEST_TEMP_GEAR == 1
			fun.testGearTime = FUN_TEST_GEAR_TIME;//刷新挡位停留时间
			#endif
			if(!(fun.state & FUN_STA_SET_TEMP))//如果未在设定状态
				fun.state |= FUN_STA_SET_TEMP;//进入温度设定状态
			fun.tempSetTime = FUN_TEMP_SET_TIME;//设置温度设定时间
			fun.tempGear ++;
			if(fun.tempGear >= FUN_MAX_TEMP_GEAR)
				fun.tempGear = 0;
			tcl.setTemp = setTempGearList[fun.tempGear];//设定温度
			funShowUnlock();//解除显示锁定
			fun.ledBlankTime = 0;
			fun.state |= FUN_STA_LED_FLAG;
			HalBuzzerSet(1,300,150);
		}
		if(HalKeyGetShortPress(HAL_KEY_LOCK))//短按锁定键 (锁定设置)
		{
			fun.state = (fun.state & FUN_STA_LOCK_SET) ? fun.state&~FUN_STA_LOCK_SET : fun.state|FUN_STA_LOCK_SET;
			HalBuzzerSet(3,300,150);
		}
	}
	else if(autoc.state & AUTOC_STA_CALIBRATION_END)//调节模式 且校准结束
	{
		if(HalKeyGetShortPress(HAL_KEY_LOCK))//短按锁定键
		{
			mchAdjustValueSet(mchAdjustValueGet()-1);
			fun.ledBlankTime = 100;
			fun.ledStep = 0;
			HalBuzzerSet(1,100,100);
			debug("\r[manual calibration] Adjust value: %f\r\n",mchAdjustValueGet());
		}
		if(HalKeyGetShortPress(HAL_KEY_SET))//短按设置键
		{
			mchAdjustValueSet(mchAdjustValueGet()+1);
			fun.ledBlankTime = 100;
			fun.ledStep = 0;
			HalBuzzerSet(1,100,100);
			debug("\r[manual calibration] Adjust value: %f\r\n",mchAdjustValueGet());
		}
	}

	if(fun.state & FUN_STA_POWER && fun.autoCloseTime == 0)//如果开机的状态 自动关机计时结束
	{
		tcl.state &= ~(TCL_STA_CONT_TEMP|TCL_STA_RUSH_TEMP);//复位恒温和冲温状态
		fun.state &= ~FUN_STA_POWER;//关闭电源
	}
	if(fun.state & FUN_STA_SET_TEMP && fun.tempSetTime == 0)//设定完成
	{
		fun.state &= ~FUN_STA_SET_TEMP;//取消温度设定
	}
	if(tcl.state & TCL_STA_COLLECT_ERR && fun.state & FUN_STA_POWER)//采集错误
	{
		tcl.state &= ~(TCL_STA_CONT_TEMP|TCL_STA_RUSH_TEMP);//复位恒温和冲温状态
		fun.state &= ~FUN_STA_POWER;//关闭电源
	}
}

void funTimeCallBack (void)
{
	static u16 sec = 0;
	if(fun.tempSetTime)
		fun.tempSetTime --;
	if(fun.showTempLockTime)
		fun.showTempLockTime--;
	if(fun.ledBlankTime)
		fun.ledBlankTime--;
	if(fun.ulTime)
		fun.ulTime--;

	sec++;
	if(sec >= 1000)
	{
		sec = 0;
		if(fun.autoCloseTime)
			fun.autoCloseTime--;
		if(fun.lowPowerTime)
			fun.lowPowerTime--;
#if FUN_TEST_TEMP_GEAR == 1
		if(fun.testGearTime)
			fun.testGearTime--;
#endif
	}
}

void funShowUnlock (void)
{
	fun.state &= ~(FUN_STA_LOCK_DOWN|FUN_STA_LOCK_UP);
	if(tcl.setTemp >= tcl.curTemp-10)
		fun.state |= FUN_STA_LOCK_UP;
	else
		fun.state |= FUN_STA_LOCK_DOWN;
	fun.showTempLockTime = 5000;//刷新解锁时间
}

