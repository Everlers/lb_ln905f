#ifndef _SILICONCONTROL_H_
#define _SILICONCONTROL_H_
#include "hal_sys.h"

#define SCHEAT											P2_3		//加热IO
#define ZERO_IO											P0_4		//零点IO

#define SC_CTRL_NUM									1				//控制数量
#define SC_PORT_HEAT								0				//加热控制
#define SC_FREQ_NUM									10			//频率检测次数
#define SC_VOLT_CHANNEL							12			//电压检测通道
#define SC_VOLT_NUM									10			//电压检测次数
#define SC_UP_RES										2000.0L	//上拉电阻值
#define SC_DOWN_RES									20.0L		//下拉电阻值

#define SC_AC_PEAK_VALUE_HOOK()			tclAcPeakValueHook()	//AC峰值回调
#define SC_AC_FREQ_HOOK()						autoAcPeakValueHook()	//AC频率回调 (在MCH的情况下需要由可控硅驱动来控制AD)

#define SC_TRIGG_PLACE_P						-500																				//正旋波触发位置 零点导通角度 超前:负数        			  	  滞后:正数
#define SC_TRIGG_PLACE_R						-500																				//负旋波触发位置 零点导通角度 超前:负数        			  	  滞后:正数
#define SC_TRIGG_TIME								2000												                //触发脉冲宽度时间
#define SC_DEFAULT_PERIOD						10												                  //可控硅默认周期

#define SC_STA_ALL_SAVE							0x01//全波控制

typedef enum{
	SC_AC_FREQ_NOT = 0,
	SC_AC_FREQ_60HZ = 1,
	SC_AC_FREQ_50HZ = 2,
}powerFrequency;

typedef enum{
	SC_AC_VOLT_NOT = 0,
	SC_AC_VOLT_110V	= 1,
	SC_AC_VOLT_220V = 2,
}powerVoltage;

typedef struct{
	u8 	state;							//状态
	u16 zeroTime;						//零点时间
  u16 count; 							//计数
  u16	period;							//周期设定
  u16 duty[SC_CTRL_NUM];	//占空比设定

	powerVoltage	 wv;			//电源电压
	powerFrequency wf;			//电源频率
}siliconControl_t;

void siliconControlInit (void);
void scWaveConfig(bit sw);
u16 scPeriodGet (void);
void scPeriodSet (u16 period);
u16 scDutyGet (u8 port);
void scDutySet (u8 port,u16 duty);
void scPowerCheck(void);
powerVoltage scGetVolt(void);
powerFrequency scGetFreq(void);
#endif