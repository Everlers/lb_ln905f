#include "nixieLight.h"

nl_t xdata nl;
const u8 nlList[]={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0x00,0x02,0x9E,0x8E};//'_''-''E''F'
static void nlSetData(u8 dat);

void nlInit(void)
{
	nl.state = 0;
	nl.blankTime = 0;
	nl.index = 0;
	nlClean();
}

void nlBlank(bit sw)
{
	if(sw){
		if(!(nl.state & NL_STA_BLANK))
		nl.state |= NL_STA_BLANK;
	}
	else if(nl.state & NL_STA_BLANK)
		nl.state &= ~NL_STA_BLANK;
}

void nlClean(void)
{
	u8 i;
	for(i=0;i<NL_NUM;i++)
		nl.show[i] = 0x00;
}

void nlShowErrCode(u8 c)
{
	if(c > 9)c = 9;
	nlSet(0,NL_SHOW_E);
	nlSet(1,NL_SHOW_SIGN);
	nlSet(2,c%10);
}

void nlShowNum(u16 num)
{
	if(num > 999)num = 999;
	nlSet(0,num/100);
	nlSet(1,num/10%10);
	nlSet(2,num%10);
}

void nlSetVolume(u8 volume)
{
	u8 i=0;
	for(i=0;i<6;i++)
	{
		if(volume > i)
			nlSetIcon(i,1);
		else
			nlSetIcon(i,0);
	}
}

void nlSetIcon(u8 index,u8 sw)
{
	if(index < 6)//volume
	{
		nl.show[3] = (sw) ? nl.show[3]|BV(index+1) : nl.show[3]&~BV(index+1);
	}
	else
	{
		switch(index)
		{
			case 6://»ªÊÏ¶È
				nl.show[1] = (sw) ? nl.show[1]|0x01 : nl.show[1]&~0x01;
			break;
			case 7://ÉãÊÏ¶È
				nl.show[3] = (sw) ? nl.show[3]|0x01 : nl.show[3]&~0x01;
			break;
			case 8://READY
				nl.show[2] = (sw) ? nl.show[2]|0x01 : nl.show[2]&~0x01;
			break;
		}
	}
}

void nlSet(u8 index,u8 show)
{
	nl.show[index] = (nlList[show]&0xFE)|(nl.show[index]&0x01);
}

void nlTask(void)
{
	NL1 = NL2 = NL3 = NL4 = 0;
	
	if(nl.index <= 2 && ((nl.state & NL_STA_BLANK) && nl.blankTime <= (NL_BLANK_FREQ/2)))
	{
		nlSetData(nl.show[nl.index]&0x01);
	}
	else
		nlSetData(nl.show[nl.index]);
	switch(nl.index)
	{
		case 0:NL1 = 1;break;
		case 1:NL2 = 1;break;
		case 2:NL3 = 1;break;
		case 3:NL4 = 1;break;
	}
	
	nl.index ++;
	if(nl.index >= NL_NUM)
		nl.index = 0;
	if(nl.blankTime)
		nl.blankTime--;
	if(nl.blankTime == 0)
		nl.blankTime = NL_BLANK_FREQ;
}

static void nlSetData(u8 dat)
{
	P1_3 = !(dat & 0x80) ? 1:0;//A
	P3_0 = !(dat & 0x40) ? 1:0;//B
	P1_7 = !(dat & 0x20) ? 1:0;//C
	P1_5 = !(dat & 0x10) ? 1:0;//D
	P1_4 = !(dat & 0x08) ? 1:0;//E
	P0_0 = !(dat & 0x04) ? 1:0;//F
	P3_2 = !(dat & 0x02) ? 1:0;//G
	P1_6 = !(dat & 0x01) ? 1:0;//H
}

