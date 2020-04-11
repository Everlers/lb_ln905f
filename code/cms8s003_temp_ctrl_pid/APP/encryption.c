#include "encryption.h"
#include "hal_flash.h"
#include "hal_uart.h"
#include "string.h"
#include "delay.h"
#include "fun.h"

#if EMPOWER_CODE_WRITE
//芯片授权
//将芯片ID通过简单运算写入Flash 
void encryptionEmpower(void)
{
	volatile u8 xdata *CPUID = MCU_UUID_ADDR;
	u8 i,tmp[CPUID_LENGTH],tmp1[CPUID_LENGTH];//计算后的MCUID 加密后的MCUID
	for(i=0;i<CPUID_LENGTH;i++)//计算CPUID
		tmp[i] = CPUID[CPUID_LENGTH-i-1]^CPUID[i];
	HalFlashRead(HAL_FLASH_DATA,ID_WRITE_ADDR,tmp1,CPUID_LENGTH);//读取授权信息
	if(memcmp(tmp1,tmp,CPUID_LENGTH))//如果没有储存加密后的ID
	{
		HalFlashWrite(HAL_FLASH_DATA,ID_WRITE_ADDR,tmp,CPUID_LENGTH);//保存加密后的CPUID到Flash
	}
}
#endif

//取消加密授权
void encryptionCancelEmpower(void)
{
	u8 Clear[]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	HalFlashWrite(HAL_FLASH_DATA,ID_WRITE_ADDR,Clear,CPUID_LENGTH);
}

//授权验证
void encryptionEmpowerCheck(void)
{
	volatile u8 xdata *CPUID = MCU_UUID_ADDR;
	u8 i,tmp[CPUID_LENGTH],tmp1[CPUID_LENGTH];//计算后的MCUID 加密后的MCUID
	for(i=0;i<CPUID_LENGTH;i++)//计算CPUID
		tmp[i] = CPUID[CPUID_LENGTH-i-1]^CPUID[i];
	HalFlashRead(HAL_FLASH_DATA,ID_WRITE_ADDR,tmp1,CPUID_LENGTH);//读取授权信息
	#if DEBUG_MCUID == 1
	debug("[encryption] SMCUID: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
					,(u16)UID0,(u16)UID1,(u16)UID2,(u16)UID3,(u16)UID4,(u16)UID5,(u16)UID6,(u16)UID7,(u16)UID8,(u16)UID9);
	debug("%02X%02X\r\n",(u16)UID10,(u16)UID11);
	debug("[encryption] CMCUID: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
					,(u16)tmp[0],(u16)tmp[1],(u16)tmp[2],(u16)tmp[3],(u16)tmp[4],(u16)tmp[5],(u16)tmp[6],(u16)tmp[7],(u16)tmp[8],(u16)tmp[9]);
	debug("%02X%02X\r\n",(u16)tmp[10],(u16)tmp[11]);
	debug("[encryption] FMCUID: 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
					,(u16)tmp1[0],(u16)tmp1[1],(u16)tmp1[2],(u16)tmp1[3],(u16)tmp1[4],(u16)tmp1[5],(u16)tmp1[6],(u16)tmp1[7],(u16)tmp1[8],(u16)tmp1[9]);
	debug("%02X%02X\r\n",(u16)tmp1[10],(u16)tmp1[11]);
	#endif
	if(memcmp(tmp,tmp1,CPUID_LENGTH)){//如果ID不正确(未授权)
		u8 Countdown;
		debug("[encryption] Chip not authorized!!!\r\n");
		if(HalFlashDefault() != HAL_FLASH_DONE)//如果是刚出厂的板子
		{
			Countdown = TERM_COUNTDOWN;//写入默认的运行次数
			HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_ENCRYPTION_NUM_ADDR,(u8 *)&Countdown,sizeof(Countdown));//写入次数
		}
		else
			HalFlashRead(HAL_FLASH_DATA,HAL_FLASH_ENCRYPTION_NUM_ADDR,&Countdown,sizeof(Countdown));
		if(Countdown == 0){//如果计数完成(使用次数达到)
			debug("[encryption] Please contact YF Technical Department\r\n");
			while(i)
			{
				if(!LED1){
					LED1 = 1;
					LED2 = 0;
				}
				else if(!LED2){
					LED2 = 1;
					LED3 = 0;
				}
				else if(!LED3){
					LED3 = 1;
					LED4 = 0;
				}
				else if(!LED4){
					LED4 = 1;
					LED5 = 0;
				}
				else if(!LED5){
					LED5 = 1;
					LED1 = 0;
				}
				else
					LED1 = 0;
				delay_ms(100);
			}
		}
		else{
			debug("[encryption] Remaining times of use ==> %u\r\n",(u16)Countdown);
			Countdown --;
			HalFlashWrite(HAL_FLASH_DATA,HAL_FLASH_ENCRYPTION_NUM_ADDR,(u8 *)&Countdown,sizeof(Countdown));
		}
	}
}
