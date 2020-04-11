#include "stpid.h"
#include "string.h"

//代替arm内核的饱和运算:q15_t __SSAT(q31_t,16)
q15_t ssat(q31_t x)
{
	if(x > 32767)return 32767;
	if(x < -32768)return -32768;
	return x;
}

//PID初始化比例/积分/微分
void arm_pid_init_q15(arm_pid_instance_q15 * S,int32_t resetStateFlag)
{
  /* Run the below code for Cortex-M0 */
	
  q31_t temp;                                    /*to store the sum */

  /* Derived coefficient A0 */
  temp = S->Kp + S->Ki + S->Kd;
  S->A0 = (q15_t) ssat(temp);

  /* Derived coefficients and pack into A1 */
  temp = -(S->Kd + S->Kd + S->Kp);
  S->A1 = (q15_t) ssat(temp);
  S->A2 = S->Kd;
	
  /* Check whether state needs reset or not */
  if(resetStateFlag)
  {
    /* Clear the state buffer.  The size will be always 3 samples */
    memset(S->state, 0, 3u * sizeof(q15_t));
  }
}

//PID运算
q15_t arm_pid_q15(arm_pid_instance_q15 * S,q15_t in)
{
	q63_t acc;
	q15_t out;
	
	/* acc = A0 * x[n]  */
	acc = ((q31_t) S->A0) * in;
	
	/* acc += A1 * x[n-1] + A2 * x[n-2]  */
	acc += (q31_t) S->A1 * S->state[0];
	acc += (q31_t) S->A2 * S->state[1];
	
	/* acc += y[n-1] */
	acc += (q31_t) S->state[2] << 15;
	
	/* saturate the output */
	out = (q15_t) (ssat((acc >> 15)));
	
	/* Update state */
	S->state[1] = S->state[0];
	S->state[0] = in;
	S->state[2] = out;
	
	/* return to application */
	return (out);
}
