#ifndef _HAL_ADC_H_
#define _HAL_ADC_H_
#include "hal_sys.h"

/*采集通道选择*/
#define  ADC0           0
#define  ADC1           1
#define  ADC2           2
#define  ADC3           3
#define  ADC4           4
#define  ADC5           5
#define  ADC6           6
#define  ADC7           7
#define  ADC8           8
#define  ADC9           9
#define  ADC10          10
#define  ADC11          11
#define  ADC12          12
#define  ADC13          13
#define  ADC14          14
#define  ADC15          15            

#define  ADC_START        ADCON0 |= ADGO;   //ADC序列转换启动

//#define  HalAdcStart()		ADC_START

//一个完成12位转换需要18.5个AD时钟
typedef enum{
	HAL_ADC_CLK_2 = 0,//F/2
	HAL_ADC_CLK_4 = 1,//F/4
	HAL_ADC_CLK_8 = 2,//F/8
	HAL_ADC_CLK_16 = 3,//F/16
	HAL_ADC_CLK_32 = 4,//F/32
	HAL_ADC_CLK_64 = 5,//F/64
	HAL_ADC_CLK_128 = 6,//F/128
	HAL_ADC_CLK_256 = 7,//F/256
}halAdcCLK;//ADC时钟分频

typedef enum{
	HAL_ADC_INT_EN = 1,//开启中断模式
	HAL_ADC_INT_DS = 0,//关闭中断模式
}HalAdcIntState;//选择中断形式

extern u8 adcTimeout;

void HalAdcInit(halAdcCLK clock,HalAdcIntState intEn);
u16 HalAdcRead(u8 channel);
u16 HalAdcReadNum(u8 channel,u8 num);

/*中断形式*/
void HalAdcStart(u8 channel);
void HalAdcChangeChannel(u8 channel);

#endif