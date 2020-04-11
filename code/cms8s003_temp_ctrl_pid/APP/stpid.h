#ifndef _STPID_H_
#define _STPID_H_
#include "hal_sys.h"

typedef struct
{
	q15_t A0;           /**< The derived gain, A0 = Kp + Ki + Kd . */
	q15_t A1;
	q15_t A2;
	q15_t state[3];     /**< The state array of length 3. */
	q15_t Kp;           /**< The proportional gain. */
	q15_t Ki;           /**< The integral gain. */
	q15_t Kd;           /**< The derivative gain. */
}arm_pid_instance_q15;

void arm_pid_init_q15(arm_pid_instance_q15 * S,int32_t resetStateFlag);
q15_t arm_pid_q15(arm_pid_instance_q15 * S,q15_t in);
#endif
