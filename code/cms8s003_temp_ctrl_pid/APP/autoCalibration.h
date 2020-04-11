#ifndef _AUTOCALIBRATION_H_
#define _AUTOCALIBRATION_H_
#include "hal_sys.h"
#include "fun.h"
#include "tempCtrl.h"

#define AUTOC_ENABLE													1			//1:使能自动校准功能
#define AUTOC_EXTI_TEMP_COMMON								0			//1:启用通讯取得外部温度 0:使用内部NTC获取温度

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_ENABLE == 1

#define AUTOC_TRY_ALTIUS_ACCURACY							1			//尝试更高精度的校准
#define AUTOC_DEBUG_TEMP											1			//打印实际的温度数据
#define AUTOC_ADCH_NTC												10		//内部采集的状态下NTC采集通道
#define AUTOC_NTC_COLL_RES										4700.0//内部采样的状态下NTC的采样电阻
#define AUTOC_NTC_OFFSET											0			//内部NTC的温度偏差 正补偿
#define AUROC_NTC_COLL_NUM										100		//采集滤波10ms*100=1S完成一次采集

//状态
#define AUTOC_STA_RUN													0x01	//自动校准开启
#define AUTOC_STA_COM_TIMEOUT									0x02	//通讯超时状态
#define AUTOC_STA_TEMP_CONT										0x04	//温度恒定状态
#define AUTOC_STA_CALIBRATION_DONE						0x08	//校准完成
#define AUTOC_STA_CALIBRATION_END							0x10	//校准结束
#define AUTOC_STA_NTC_HITCH										0x20	//内部NTC故障

//通讯指令
#define AUTOC_COM_TEMP_DATA										1			//温度数据

//参数
#define AUTOC_COMMON_TIMEOUT									3500	//通讯超时时间
#define AUTOC_NTC_HITCH_TIME									3000	//NTC故障排查时间
#define AUTOC_TEMO_CONT_RANGE									3			//温度恒定范围
#define AUTOC_TEMP_CONT_TIME									50000	//等待温度恒定时间
#define AUTOC_STABILISE_TIME									60000	//等待温度稳定时间
#define AUTOC_CALIBRATION_OFFSET							3.0		//校准允许的正负偏差(不需设定太高精度，可开启持续校准)
#define AUTOC_TRY_CALIBRATION_OFFSET					1.0		//持续更高精度校准偏差(在此偏差内程序结束校准)

typedef struct{
	u8 		state;									//状态
	float	combTemp;								//梳子温度

	#if AUTOC_EXTI_TEMP_COMMON == 0
	float adMcuVolt;							//MCU电压(AD值)
	u16		adCombTemp;							//梳齿温度(AD值)
	u32		combTempNum;						//梳齿温度累加值
	u16		combTempCnt;						//梳齿温度累加计数
	u16		ntcHitchTime;						//内部NTC故障时间
	#endif

	float	oldTemp;								//旧的梳子温度
	u8 		CalibrationNum;					//校准次数
	
	u32		nextCalibrationTime;		//下次校准时间
	u16		commonTimeout;					//通讯超时计数
	u16		conTempTime;						//温度稳定时间
	u16		time;										//校准使用时间
}autoCalibration_t;
extern autoCalibration_t xdata autoc;

void autoCalibrationInit(void);
void autoCalibrationTask(void);
void autoCTempCollManage(void);
void autoCalibrationTimeCallBack(void);
void autoAcPeakValueHook (void);
#endif

#endif