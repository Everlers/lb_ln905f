#include "mch.h"
#include "hal_flash.h"
#include "hal_uart.h"
#include "siliconControl.h"
#include "delay.h"

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH

mch_t mch;

code u16 tempList[MCH_TEMMP_PIONT_NUM] = 	{23 		,100 		,120 		,140 		,160 		,180 		,200 		,220 	 ,240		};//温度点
////LaBo-7030-37.3Ω-110V
//code float resList[MCH_TEMMP_PIONT_NUM] = {37.30 	,48.35 	,51.50 	,54.65 	,57.80 	,61.00  ,64.20 ,67.40  ,70.60	};//电阻点
//LaBo-7020-2-30/35.9Ω-110V
code float resList[MCH_TEMMP_PIONT_NUM] = {31.40 	,40.60 	,43.10 	,45.70 	,48.20 	,50.80  ,53.40 ,55.90  ,58.50	};//电阻点
//QuZhi-65Ω-110V
//code float resList[MCH_TEMMP_PIONT_NUM] = {65.10 	,84.40 	,89.50 	,94.60 	,99.80 	,105.40 ,111.00 ,116.30 ,121.70	};//电阻点

void mchInit(void)
{
	if(HalFlashDefault() != HAL_FLASH_DONE)//如果是出厂状态
	{
		mch.adjTemp = MCH_DEFAULT_ADJ_TEMP_OFFSET;
		HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_ADJ_TEMP_ADDR,(u8 *)&mch.adjTemp,sizeof(mch.adjTemp));//写入调节温度
	}
	HalFlashRead(HAL_FLASH_DATA,HAL_FLASH_ADJ_TEMP_ADDR,(u8 *)&mch.adjTemp,sizeof(mch.adjTemp));//读取校准温度
	debug("[MCH] temp adjust value: %0.2f\r\n",mch.adjTemp);
}

//参数：电阻值 单位：欧姆
//返回值：温度 单位：摄氏度
float mchTempGet(float res)
{
	double temp;
	u8 i;
	if((res < resList[0]))//如果低于最小温度点
		return tempList[0];//最低温度
	for(i=0;i<MCH_TEMMP_PIONT_NUM-1;i++)
		if(res >= resList[i] && res <= resList[i+1])
			break;
	if(i >= MCH_TEMMP_PIONT_NUM-1)
		i = MCH_TEMMP_PIONT_NUM-2;
	temp = tempList[i+1] - tempList[i];//温度计算宽度
	temp /= resList[i+1] - resList[i];//计算系数
	temp *= res - resList[i];//根据当前阻值计算系数内的温度以及校准值
	temp += (float)tempList[i];//计算当前温度
	temp = MAX_VALUE(temp-mch.adjTemp,0);
	return temp;
}

//设定调整值
void mchAdjustValueSet(float value)
{
	mch.adjTemp = value;
}

//查询调整值
float mchAdjustValueGet(void)
{
	return mch.adjTemp;
}

//查询最高温度
u16 mchGetMaxTemp(void)
{
	return tempList[MCH_TEMMP_PIONT_NUM-1];
}

#endif
