#include "hal_sys.h"
#include "hal_time.h"
#include "hal_adc.h"
#include "hal_pwm.h"
#include "hal_key.h"
#include "hal_flash.h"
#include "hal_buzzer.h"
#include "hal_uart.h"
#include "string.h"
#include "intrins.h"
#include "delay.h"
#include "stdio.h"
#include "siliconControl.h"
#include "fun.h"
#include "stpid.h"
#include "pid.h"
#include "mch.h"
#include "tempCtrl.h"
#include "test.h"
#include "protocol.h"
#include "autoCalibration.h"
#include "encryption.h"

void main (void)
{
	HalSysInit();//系统初始化
	HalUARTInit(HAL_UART_0,9600);//初始化串口
	//HalUARTInit(HAL_UART_1,9600);
	HalBuzzerInit();
	HalAdcInit(HAL_ADC_CLK_32,HAL_ADC_INT_DS);//初始化ADC
	HalTimeInit(1000);

	debug("Hal init OK.\r\n");
	siliconControlInit();//初始化可控硅
	pidInit();
	
#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH
	mchInit();
#endif
	
#if AUTOC_ENABLE == 1 && AUTOC_EXTI_TEMP_COMMON == 1
	ptlInit();
#endif
	
#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_ENABLE == 1
	autoCalibrationInit();
#endif
	
	test();//测试架
	
	encryptionEmpowerCheck();//执行代码保护程序

	tclInit();//初始化温度控制	
	funInit();//初始化功能

	if(HalFlashDefault() != HAL_FLASH_DONE)//如果是在出厂状态
		HalFlashFlag();//标记出厂设置完成
	while(1)
	{
		funShowManage();//显示任务
		funCtrlManage();//控制任务
		
		tclCollectManage();//采集任务
		tclTempManage();//恒温任务
		
#if AUTOC_ENABLE == 1 && TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_EXTI_TEMP_COMMON == 1
		ptlTask();//通讯协议任务
#endif
		
#if TCL_TEMP_COLL_SELECT == TCL_TEMP_MCH && AUTOC_ENABLE == 1
		autoCalibrationTask();//自动校准任务
		autoCTempCollManage();//自动校准实际温度采集任务
#endif
	}
}
           