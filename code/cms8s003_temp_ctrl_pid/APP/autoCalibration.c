#include "autoCalibration.h"
#include "fun.h"
#include "mch.h"
#include "ntc.h"
#include "protocol.h"
#include "string.h"
#include "hal_adc.h"
#include "hal_uart.h"
#include "hal_flash.h"
#include "math.h"
#include "tempCtrl.h"

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_ENABLE == 1

autoCalibration_t xdata autoc;
#if AUTOC_EXTI_TEMP_COMMON == 1
volatile protocol_t xdata recvP;
#endif

void autoCalibrationInit(void)
{
	autoc.state = 0;
	autoc.combTemp = 0;
	autoc.oldTemp = 0;
	autoc.CalibrationNum = 0;

	autoc.adCombTemp = 0x8000;
	autoc.adMcuVolt = 0;
	autoc.combTempNum = 0;
	autoc.combTempCnt = 0;
	
	autoc.nextCalibrationTime = 0;
	autoc.conTempTime = AUTOC_TEMP_CONT_TIME;
	autoc.commonTimeout = AUTOC_COMMON_TIMEOUT;
	autoc.time = 0;
	
	#if AUTOC_EXTI_TEMP_COMMON == 1
	ptlDeInit(&recvP);
	#endif
}

void autoCalibrationTask(void)
{
	if(autoc.state & AUTOC_STA_RUN //运行自动校准
	#if AUTOC_EXTI_TEMP_COMMON == 1//如果启用外部NTC来获取真实温度 
		&& !(autoc.state & AUTOC_STA_COM_TIMEOUT)
	#else//使用内部NTC检测温度
		&& !(autoc.state & AUTOC_STA_NTC_HITCH)
	#endif
	)
	{
		//如果刚进入自动校准
		if(autoc.CalibrationNum == 0)
		{
			autoc.state &= ~AUTOC_STA_TEMP_CONT;//清除恒温状态
			autoc.conTempTime = AUTOC_TEMP_CONT_TIME;//刷新恒温稳定时间
			autoc.CalibrationNum = 1;//清除校准次数
			autoc.time = 0;//开始计算校准时间
			#if AUTOC_EXTI_TEMP_COMMON == 0
			autoc.ntcHitchTime = AUTOC_NTC_HITCH_TIME;//刷新内部NTC故障检查时间
			#endif
			debug("\r[auto calibration] *************************************************************************\r\n");
			debug("\r[auto calibration] *************************Start auto calibration**************************\r\n");
		}
		//恒温检测
		if(fabs(autoc.combTemp - autoc.oldTemp) <= AUTOC_TEMO_CONT_RANGE)//如果温度稳定
		{
			if(autoc.conTempTime == 0 && !(autoc.state & AUTOC_STA_TEMP_CONT))//如果内部温度进入恒定状态
			{
				autoc.state |= AUTOC_STA_TEMP_CONT;//进入恒温状态
				autoc.nextCalibrationTime = AUTOC_STABILISE_TIME;//刷新下一次校准时间
				debug("\r[auto calibration] Enter a constant temperature state.\r\n");
			}
		}
		else
		{
			autoc.oldTemp = autoc.combTemp;
			autoc.conTempTime = AUTOC_TEMP_CONT_TIME;
		}
		//调整时间到 开始计算校准温度差
		if(autoc.state & AUTOC_STA_TEMP_CONT && autoc.nextCalibrationTime == 0)//如果温度恒定状态且校准时间到
		{
			if(!(autoc.state & AUTOC_STA_CALIBRATION_END))//如果校准未结束
			{
				mchAdjustValueSet((float)mchAdjustValueGet() + ((float)FUN_ADJUST_TEMP - autoc.combTemp));//调整MCH偏差值
				autoc.state &= ~AUTOC_STA_TEMP_CONT;//清除恒温状态
				autoc.conTempTime = AUTOC_TEMP_CONT_TIME;//刷新恒温稳定时间
				autoc.CalibrationNum ++;//进入下一次校准
				debug("\r[auto calibration] To adjust the thermostat ==> combTemp: %0.2f adjustValue: %0.2f\r\n"
								,autoc.combTemp
								,(FUN_ADJUST_TEMP - autoc.combTemp));
				debug("\r[auto calibration] adjTemp: %0.2f Calibration number:%d\r\n"
								,mchAdjustValueGet()
								,(u16)(autoc.CalibrationNum-1));
				if(fabs((float)FUN_ADJUST_TEMP - autoc.combTemp) <= AUTOC_CALIBRATION_OFFSET)//温度在校准允许偏差范围内(校准完成)
				{
					if(!(autoc.state & AUTOC_STA_CALIBRATION_DONE))//如果第一次校准完成
					{
						autoc.state |= AUTOC_STA_CALIBRATION_DONE;//标记校准完成状态
						debug("\r[auto calibration]  _____________________________________\r\n");
						debug("\r[auto calibration] |   Auto calibration done!!!          |\r\n");
						debug("\r[auto calibration] |   Time consuming =======>>>%5usec |\r\n",autoc.time);//打印校准消耗时间
						debug("\r[auto calibration] |_____________________________________|\r\n");
						#if AUTOC_TRY_ALTIUS_ACCURACY == 0//如果不尝试更高精度的校准
						autoc.state |= AUTOC_STA_CALIBRATION_END;//标记校准结束
						#endif
					}
					if(autoc.state & AUTOC_STA_CALIBRATION_DONE)//如果校准完成
					{
						HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_ADJ_TEMP_ADDR,(u8 *)&mch.adjTemp,sizeof(mch.adjTemp));//写入调节温度
						debug("\r[auto calibration] Data has been saved to flash.\r\n");
					}
				}
				#if AUTOC_TRY_ALTIUS_ACCURACY == 1//如果尝试更高精准的校准
				if(fabs((float)FUN_ADJUST_TEMP - autoc.combTemp) <= AUTOC_TRY_CALIBRATION_OFFSET)//如果达到最高精度(结束校准)
				{
					if(!(autoc.state & AUTOC_STA_CALIBRATION_END))
					{
						autoc.state |= AUTOC_STA_CALIBRATION_END;//标记校准结束
						autoc.state |= AUTOC_STA_TEMP_CONT;//进入恒温状态
						debug("\r[auto calibration] Time consuming =======>>>%5usec\r\n",autoc.time);//打印校准消耗时间
						debug("\r[auto calibration] ************************End automatic calibration************************\r\n");
						debug("\r[auto calibration] *************************************************************************\r\n");
					}
				}
				#endif
			}
		}
		//通讯检测
		#if AUTOC_EXTI_TEMP_COMMON == 1//如果启用外部NTC来获取真实温度 
		if(!(autoc.state & AUTOC_STA_COM_TIMEOUT) && autoc.commonTimeout == 0)//如果超时
		{
			autoc.state |= AUTOC_STA_COM_TIMEOUT;//进入通讯超时状态
			debug("\r[auto calibration] communication timeout.\r\n");
		}
		#endif
	}
}

