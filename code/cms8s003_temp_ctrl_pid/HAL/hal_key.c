#include "hal_key.h"

static u8 xdata key = 0;
static u8 xdata keyLongPress;
static u16 xdata longPressTime = 0;

static u16 xdata pollKey = HAL_KEY_SW_ALL;
static u16 xdata waitKey = 0;
static u8 xdata shakeTime;

static void shortPressPoll(u16 port)
{
	if((waitKey & HAL_KEY_SW_1) && (port & HAL_KEY_SW_1)){
		key |= HAL_KEY_POWER;
	}
	if((waitKey & HAL_KEY_SW_2) && (port & HAL_KEY_SW_2)){
		key |= HAL_KEY_LOCK;
	}
	if((waitKey & HAL_KEY_SW_3) && (port & HAL_KEY_SW_3)){
		key |= HAL_KEY_SET;
	}
}

static void longPressPoll(void)
{
	if(waitKey & HAL_KEY_SW_1){
		keyLongPress |= HAL_KEY_POWER;
	}
	else
		keyLongPress &= ~HAL_KEY_POWER;

	if(waitKey & HAL_KEY_SW_2){
		keyLongPress |= HAL_KEY_LOCK;
	}
	else
		keyLongPress &= ~HAL_KEY_LOCK;

	if(waitKey & HAL_KEY_SW_3){
		keyLongPress |= HAL_KEY_SET;
	}
	else
		keyLongPress &= ~HAL_KEY_SET;
}

void HalKeyTask(void)
{
	if(shakeTime == 0 && pollKey != HAL_KEY_SW_ALL)//如果有按下并且抖动时间过
	{
		if(pollKey == (HAL_KEY_PORT&HAL_KEY_SW_ALL))//按下状态确认
		{
			waitKey = ~pollKey;
			waitKey &= HAL_KEY_SW_ALL;
			pollKey = HAL_KEY_SW_ALL;
		}
		else{
			pollKey = HAL_KEY_SW_ALL;
		}
	}
	
	if((HAL_KEY_PORT & HAL_KEY_SW_ALL) !=  HAL_KEY_SW_ALL && shakeTime == 0 && waitKey == 0)//如果有按钮按下
	{
		pollKey = (HAL_KEY_PORT & HAL_KEY_SW_ALL);//得到按钮状态
		shakeTime = HAL_KEY_SHAKE_TIME;//设置祛抖时间
	}
	
	if(waitKey)//如果有按键按下
	{
		u16 port = HAL_KEY_PORT & HAL_KEY_SW_ALL;
		if(longPressTime < HAL_KEY_LONG_PRESS_TIME){//如果不到长按时间
			longPressTime++;
			shortPressPoll(port);//执行短按扫描
		}
		else if(longPressTime == HAL_KEY_LONG_PRESS_TIME)//长按时间到
		{
			longPressTime++;
			longPressPoll();//执行长按扫描
		}
		waitKey ^= (port&waitKey);//清除已经松开的按钮
	}
	else{//所有按钮已经松开
		keyLongPress = 0;
		longPressTime =  0;
	}
	
	if(shakeTime)
		shakeTime--;
}

//查询是否松开
bit HalKeyGetLoosen(u8 sw)
{
	return (HAL_KEY_PORT & sw) ? 1:0;
}

bit HalKeyGetShortPress(u8 sw)
{
	bit s = (sw&key) ? 1:0;
	key ^= sw&key;
	return s;
}

bit HalKeyGetLongPress(u8 sw)
{
	return(sw&keyLongPress) ? 1:0;
}

u8 HalKeyGetShortPressNoClear(u8 sw)
{
	return sw & key;
}

u8 HalKeyGetLongPressNoClear(u8 sw)
{
	return sw & keyLongPress;
}




