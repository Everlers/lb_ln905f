#include "tempCtrl.h"
#include "hal_adc.h"
#include "hal_uart.h"
#include "fun.h"
#include "pid.h"
#include "mch.h"
#include "ntc.h"
#include "math.h"
#include "siliconControl.h"
#include "autoCalibration.h"

tempCtrl_t xdata tcl;

void tclInit(void)
{
	tcl.state = 0;
	tcl.tempComputeTime = 0;
	tcl.contTempSpeed = 0;
	tcl.tempColHitchTime = TCL_COLLECT_HITCH_TIME*2;
	tcl.curTemp = 0;
	tcl.adMcuVolt = 0;
	tcl.mcuVolt = 0;
	#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
	tcl.adAcVolt = 0;
	tcl.adResVolt = 0;
	tcl.acVolt = 0;
	tcl.resVolt = 0;
	tcl.heatCur = 0;
	tcl.heatRes = 0;
	#else
	tcl.adNtcVolt = 0;
	tcl.ntcRes = 0;
	#endif
	tcl.setTemp = setTempGearList[fun.tempGear];
	//配置PID
	//参数 比例 积分 微分
	#if TCL_TEMP_COLL_SELECT == TCL_TEMP_NTC
	pidConfig(15 ,1 ,1);//NTC采集PID配置
	#else
	pidConfig(1 ,0.5 ,0.1);//MCH采集PID配置
	#endif
}

