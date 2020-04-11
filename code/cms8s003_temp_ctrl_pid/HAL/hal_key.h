#ifndef _HAL_KEY_H_
#define _HAL_KEY_H_
#include "hal_sys.h"

#define HAL_KEY_SHAKE_TIME						20//20ms祛抖时间
#define HAL_KEY_LONG_PRESS_TIME				1800//1800ms长按时间

#define HAL_KEY_POWER								BV(1)
#define HAL_KEY_LOCK								BV(2)
#define HAL_KEY_SET									BV(3)
#define HAL_KEY_ALL									(HAL_KEY_POWER|HAL_KEY_LOCK|HAL_KEY_SET)

/*io pin config*/
#define HAL_KEY_PORT  P0
#define HAL_KEY_SW_1	BV(1)
#define HAL_KEY_SW_2	BV(3)
#define HAL_KEY_SW_3	BV(2)
#define HAL_KEY_SW_ALL (HAL_KEY_SW_1|HAL_KEY_SW_2|HAL_KEY_SW_3)

void HalKeyTask(void);
bit HalKeyGetShortPress(u8 sw);
bit HalKeyGetLongPress(u8 sw);
u8 HalKeyGetShortPressNoClear(u8 sw);
u8 HalKeyGetLongPressNoClear(u8 sw);
bit HalKeyGetLoosen(u8 sw);

#endif