//实际温度采集管理
void autoCTempCollManage(void)
{
	#if AUTOC_EXTI_TEMP_COMMON == 1//如果启用外部NTC来获取真实温度
	if(ptlRead(&recvP) == 1)//如果接收到数据
	{
		if(!(autoc.state & AUTOC_STA_RUN))//如果进入调节模式
		{
			autoc.state |= AUTOC_STA_RUN;//运行自动校准
			if(!(fun.state & FUN_STA_ADJUST_MODE))//如果未进入校准模式
			{
				fun.state |= (FUN_STA_POWER|FUN_STA_ADJUST_MODE);//进入调节模式
				tcl.setTemp = FUN_ADJUST_TEMP;//设定校准温度
				mch.adjTemp = MCH_DEFAULT_ADJ_TEMP_OFFSET;//设置默认
			}
		}
		autoc.commonTimeout = AUTOC_COMMON_TIMEOUT;//刷新超时时间
		if(recvP.cmd == AUTOC_COM_TEMP_DATA && recvP.len == sizeof(autoc.combTemp))//如果是温度数据
		{
			memcpy(&autoc.combTemp,recvP.dat,sizeof(autoc.combTemp));//复制温度数据
			#if AUTOC_DEBUG_TEMP == 1
			if(!(autoc.state & AUTOC_STA_CALIBRATION_DONE))//如果未完成校准
				debug("\r[auto calibration] comb temp: %0.2f\r\n",autoc.combTemp);
			#endif
		}
		else
		{
			debug("\r[auto calibration] communication recvive data err.\r\n");
			debug("\r[auto calibration] p.cmd: %d p.len: %d\r\n",(u16)recvP.cmd,(u16)recvP.len);
		}
		ptlDeInit(&recvP);
	}
	#else//使用内部NTC获取真实温度
	if(!(autoc.adCombTemp & 0x8000) && autoc.state & AUTOC_STA_RUN)
	{
		autoc.combTempNum += autoc.adCombTemp;//累加
		autoc.combTempCnt++;
		if(autoc.combTempCnt >= AUROC_NTC_COLL_NUM)
		{
			float ioVolt;
			float cur;
			autoc.combTempNum /= AUROC_NTC_COLL_NUM;
			ioVolt = 5.0 * (float)autoc.combTempNum / 4096.0;//NTC电压
			cur = (5.0 - ioVolt) / AUTOC_NTC_COLL_RES;//计算电流
			autoc.combTemp = ntcTempCompute(ioVolt/cur) + AUTOC_NTC_OFFSET;//计算温度
			autoc.combTempNum = 0;
			autoc.combTempCnt = 0;
			if(!(autoc.state & AUTOC_STA_CALIBRATION_END))
				debug("\r[auto calibration] ntcTemp: %0.2f mchTemp: %0.2f ",autoc.combTemp,tcl.curTemp);
		}
		if(autoc.adCombTemp >= 4086 || autoc.adCombTemp <= 10)//如果NTC故障
		{
			if(autoc.ntcHitchTime == 0 && !(autoc.state & AUTOC_STA_NTC_HITCH))
			{
				autoc.state |= AUTOC_STA_NTC_HITCH;//标记NTC故障
				debug("\r[auto calibration] error: ntc hitch!!!\r\n");
			}
		}
		else
			autoc.ntcHitchTime = AUTOC_NTC_HITCH_TIME;
		autoc.adCombTemp |= 0x8000;
	}
	#endif
}

//在MCH的情况下需要在交流电制高点采集数据，AD采集不可在主循环操作。
#if AUTOC_EXTI_TEMP_COMMON == 0//内部采集，不使用外部采集通讯。
void autoAcPeakValueHook (void)
{
	autoc.adCombTemp = HalAdcRead(AUTOC_ADCH_NTC);
}
#endif

void autoCalibrationTimeCallBack(void)
{
	static u16 sec = 0;

	sec++;
	if(sec >= 1000)
	{
		sec = 0;
		autoc.time ++;
	}
	
	if(autoc.nextCalibrationTime)
		autoc.nextCalibrationTime--;
	if(autoc.commonTimeout)
		autoc.commonTimeout--;
	if(autoc.conTempTime)
		autoc.conTempTime--;
	#if AUTOC_EXTI_TEMP_COMMON == 0
	if(autoc.ntcHitchTime)
		autoc.ntcHitchTime--;
	#endif
}

#endif