//温度控制任务
void tclTempManage (void)
{
	if(fun.state & FUN_STA_POWER)//如果开启电源
	{
		if(!(tcl.state & TCL_STA_RUSH_TEMP) && !(tcl.state & TCL_STA_CONT_TEMP))//如果刚打开电源
		{
			scWaveConfig(1);//全波控制
			tcl.state |= TCL_STA_RUSH_TEMP;//进入冲温状态
		}
		
		if(tcl.state & TCL_STA_RUSH_TEMP)//冲温状态
		{
			if(scGetVolt() == SC_AC_VOLT_220V)
			{
				scDutySet(SC_PORT_HEAT,TCL_RUSH_TEMP_DUTY_220V);//设定冲温占空比
				scPeriodSet(TCL_RUSH_TEMP_PERIOD_220V);//设定冲温周期
			}
			else
			{
				scDutySet(SC_PORT_HEAT,TCL_RUSH_TEMP_DUTY_110V);//设定冲温占空比
				scPeriodSet(TCL_RUSH_TEMP_PERIOD_110V);//设定冲温周期
			}
			if(tcl.curTemp >= (tcl.setTemp + TCL_RUSH_TEMP_STOP_OFFSET)){//如果即将到达设定温度
				tcl.state &= ~TCL_STA_RUSH_TEMP;//退出冲温
				tcl.state |= TCL_STA_CONT_TEMP;//进入恒温状态
				pidSetOutValue(PID_MAX_OUT_VALUE/2);//设定恒温初始值
			}
		}
		else if(tcl.state & TCL_STA_CONT_TEMP && tcl.contTempSpeed == 0)//恒温状态
		{
			u16 period = TCL_CONT_TEMP_MAX_PERIOD - pidCompute(tcl.setTemp,tcl.curTemp);//计算周期
			period = MIN_VALUE(period,TCL_CONT_TEMP_MAX_PERIOD);//限制最大周期
			period = MAX_VALUE(period,(scGetVolt() == SC_AC_VOLT_220V) ? 
			TCL_CONT_TEMP_MIN_PERIOD_220V : TCL_CONT_TEMP_MIN_PERIOD_110V);//限制最小周期
			#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH//如果是MCH设备
			{
				if(period >= TCL_CONT_TEMP_MAX_PERIOD)//如果需要关闭输出
					scPeriodSet(500);//设定500个采集周期(时间: 50Hz=5s 60Hz=4.16s)
				else
					scPeriodSet(period);//设定恒温周期
			}
			#else//如果是NTC采集方式可直接设定周期  因占空比可直接关闭
				scPeriodSet(period);//设定PID计算的恒温周期
			#endif
			#if TCL_TEMP_COLL_SELECT == TCL_TEMP_NTC
			if(period < TCL_CONT_TEMP_MAX_PERIOD)//如果未到最大周期
			#endif
			{
				if(scGetVolt() == SC_AC_VOLT_220V)
					scDutySet(SC_PORT_HEAT,TCL_CONT_TEMP_MIN_DUTY_220V);//设定恒温占空比
				else
					scDutySet(SC_PORT_HEAT,TCL_CONT_TEMP_MIN_DUTY_110V);//设定恒温占空比
			}
			#if TCL_TEMP_COLL_SELECT == TCL_TEMP_NTC
			else//NTC采集的情况下到最大周期可直接关闭
				scDutySet(SC_PORT_HEAT,0);//设定恒温占空比
			#endif
			tcl.contTempSpeed = TCL_CONT_TEMP_SPEED;//刷新恒温速度
		}
	}
	else//电源关闭状态
	{
		scPeriodSet(TCL_RUSH_TEMP_PERIOD_110V);//设定周期
		scDutySet(SC_PORT_HEAT,0);//关闭占空比
		if(tcl.state & (TCL_STA_CONT_TEMP|TCL_STA_RUSH_TEMP))//如果刚关闭电源
			tcl.state &= ~(TCL_STA_CONT_TEMP|TCL_STA_RUSH_TEMP);//清除温度控制的状态
	}
}
#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
double tclComputeHeatRes(void)
{
	double ioVOlt,heatVolt;
	tcl.mcuVolt = 1.2L * 4096.0L / (double)tcl.adMcuVolt;//计算MCU电压
	ioVOlt = tcl.adAcVolt * tcl.mcuVolt / 4096.0L;//计算Ac电源的IO电压
	tcl.acVolt = (tcl.mcuVolt - ioVOlt) / SC_DOWN_RES * SC_UP_RES;//计算AC电压
	ioVOlt = tcl.adResVolt * tcl.mcuVolt / 4096.0L;//计算采样电阻的IO电压
	tcl.resVolt = tcl.mcuVolt - ioVOlt;//计算采样电阻电压
	tcl.heatCur = tcl.resVolt / TCL_COLLECT_RES_VALUE;//计算电流
	if(tcl.heatCur <= 1.0 && (tcl.state & TCL_STA_RUSH_TEMP || tcl.state & TCL_STA_CONT_TEMP))//如果MCH电流小于1A
	{
		if(tcl.tempColHitchTime == 0 && !(tcl.state & TCL_STA_COLLECT_ERR))
			tcl.state |= TCL_STA_COLLECT_ERR;
	}
	else
		tcl.tempColHitchTime = TCL_COLLECT_HITCH_TIME;
	heatVolt = tcl.acVolt - tcl.resVolt;//计算发热体电压
	return heatVolt / tcl.heatCur;//计算发热丝电阻值
}
#else
double tclComputeNtcRes(void)
{
	double ioVolt,cur;
	tcl.adMcuVolt = HalAdcRead(TCL_MCUVOLT_CHANNEL);//采集芯片电压
	tcl.adNtcVolt = HalAdcRead(TCL_RESVOLT_CHANNEL);//采集NTC电压
	if((tcl.adNtcVolt >= 4090 || tcl.adNtcVolt <= 10)  && //如果NTC故障
		(tcl.state & TCL_STA_RUSH_TEMP || tcl.state & TCL_STA_CONT_TEMP))//如果开启了恒温或者冲温
	{
		if(tcl.tempColHitchTime == 0 && !(tcl.state & TCL_STA_COLLECT_ERR))
			tcl.state |= TCL_STA_COLLECT_ERR;
	}
	else
		tcl.tempColHitchTime = TCL_COLLECT_HITCH_TIME;
	tcl.mcuVolt = 1.2L * 4096.0L / (double)tcl.adMcuVolt;//计算MCU电压
	ioVolt = tcl.adNtcVolt * tcl.mcuVolt / 4096.0L;//计算NTC电压
	cur = (tcl.mcuVolt - ioVolt) / TCL_COLLECT_NTC_RES_VALUE;//计算电流
	return ioVolt / cur;//计算阻值
}
#endif
//采集管理
void tclCollectManage (void)
{
	u16 curTemp;
	if(fun.state & FUN_STA_POWER)//如果打开电源
	{
		if(tcl.tempComputeTime == 0)//在较高电压 且 开启的情况下检测
		{
			#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
			tcl.heatRes = tclComputeHeatRes();//计算发热丝阻值
			if(tcl.heatCur >= 1)//发热体电流必须大于1A
				curTemp = mchTempGet(tcl.heatRes);//计算当前温度
			else
				curTemp = tcl.curTemp;
			#else
			tcl.ntcRes = tclComputeNtcRes();//计算NTC阻值
			curTemp = ntcTempCompute(tcl.ntcRes);//计算当前温度
			#endif
			if(abs(tcl.curTemp - curTemp) >= 50)
				tcl.curTemp = curTemp;
			if(tcl.curTemp > curTemp)
				tcl.curTemp -= 1.0;
			if(tcl.curTemp < curTemp)
				tcl.curTemp += 1.0;
			#if TCL_DEBUG_TCL_DATA == 1
			#if AUTOC_ENABLE == 1
			if(!(autoc.state & AUTOC_STA_RUN))
			#endif
			{
				debug("\r\n");
				debug("mcuVolt:%4.2f ",tcl.mcuVolt);
				#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
				debug("acVolt:%6.2f ",tcl.acVolt);
				debug("heatCur:%4.2f ",tcl.heatCur);
				debug("resVolt:%4.2f ",tcl.resVolt);
				debug("heatRes:%5.2f ",tcl.heatRes);
				#endif
				debug("outVlaue:%6.2f ",pidGetOutValue());
				debug("fun.curTemp:%6.2f(%3u)",tcl.curTemp,curTemp);
			}
			#endif
			tcl.tempComputeTime = TCL_TEMP_COMPUTE_TIME;
		}
	}
}

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
void tclAcPeakValueHook (void)
{
	tcl.adMcuVolt = HalAdcRead(TCL_MCUVOLT_CHANNEL);//读取MCU电压
	tcl.adResVolt = HalAdcRead(TCL_RESVOLT_CHANNEL);//读取采样电阻电压
	tcl.adAcVolt = HalAdcRead(TCL_ACVOLT_CHANNEL);//读取AC电压
}
#endif

void tclTimeHook(void)
{
	if(tcl.contTempSpeed)
		tcl.contTempSpeed --;
	if(tcl.tempComputeTime)
		tcl.tempComputeTime--;
	if(tcl.tempColHitchTime)
		tcl.tempColHitchTime--;
}
