#ifndef _TEMPCTRL_H_
#define _TEMPCTRL_H_
#include "hal_sys.h"

#define TCL_DEBUG_TCL_DATA										0			//打印温控数据

#define TCL_STA_RUSH_TEMP											0x0001//冲温状态
#define TCL_STA_CONT_TEMP											0x0002//恒温状态
#define TCL_STA_COLLECT_ERR										0x0004//采样错误 (MCH/NTC采集端口有误)

//温度采集选择
#define TCL_TEMP_NTC													1
#define TCL_TEMP_MCH													2
#define TCL_TEMP_COLL_SELECT									TCL_TEMP_MCH//选择采集源

#define TCL_TEMP_COMPUTE_TIME									150		//温度计算时间
#define TCL_RUSH_TEMP_PERIOD_110V							4			//冲温周期
#define TCL_RUSH_TEMP_PERIOD_220V							6			//冲温周期
#define TCL_RUSH_TEMP_DUTY_220V								2			//冲温占空比220V
#define TCL_RUSH_TEMP_DUTY_110V								4			//冲温占空比110V
#define TCL_RUSH_TEMP_STOP_OFFSET							-5		//冲温停止差值（设定温度的差值，用于进入恒温状态）
#define TCL_CONT_TEMP_MAX_PERIOD							150		//最大恒温周期
#define TCL_CONT_TEMP_MIN_PERIOD_220V					9			//最小恒温周期 110V
#define TCL_CONT_TEMP_MIN_PERIOD_110V					5			//最小恒温周期 110V
#define TCL_CONT_TEMP_MIN_DUTY_220V						2			//恒温最小占空比（不得全关，需要采集发热体阻值）220V
#define TCL_CONT_TEMP_MIN_DUTY_110V						4			//恒温最小占空比（不得全关，需要采集发热体阻值）110V
#define TCL_CONT_TEMP_SPEED										900		//恒温速度 执行时间 单位：毫秒
#define TCL_COLLECT_RES_VALUE									0.1		//采样电阻值 单位：欧姆 (用于MCH发热体的采样电阻)
#define TCL_COLLECT_NTC_RES_VALUE							4700.0//分压电阻值	单位：欧姆 (用于PTC发热体的NTC分压电阻)
#define TCL_COLLECT_HITCH_TIME								1000	//温度采集故障的时间

#define TCL_MCUVOLT_CHANNEL										ADC15							//MCU电压采集通道
#define TCL_ACVOLT_CHANNEL										SC_VOLT_CHANNEL		//AC电压采集通道
#define TCL_RESVOLT_CHANNEL										ADC8							//采样电阻采集通道

typedef struct{
	u16 		state;								//状态
	u16			setTemp;							//设定温度
	float 	curTemp;							//当前温度
	
	u16 		adMcuVolt;						//MCU电压AD值
	double 	mcuVolt;							//MCU电压
	#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
	u16 		adAcVolt;							//交流电压AD值
	u16 		adResVolt;						//采样电阻电压AD值
	double 	acVolt;								//AC电压
	double 	resVolt;							//采样电阻电压
	double 	heatCur;							//发热丝电流
	double 	heatRes;							//发热丝阻值
	#else
	u16 		adNtcVolt;						//NTC电压AD值
	double	ntcRes;								//热敏电阻值
	#endif

	u16 		tempComputeTime;			//温度计算时间计数
	u16 		contTempSpeed;				//恒温速度计数
	u16 		tempColHitchTime;			//温度采集故障时间计数
}tempCtrl_t;
extern tempCtrl_t xdata tcl;

void tclInit(void);
void tclCollectManage (void);
void tclTempManage (void);
void tclTimeHook(void);
void tclAcPeakValueHook (void);
#endif