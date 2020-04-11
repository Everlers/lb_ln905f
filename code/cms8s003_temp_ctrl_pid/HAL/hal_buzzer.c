#include "hal_buzzer.h"
#include "string.h"
#include "hal_time.h"
#include "hal_pwm.h"

Buzzer_t xdata Buzzer;
u8 xdata NumberCount = RESET;
u16 xdata dutyCycleCount = RESET;

void HalBuzzerInit(void)
{
	HalPwmInit();
	BUZZER_CLR;
	memset(&Buzzer,0,sizeof(Buzzer));
	HalBuzzerSwitch(ENABLE);
}

void HalBuzzerSwitch(FunctionalState State)
{
	Buzzer.BuzzerSw = State;
}

FunctionalState HalBuzzerSwitchState(void)
{
	return Buzzer.BuzzerSw;
}

//Set buzzer open time
void HalBuzzSetPeriod(u16 Period)
{
	if(Buzzer.BuzzerSw == ENABLE)
	if(Buzzer.State == DISABLE)
	{
		Buzzer.Number = 1;
		Buzzer.Period = Period;
		Buzzer.DutyCycle = Period;
		Buzzer.State = ENABLE;
	}
}
void HalBuzzerStop(void)
{
	memset((u8 *)&Buzzer,RESET,sizeof(Buzzer_t));
	BUZZER_CLR;
}
//set buzzer open time and duty cycle (Number=0xFF Without stop)
void HalBuzzerSet(u8 Number,u16 Period,u16 dutyCucle)
{
	if(Buzzer.BuzzerSw == ENABLE)
	if(Buzzer.State == DISABLE)
	{
		Buzzer.Number = Number;
		Buzzer.Period = Period;
		Buzzer.DutyCycle = dutyCucle;
		Buzzer.State = ENABLE;
	}
}
//Get buzzer state
FunctionalState HalGetBuzzerState(void)
{
	return Buzzer.State;
}

//Buzzer task,1 milliseconds enter once
void HalBuzzerTask(void)
{
	if(Buzzer.State == ENABLE)
	{
		dutyCycleCount++;
		if(dutyCycleCount >= Buzzer.DutyCycle)
			BUZZER_CLR;
		if(dutyCycleCount < Buzzer.DutyCycle)
			BUZZER_SET;
		if(dutyCycleCount >= Buzzer.Period)
		{
			NumberCount ++;
			dutyCycleCount = RESET;
		}
		if(NumberCount >= Buzzer.Number )//&& Buzzer.Number != 0xFF)
		{
			NumberCount = RESET;
			BUZZER_CLR;
			Buzzer.State = DISABLE;
		}
	}
}

