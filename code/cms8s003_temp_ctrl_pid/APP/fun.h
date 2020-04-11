#ifndef _FUN_H_
#define _FUN_H_
#include "hal_sys.h"

#define LED5																	P0_0
#define LED4																	P1_3
#define LED3																	P1_4
#define LED2																	P1_5
#define LED1																	P1_6

#define FUN_TEST_TEMP_GEAR										0			//测试各挡位温度
#define FUN_TEST_GEAR_TIME										360		//各挡位停留时间

#define FUN_MAX_TEMP_GEAR											4			//最大温度挡位
#define FUN_DEFAULT_TEMP_GEAR									0			//默认温度挡位
#define FUN_ADJUST_TEMP												200		//设定校准温度
#define FUN_MIN_SHOW_TEMP											30		//最小显示温度
#define FUN_TEMP_SET_TIME											3000	//温度设定的时间

#define FUN_LOW_POWER_TIME										600		//低功耗时间	10分钟
#define FUN_AUTO_CLOSE_TIME										3600	//自动关机时间 60分钟

//state
#define FUN_STA_POWER													0x0001//电源开关
#define FUN_STA_SET_TEMP											0x0002//设定温度
#define FUN_STA_LOCK_UP												0x0010//上升温度
#define FUN_STA_LOCK_DOWN											0x0020//下降温度
#define FUN_STA_ADJUST_MODE										0x0040//校准模式
#define FUN_STA_LED_FLAG											0x0080//LED闪烁标志
#define FUN_STA_LOCK_SET											0x0100//锁定设置

//unit
#define FUN_UNIT_DC														0//摄氏度
#define FUN_UNIT_FC														1//华氏度

typedef struct{
	u16 		state;								//状态
	u8			tempGear;							//温度挡位
	u8 			tempUnit;							//温度单位
	u8			gearBak;							//挡位备份
	
	u16			ulTime;								//滚动开关时间计数
	u16 		lowPowerTime;					//低功耗时间计数
	u16 		tempSetTime;					//温度设定时间计数
	u16 		showTempLockTime;			//显示温度的锁定时间计数
	u16 		autoCloseTime;				//自动关机时间计数
	
	u8 			ledStep;							//LED显示挡位
	u16			ledBlankTime;					//LED闪烁时间

#if FUN_TEST_TEMP_GEAR == 1			//如果需要测试各挡位温度
	u16			testGearTime;					//挡位测试时间
#endif
}fun_t;

extern fun_t xdata fun;
extern code u16 setTempGearList[FUN_MAX_TEMP_GEAR];

void funInit (void);
void funTimeCallBack (void);
void funCtrlManage (void);
void funShowManage (void);
void funShowUnlock (void);
void funReadFlash (void);
void funSaveFlash (void);
#endif