#ifndef _NIXIELIGHT_H_
#define _NIXIELIGHT_H_
#include "hal_sys.h"

#define NL_NUM							4
#define NL_BLANK_FREQ				800

#define NL1 	P0_1
#define NL2		P0_5
#define NL3		P3_6
#define NL4		P0_3

#define NL_STA_BLANK				0x01

#define NL_SHOW_NULL				10//显示空
#define NL_SHOW_SIGN				11//显示符号-
#define NL_SHOW_E						12//显示E
#define NL_SHOW_F						13//显示F
#define NL_ICON_FC					6//显示华氏度
#define NL_ICON_DC					7//显示摄氏度
#define NL_ICON_RE					8//显示READY

typedef struct{
	u8 	state;
	u8 	index;
	u8 	show[NL_NUM];
	u16 blankTime;
	u8	openTime;
}nl_t;

void nlInit(void);
void nlTask(void);
void nlClean(void);
void nlBlank(bit sw);
void nlShowErrCode(u8 c);
void nlSet(u8 index,u8 show);
void nlShowNum(u16 num);
void nlSetVolume(u8 volume);
void nlSetIcon(u8 index,u8 sw);
#endif