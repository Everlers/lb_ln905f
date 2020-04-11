#ifndef _PID_H_
#define _PID_H_
#include "hal_sys.h"

#define PID_MAX_OUT_VALUE										150				//最大输出值
#define PID_MIN_OUT_VALUE										0					//最小输出值
#define PID_DEFAULT_KP											0.1 			//默认比例值
#define PID_DEFAULT_KI											0.001			//默认积分值
#define PID_DEFAULT_KD											0.0				//默认微分值

typedef struct
{
	float out;//输出值
	float err;//定义偏差值
	float err_last;//定义上一个偏差值
	float err_prev;//定义前一个的偏差值
	float kp, ki, kd;//定义比例、积分、微分系数
}pid_t;

void pidInit(void);
float pidGetOutValue(void);
void pidSetOutValue(float value);
void pidConfig (float kp,float ki,float kd);
float pidCompute(float expectedValue,float actualValue);
#endif
