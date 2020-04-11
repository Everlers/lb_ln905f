#include "hal_flash.h"
#include "hal_uart.h"
#include "intrins.h"

u32 xdata defaultValue;
#if HAL_FLASH_BACKUPS_DATA == 1
u8  xdata backupsData[HAL_FLASH_SECTION_SIZE];//备份数据
#endif

//检测是否为默认出厂设置
HalFlashError HalFlashDefault(void)
{
	HalFlashRead(HAL_FLASH_DATA,HAL_FLASH_DEFAULT_ADDR,(u8 *)&defaultValue,4);
	return (defaultValue == HAL_FLASH_DEFAULE_VALUE) ? HAL_FLASH_DONE : HAL_FLASH_ERROR;
}

//标记Flash被写过 非出厂设置
HalFlashError HalFlashFlag(void)
{
	if(HalFlashDefault() != HAL_FLASH_DONE)//有写入过数值
	{
		defaultValue = HAL_FLASH_DEFAULE_VALUE;
		return HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_DEFAULT_ADDR,(u8 *)&defaultValue,4);
	}
	return HAL_FLASH_DONE;
}

//擦除对应地址的扇区
HalFlashError HalFlashErase(HalFlashArea area,u16 addr)
{
	MADRL		=	addr;							//IAP write/read address high byte
	MADRH		=	addr>>8;					//IAP write/read address low byte
	MCTRL		=	0x0D|area;							
	HAL_FLASH_NOP;
	while(MCTRL & 0x01);				//wait the erase operation complete
	if(MCTRL & 0x20)
		return HAL_FLASH_ERROR;
	return HAL_FLASH_DONE;
}

HalFlashError HalFlashWrite(HalFlashArea area,u16 addr,u8 *p,u16 len)
{
	u16 i,wAdd = addr,wLen = len,sLen = len;
	u8 *ptr = p;
#if HAL_FLASH_BACKUPS_DATA == 1//如果需要备份数据
	u16 secpos = addr & (~(HAL_FLASH_SECTION_SIZE - 1));//扇区首地址
	u16 seccoff = addr & (HAL_FLASH_SECTION_SIZE - 1);//扇区内的偏移地址
	WRITE:
	HalFlashRead(area,secpos,backupsData,HAL_FLASH_SECTION_SIZE);//读出整个扇区
	HalFlashErase(area,secpos);//擦除扇区
	for(i = 0; i < MIN_VALUE(sLen,HAL_FLASH_SECTION_SIZE - seccoff); i ++)
		backupsData[seccoff+i] = p[i];
	wAdd = secpos;//从扇区首地址写入
	wLen = HAL_FLASH_SECTION_SIZE;//写入整个扇区
	ptr = backupsData;//写入整个扇区
#endif
	for(i=0;i<wLen;i++)
	{
		MADRL = wAdd;
		MADRH = wAdd >> 8;
		MDATA = ptr[i];
		MCTRL = 0x09|area;
		HAL_FLASH_NOP;
		while(MCTRL & 0x01);
		wAdd++;
		if(MCTRL & 0x20)
			return HAL_FLASH_ERROR;
	}
#if HAL_FLASH_BACKUPS_DATA == 1//如果需要备份数据
	if(sLen > (HAL_FLASH_SECTION_SIZE - seccoff))//如果还有剩余数据未写入(跨越扇区)
	{
		sLen = len - (HAL_FLASH_SECTION_SIZE - seccoff);//计算剩余的写入长度
		secpos += HAL_FLASH_SECTION_SIZE;//偏移到下一个扇区
		seccoff = 0;//偏移地址设置0
		goto WRITE;//再次写入
	}
#endif
	return HAL_FLASH_DONE; 
}

HalFlashError HalFlashRead(HalFlashArea area,u16 addr,u8 *p,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		MADRL = addr;
		MADRH = addr >> 8;
		MCTRL		=	0x01|area;
		HAL_FLASH_NOP;
		while(MCTRL & 0x01);
		p[i] = MDATA;
		addr++;
		if(MCTRL & 0x20)
			return HAL_FLASH_ERROR;
	}
	return HAL_FLASH_DONE;
}
