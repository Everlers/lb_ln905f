#ifndef _NTC_H_
#define _NTC_H_
#include "hal_sys.h"
#include "tempCtrl.h"
#include "autoCalibration.h"

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_NTC || (AUTOC_ENABLE == 1 && AUTOC_EXTI_TEMP_COMMON == 0)

#define NTC_MAX_LIST					227//NTC最大列表数量


float ntcTempCompute(float res);

#endif

#endif