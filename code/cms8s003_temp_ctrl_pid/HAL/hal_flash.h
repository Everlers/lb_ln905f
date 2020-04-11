#ifndef _HAL_FLASH_H_
#define _HAL_FLASH_H_
#include "hal_sys.h"

//数据区域 1024字节
//程序区域 16KByte
//一个扇区512Byte

#define HAL_FLASH_BACKUPS_DATA							1//启用数据备份功能(无需手动擦除扇区) 注意：使用此功能会增加RAM空间512Byte
#define HAL_FLASH_SECTION_SIZE							512//扇区大小512Byte
#define HAL_FLASH_DATA_START								0x00//数据区域的开始地址0~1024
#define	HAL_FLASH_DEFAULE_VALUE							0xAA55AA55//用于识别出厂设置

#define	HAL_FLASH_DEFAULT_ADDR 							HAL_FLASH_DATA_START//0~3储存标志位 识别是否为出厂模式 - 4 Byte
#define HAL_FLASH_ADJ_TEMP_ADDR							HAL_FLASH_DEFAULT_ADDR+4//储存调节温度用的Flash地址 - 8 Byte
#define HAL_FLASH_RES_SELECT_ADDR						HAL_FLASH_ADJ_TEMP_ADDR+8//储存MCH发热体阻值选择的Flash地址 - 1 Byte
#define HAL_FLASH_SHOW_UNIT_ADDR						HAL_FLASH_RES_SELECT_ADDR+1//储存温度显示单位的Flash地址 - 1 Byte
#define HAL_FLASH_SET_TEMP_ADDR							HAL_FLASH_SHOW_UNIT_ADDR+1//储存设定温度的Flash地址 - 1Byte
#define HAL_FLASH_TEMP_LOCK_ADDR						HAL_FLASH_SET_TEMP_ADDR+1//储存设定温度锁定的Flash地址 - 1Byte
#define	HAL_FLASH_ENCRYPTION_NUM_ADDR				HAL_FLASH_TEMP_LOCK_ADDR+1//储存授权失败后的使用次数 - 1Byte
#define	HAL_FLASH_ENCRYPTION_SAVE_ADDR			HAL_FLASH_ENCRYPTION_NUM_ADDR + 1//储存加密后的芯片UUID - 12Byte

#define	HAL_FLASH_NOP												_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();

typedef enum{
	HAL_FLASH_CODE = 0x00,
	HAL_FLASH_DATA = 0x10,
}HalFlashArea;//操作区域选择

typedef enum{
	HAL_FLASH_ERROR = 0,
	HAL_FLASH_DONE = 1,
}HalFlashError;//错误代码

HalFlashError HalFlashFlag(void);
HalFlashError HalFlashDefault(void);
HalFlashError HalFlashErase(HalFlashArea area,u16 addr);
HalFlashError HalFlashWrite(HalFlashArea area,u16 addr,u8 *p,u16 len);
HalFlashError HalFlashRead(HalFlashArea area,u16 addr,u8 *p,u16 len);

#endif