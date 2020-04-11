#include "protocol.h"
#include "hal_uart.h"
#include "fun.h"

#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_EXTI_TEMP_COMMON == 1

static xdata u8 	state = 0;
static xdata u8 	rxState = 0;
static xdata u8 	recvLen = 0;
static xdata u16 	recvTimeout = 0;

static xdata u8 	rxBuf[sizeof(protocol_t)];
static xdata u8 	txBuf[sizeof(protocol_t)];

//初始hi
void ptlInit(void)
{
	state = 0;
	rxState = 0;
	recvLen = 0;
	recvTimeout = 0;
}

//任务
void ptlTask(void)
{
	if(state & PTL_STA_SEND)//发送数据
	{
		HalUARTWrite(HAL_UART_0,txBuf,sizeof(txBuf));
		state &= ~PTL_STA_SEND;
	}
	if(rxState & PTL_RXSTA_RECV)//接收数据
	{
		
	}
	if(recvTimeout == 0 && rxState & PTL_RXSTA_START)//接收超时
	{
		rxState &= ~(PTL_RXSTA_START);
		recvLen = 0;
	}
}

//通讯协议结构体初始化
//提供协议结构体参数
void ptlDeInit(protocol_t *p)
{
	u8 i;
	p->start = PTL_START_DATA;
	p->len = 0;
	p->cmd = 0;
	for(i=0;i<PTL_MAX_DATA_LEN;i++)
		p->dat[i] = 0;
	p->check = 0;
}

//发送通讯协议
//提供协议结构体参数
void ptlSend(protocol_t *p)
{
	u8 i;
	p->check = ptlCheckCompute(p);//计算校验
	txBuf[0] = p->start;
	txBuf[1] = p->len;
	txBuf[2] = p->cmd;
	for(i=0;i<PTL_MAX_DATA_LEN;i++)
		txBuf[i+3] = p->dat[i];
	txBuf[sizeof(txBuf)-1] = p->check;
	state |= PTL_STA_SEND;//标记需要发送数据
}

//读取通讯协议
//提供需要接收的通讯协议
//返回值：0:没数据 1:读取到数据 2:校验错误
u8 ptlRead(protocol_t *p)
{
	if(rxState & PTL_RXSTA_RECV)
	{
		u8 i;
		ptlDeInit(p);
		p->start = rxBuf[0];
		p->len = rxBuf[1];
		p->cmd = rxBuf[2];
		for(i=0;i<PTL_MAX_DATA_LEN;i++)
			p->dat[i] = rxBuf[3+i];
		p->check = ptlCheckCompute(p);//计算校验
		if(p->check == rxBuf[sizeof(rxBuf)-1])
		{
			recvLen = 0;
			rxState &= ~PTL_RXSTA_RECV;
			return 1;
		}
		else
		{
			recvLen = 0;
			rxState &= ~PTL_RXSTA_RECV;
			return 2;
		}
	}
	else
		return 0;
}

//通讯协议的校验计算
u8 ptlCheckCompute(protocol_t *p)
{
	u8 i;
	u32 num = 0;
	num += p->start;
	num += p->len;
	num += p->cmd;
	for(i=0;i<PTL_MAX_DATA_LEN;i++)
		num += p->dat[i];
	return (u8)num;
}

//串口接收回调
void ptlUartCallBack(u8 dat)
{
	if(rxState & PTL_RXSTA_START && !(rxState & PTL_RXSTA_RECV))//未接收完成且接收到开始信号
	{
		if(recvLen < sizeof(protocol_t))
			rxBuf[recvLen++] = dat;
		if(recvLen >= sizeof(protocol_t))//如果接收完成
			rxState |= PTL_RXSTA_RECV;
	}
	else if(dat == PTL_START_DATA && !(rxState & PTL_RXSTA_RECV))//接收到开始数据
	{
		recvLen = 1;
		rxBuf[0] = dat;
		rxState |= PTL_RXSTA_START;
	}
	recvTimeout = PTL_RECV_TIMEOUT;
}

//定时器回调
void ptlTimeCallBack(void)
{
	if(recvTimeout)
		recvTimeout--;
}

#endif