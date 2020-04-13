#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_
#include "hal_sys.h"

#define HAL_PWM_EPREIOD_VAL				3500

void HalPwmInit(void);
void HalPwmSetDutyCycle(u16 duty);

#endif