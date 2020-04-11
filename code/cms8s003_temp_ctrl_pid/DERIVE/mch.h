#ifndef _MCH_H_
#define _MCH_H_
#include "hal_sys.h"
#include "tempCtrl.h"

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH

#define MCH_TEMMP_PIONT_NUM						9//温度点数量
#define MCH_DEFAULT_ADJ_TEMP_OFFSET		0//默认温度调节偏差值

typedef struct{
	float adjTemp;		//调节温度
}mch_t;
extern mch_t mch;

void mchInit(void);
float mchTempGet(float res);
u16 mchGetMaxTemp(void);
float mchAdjustValueGet(void);
void mchAdjustValueSet(float value);

#endif

#endif