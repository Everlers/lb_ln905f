#include "pid.h"

pid_t xdata pid;

void pidInit(void)
{
	pid.out = 0.0;
	pid.err = 0.0;
	pid.err_prev = 0.0;
	pid.err_last = 0.0;
	pid.kp = PID_DEFAULT_KP;
	pid.ki = PID_DEFAULT_KI;
	pid.kd = PID_DEFAULT_KD;
}

//描述:手动设置输出值
void pidSetOutValue(float value)
{
	pid.out = value;
}

//描述:查询输出值
float pidGetOutValue(void)
{
	return pid.out;
}

//描述:PID配置
//kp:比例
//ki:积分
//kd:微分
void pidConfig (float kp,float ki,float kd)
{
	pid.kp = kp;
	pid.ki = ki;
	pid.kd = kd;
}

//描述:PID计算
//expectedValue:理想值
//actualValue:实际值
float pidCompute(float expectedValue,float actualValue)
{
	float incrementValue;
	pid.err = expectedValue - actualValue;//计算差值
	incrementValue = pid.kp*(pid.err - pid.err_last) + pid.ki*pid.err + pid.kd*(pid.err - 2 * pid.err_last + pid.err_prev);//增量式pid公式
	pid.out += incrementValue;//设定输出
	pid.err_prev = pid.err_last;//保存上次偏差
	pid.err_last = pid.err;//保存本次偏差
	pid.out = MIN_VALUE(PID_MAX_OUT_VALUE,pid.out);//设定最大值
	pid.out = MAX_VALUE(PID_MIN_OUT_VALUE,pid.out);//设定最小值
	return pid.out;
}
