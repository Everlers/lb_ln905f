#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "hal_sys.h"
#include "fun.h"
#include "tempCtrl.h"
#include "autoCalibration.h"

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_EXTI_TEMP_COMMON == 1

#define PTL_MAX_DATA_LEN					10
#define PTL_START_DATA						0xAA
#define PTL_RECV_TIMEOUT					100

//state
#define PTL_STA_SEND								0x01//发送数据

//rxState
#define PTL_RXSTA_START							0x01//开始接收数据
#define PTL_RXSTA_RECV							0x02//接收到数据

typedef struct{
	u8 start;
	u8 len;
	u8 cmd;
	u8 dat[PTL_MAX_DATA_LEN];
	u8 check;
}protocol_t;

void 	ptlInit(void);
void 	ptlTask(void);
void 	ptlDeInit(protocol_t *p);
void 	ptlSend(protocol_t *p);
u8 		ptlRead(protocol_t *p);
u8 		ptlCheckCompute(protocol_t *p);
void 	ptlUartCallBack(u8 dat);
void 	ptlTimeCallBack(void);
#endif

#endif