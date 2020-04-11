/*--------------------------------------------------------------------------
  CMS8S003.H
 Registers definition for CMS8S003

Copyright CMS Semicon Co., Ltd
 All rights reserved.
-------------------------------------------------------------------------- */
#ifndef _CMS8S003XX_H_
#define _CMS8S003XX_H_

#include "intrins.h"

#define MAIN_CLOCK		8000000UL	     // 

#define UART_CKGEN 	"T4"     // "T1" or "T4"
/**********************************************************/
/**
 * IO definitions
 *
 * define access restrictions to peripheral registers
 */
#define     __I     volatile const   /*!< defines 'read only' permissions     */
#define     __O     volatile         /*!< defines 'write only' permissions    */
#define     __IO    volatile         /*!< defines 'read / write' permissions  */

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

/*!< STM8 Standard Peripheral Library old types (maintained for legacy purpose) */

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


typedef enum
{
	FALSE = 0, 
	TRUE = 1
}bool;

typedef enum
{
	RESET = 0, 
	SET = 1
}FlagStatus, ITStatus, BitStatus, BitAction;

typedef enum{DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONALSTATE_OK(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     (255)
#define S8_MAX     (127)
#define S8_MIN     (-128)
#define U16_MAX    (65535uL)
#define S16_MAX    (32767)
#define S16_MIN    (-32768)
#define U32_MAX    (4294967295uL)
#define S32_MAX    (2147483647)
#define S32_MIN    (-2147483648uL)

	
/* *********************************************************************** */
/* *********************************************************************** */

/*---------------------------------------------------------------------------
  Defined registers  
---------------------------------------------------------------------------*/

/*  BYTE Register  */
	sfr P0        = 0x80; /* Port 0                    */
	sfr SP        = 0x81;
	sfr DPL0      = 0x82;
	sfr DPH0      = 0x83;
	sfr DPL1      = 0x84; /* Data Pointer 1 Low byte   */
	sfr DPH1      = 0x85; /* Data Pointer 1 High byte  */
	sfr DPS       = 0x86; /* Data pointers select      */
	sfr PCON      = 0x87;   /* Power Configuration       */
	sfr TCON      = 0x88;   /* Timer 0,1 Configuration   */
	sfr TMOD      = 0x89; /* Timer 0,1 Mode            */
	sfr TL0       = 0x8A; /* Timer 0 Low byte counter  */
	sfr TL1       = 0x8B; /* Timer 1 Low byte counter  */
	sfr TH0       = 0x8C; /* Timer 0 High byte counter */
	sfr TH1       = 0x8D; /* Timer 1 High byte counter */
	sfr CKCON     = 0x8E;   /* XDATA Wait States         */
	sfr CLKDIV    = 0x8F;   /* CPU Clock Setup         */
	sfr P1        = 0x90; /* Port 1                    */
	sfr FUNCCR    = 0x91;   /* */

	sfr DPX0      = 0x93; /* Data Page Pointer 0       */

	sfr DPX1      = 0x95; /* Data Page Pointer 1       */
	sfr TA        = 0x96; /* Timed Access protection   */
	sfr WDCON     = 0x97; /* Watchdog configuration    */
	sfr WDTCON    = 0x97; /* Watchdog configuration    */	
	sfr SCON      = 0x98; /* Serial 0 Configuration    */
	sfr SCON0     = 0x98;
	sfr SBUF      = 0x99; /* Serial 0 I/O Buffer       */
	sfr SBUF0     = 0x99;
	sfr P0TRIS    = 0x9A; /*                    */
	sfr P2        = 0xA0; /* Port 2                    */
	sfr P1TRIS    = 0xA1; /*                    */
	sfr P2TRIS    = 0xA2; /*                    */
	sfr P3TRIS    = 0xA3; /*                    */
	sfr IE        = 0xA8;   /* Interrupt Enable          */
	//sfr EIE     = 0xA9;   /* External Interrupt Enable */
	//sfr EIE1    = 0xA9;   /* External Interrupt Enable */
	sfr EIE2      = 0xAA;   /* External Interrupt Enable */
	sfr P0EXTIE		= 0xAC;
	sfr P1EXTIE		= 0xAD;
	sfr P2EXTIE		= 0xAE;
	sfr P3EXTIE 	= 0xAF;
	sfr	P3				= 0xB0;
	//sfr	EIF			= 0xB1;
	//sfr	EIF1		= 0xB1;	 
	sfr EIF2			= 0xB2;

	sfr P0EXTIF   = 0xB4;   /* P0 Ports External Interrupt Flag */
	sfr P1EXTIF   = 0xB5;   /* P1 Ports External Interrupt Flag */
	sfr P2EXTIF   = 0xB6;   /* P2 Ports External Interrupt Flag */
	sfr P3EXTIF   = 0xB7;   /* P3 Ports External Interrupt Flag */

	sfr IP        = 0xB8;
	sfr EIP1      = 0xB9; /* External Interr. Priority */
	sfr EIP2      = 0xBA; /* External Interr. Priority */

	sfr WKTCL     = 0xBC; /*                    */
	sfr WKTCH     = 0xBD; /*                    */	
	sfr BUZDIV    = 0xBE; /*                    */
	sfr BUZCON    = 0xBF; /*                    */
	sfr CCL1    	= 0xC2;  /* Comare/Capture/Reload 1 Low */
	sfr CCH1     	= 0xC3;  /* Comare/Capture/Reload 1 High*/
	sfr CCL2     	= 0xC4;  /* Comare/Capture/Reload 2 Low */
	sfr CCH2     	= 0xC5;  /* Comare/Capture/Reload 2 High*/
	sfr CCL3     	= 0xC6;  /* Comare/Capture/Reload 3 Low */
	sfr CCH3     	= 0xC7;  /* Comare/Capture/Reload 3 High*/
	sfr T2CON     = 0xC8; /* Timer 2 Configuration     */
	sfr T2IF      = 0xC9; /* Timer 2 Interrupt Flags   */
	sfr RLDL      = 0xCA; /* Timer 2 Reload Low byte   */
	sfr RLDH      = 0xCB; /* Timer 2 Reload High byte  */
	sfr TL2       = 0xCC; /* Timer 2 Low byte counter  */
	sfr TH2       = 0xCD; /* Timer 2 High byte counter */
	sfr CCEN      = 0xCE; /* Comare/Capture/Reload Enable*/
	sfr T2IE      = 0xCF; 

	sfr PSW       = 0xD0;   /* Program Status Word       */
	sfr ADCMPC		= 0xD1;	
	sfr T34MOD		= 0xD2;	
	sfr ADDLYL		= 0xD3;	
	sfr ADCMPL		= 0xD4;	
	sfr ADCMPH		= 0xD5;	
	sfr CAPCR1		= 0xD6; 
	sfr CAPCR0		= 0xD7;   
	sfr TL3				= 0xDA;	
	sfr TH3				= 0xDB;		
	sfr ADRESL    = 0xDC; /* ADC result high 8-bit    */
	sfr ADRESH    = 0xDD; /* ADC result low 8-bit    */
	sfr ADCON1    = 0xDE; /* ADC control 1    */
	sfr ADCON0    = 0xDF; /* ADC control 0    */
	sfr ACC       = 0xE0;   /* Accumulator               */
	sfr TL4				= 0xE2;	
	sfr TH4				= 0xE3;		
	sfr ADCON2		= 0xE9;	
	sfr SCON1     = 0xEA; /* Serial 1 Configuration    */
	sfr SBUF1     = 0xEB;
	sfr SPCR      = 0xEC; /* SPI Control Register      */
	sfr SPSR      = 0xED; /* SPI Status Register       */
	sfr SPDR      = 0xEE; /* SPI I/O Register          */
	sfr SSCR      = 0xEF; /* SPI Sleve Select Register */
	sfr B         = 0xF0;   /* B Working register        */
	sfr I2CSADR 	= 0xF1;   /* I2CS Address              */
	sfr I2CSCR  	= 0xF2;   /* I2CS Control Register     */
	sfr I2CSBUF 	= 0xF3;   /* I2CS I/O Buffer           */
	sfr I2CMSA   	= 0xF4;  /* I2CM Slave Address        */
	sfr I2CMCR   	= 0xF5;  /* I2CM Control Register     */
	sfr I2CMBUF  	= 0xF6;  /* I2CM I/O Buffer           */
	sfr I2CMTP   	= 0xF7;  /* I2CM Timer Period         */
	sfr MADRL			= 0xFC;
	sfr MADRH			= 0xFD;
	sfr MDATA			= 0xFE;
	sfr MCTRL			= 0xFF;


/*-------------------------------------------------------------------------*/
/*	sfr 16 */
/*-------------------------------------------------------------------------*/


	sfr16	T2	= 0xCC;		// {T2L,T2H}
	sfr16	DPTR0 = 0x82;				  //********++++//
	sfr16	DPTR1 = 0x84;				  //********++++//
	sfr16	CC1	= 0xC2;		// {CCL1,CCH1}
	sfr16	CC2	= 0xC4;		// {CCL2,CCH2}
	sfr16	CC3	= 0xC6;		// {CCL3,CCH3}
	sfr16	RCR	= 0xCA;		// {RCRL,RCRH}
	sfr16	RLD	= 0xCA;		// {RCRL,RCRH}
	sfr16 	ADCMP = 0xD4;  //{ADCMPL.ADCMPH}
	sfr16	ADRES	= 0xDC;		// {ADRESL,ADRESH}
	sfr16	EEADDR	= 0xEC;		// {EEADRL,EEADRH}
	sfr16	MADR	= 0xFC;		// {MADRL,MADRH}
/*-------------------------------------------------------------------------*/
/*	XSFR */
/*-------------------------------------------------------------------------*/
#define P00CFG *(volatile unsigned char xdata *) 0xF000
#define P01CFG *(volatile unsigned char xdata *) 0xF001
#define P02CFG *(volatile unsigned char xdata *) 0xF002
#define P03CFG *(volatile unsigned char xdata *) 0xF003
#define P04CFG *(volatile unsigned char xdata *) 0xF004
#define P05CFG *(volatile unsigned char xdata *) 0xF005
 //#define P06CFG *(volatile unsigned char xdata *) 0xF006
 //#define P07CFG *(volatile unsigned char xdata *) 0xF007
#define P0ANS *(volatile unsigned char xdata *) 0xF008
#define P0OD *(volatile unsigned char xdata *) 0xF009
#define P0UP *(volatile unsigned char xdata *) 0xF00A
#define P0RD	*(volatile unsigned char xdata *) 0xF00B
#define P0DR	*(volatile unsigned char xdata *) 0xF00C
#define P0SR	*(volatile unsigned char xdata *) 0xF00D
	
 //#define P10CFG *(volatile unsigned char xdata *) 0xF010
 //#define P11CFG *(volatile unsigned char xdata *) 0xF011
 //#define P12CFG *(volatile unsigned char xdata *) 0xF012
#define P13CFG *(volatile unsigned char xdata *) 0xF013
#define P14CFG *(volatile unsigned char xdata *) 0xF014
#define P15CFG *(volatile unsigned char xdata *) 0xF015
#define P16CFG *(volatile unsigned char xdata *) 0xF016
#define P17CFG *(volatile unsigned char xdata *) 0xF017
#define P1ANS *(volatile unsigned char xdata *) 0xF018
#define P1OD *(volatile unsigned char xdata *) 0xF019
#define P1UP *(volatile unsigned char xdata *) 0xF01A
#define P1RD	*(volatile unsigned char xdata *) 0xF01B
#define P1DR	*(volatile unsigned char xdata *) 0xF01C
#define P1SR	*(volatile unsigned char xdata *) 0xF01D
	
 //#define P20CFG *(volatile unsigned char xdata *) 0xF020
#define P21CFG *(volatile unsigned char xdata *) 0xF021
#define P22CFG *(volatile unsigned char xdata *) 0xF022
#define P23CFG *(volatile unsigned char xdata *) 0xF023
#define P24CFG *(volatile unsigned char xdata *) 0xF024
#define P25CFG *(volatile unsigned char xdata *) 0xF025
#define P26CFG *(volatile unsigned char xdata *) 0xF026
 //#define P27CFG *(volatile unsigned char xdata *) 0xF027
#define P2ANS *(volatile unsigned char xdata *) 0xF028
#define P2OD *(volatile unsigned char xdata *) 0xF029
#define P2UP *(volatile unsigned char xdata *) 0xF02A
#define P2RD	*(volatile unsigned char xdata *) 0xF02B
#define P2DR	*(volatile unsigned char xdata *) 0xF02C
#define P2SR	*(volatile unsigned char xdata *) 0xF02D	

#define P30CFG *(volatile unsigned char xdata *) 0xF030
#define P31CFG *(volatile unsigned char xdata *) 0xF031
#define P32CFG *(volatile unsigned char xdata *) 0xF032
 //#define P33CFG *(volatile unsigned char xdata *) 0xF033
 //#define P34CFG *(volatile unsigned char xdata *) 0xF034
#define P35CFG *(volatile unsigned char xdata *) 0xF035
#define P36CFG *(volatile unsigned char xdata *) 0xF036
 //#define P37CFG *(volatile unsigned char xdata *) 0xF037
#define P3ANS *(volatile unsigned char xdata *) 0xF038
#define P3OD *(volatile unsigned char xdata *) 0xF039
#define P3UP *(volatile unsigned char xdata *) 0xF03A
#define P3RD	*(volatile unsigned char xdata *) 0xF03B
#define P3DR	*(volatile unsigned char xdata *) 0xF03C
#define P3SR	*(volatile unsigned char xdata *) 0xF03D	


#define P00EICFG *(volatile unsigned char xdata *) 0xF080
#define P01EICFG *(volatile unsigned char xdata *) 0xF081
#define P02EICFG *(volatile unsigned char xdata *) 0xF082
#define P03EICFG *(volatile unsigned char xdata *) 0xF083
#define P04EICFG *(volatile unsigned char xdata *) 0xF084
#define P05EICFG *(volatile unsigned char xdata *) 0xF085
 //#define P06EICFG *(volatile unsigned char xdata *) 0xF086
 //#define P07EICFG *(volatile unsigned char xdata *) 0xF087

 //#define P10EICFG *(volatile unsigned char xdata *) 0xF088
 //#define P11EICFG *(volatile unsigned char xdata *) 0xF089
 //#define P12EICFG *(volatile unsigned char xdata *) 0xF08A
#define P13EICFG *(volatile unsigned char xdata *) 0xF08B
#define P14EICFG *(volatile unsigned char xdata *) 0xF08C
#define P15EICFG *(volatile unsigned char xdata *) 0xF08D
#define P16EICFG *(volatile unsigned char xdata *) 0xF08E
#define P17EICFG *(volatile unsigned char xdata *) 0xF08F

 //#define P20EICFG *(volatile unsigned char xdata *) 0xF090
#define P21EICFG *(volatile unsigned char xdata *) 0xF091
#define P22EICFG *(volatile unsigned char xdata *) 0xF092
#define P23EICFG *(volatile unsigned char xdata *) 0xF093
#define P24EICFG *(volatile unsigned char xdata *) 0xF094
#define P25EICFG *(volatile unsigned char xdata *) 0xF095
#define P26EICFG *(volatile unsigned char xdata *) 0xF096
 //#define P27EICFG *(volatile unsigned char xdata *) 0xF097

#define P30EICFG *(volatile unsigned char xdata *) 0xF098
#define P31EICFG *(volatile unsigned char xdata *) 0xF099
#define P32EICFG *(volatile unsigned char xdata *) 0xF09A
 //#define P33EICFG *(volatile unsigned char xdata *) 0xF09B
 //#define P34EICFG *(volatile unsigned char xdata *) 0xF09C
#define P35EICFG *(volatile unsigned char xdata *) 0xF09D
#define P36EICFG *(volatile unsigned char xdata *) 0xF09E
//#define P37EICFG *(volatile unsigned char xdata *) 0xF09F    //********++++//

#define PWMCON		*(volatile unsigned char xdata *) 0xF120
#define PWMOE			*(volatile unsigned char xdata *) 0xF121  
#define PWMPINV		*(volatile unsigned char xdata *) 0xF122  
#define PWM01PSC	*(volatile unsigned char xdata *) 0xF123  
#define PWM23PSC	*(volatile unsigned char xdata *) 0xF124  
#define PWM45PSC	*(volatile unsigned char xdata *) 0xF125  
#define PWMCNTE		*(volatile unsigned char xdata *) 0xF126  
#define PWMCNTM		*(volatile unsigned char xdata *) 0xF127  
#define PWMCNTCLR	*(volatile unsigned char xdata *) 0xF128  
#define PWMLOADEN	*(volatile unsigned char xdata *) 0xF129  
	
#define PWM0DIV		*(volatile unsigned char xdata *) 0xF12A  
#define PWM1DIV		*(volatile unsigned char xdata *) 0xF12B  
#define PWM2DIV		*(volatile unsigned char xdata *) 0xF12C  
#define PWM3DIV		*(volatile unsigned char xdata *) 0xF12D  
#define PWM4DIV		*(volatile unsigned char xdata *) 0xF12E  
#define PWM5DIV		*(volatile unsigned char xdata *) 0xF12F  

#define PWMP0L		*(volatile unsigned char xdata *) 0xF130  
#define PWMP0H		*(volatile unsigned char xdata *) 0xF131 
#define PWMP1L		*(volatile unsigned char xdata *) 0xF132 
#define PWMP1H		*(volatile unsigned char xdata *) 0xF133 
#define PWMP2L		*(volatile unsigned char xdata *) 0xF134 
#define PWMP2H		*(volatile unsigned char xdata *) 0xF135 
#define PWMP3L		*(volatile unsigned char xdata *) 0xF136 
#define PWMP3H		*(volatile unsigned char xdata *) 0xF137 
#define PWMP4L		*(volatile unsigned char xdata *) 0xF138 
#define PWMP4H		*(volatile unsigned char xdata *) 0xF139 
#define PWMP5L		*(volatile unsigned char xdata *) 0xF13A 
#define PWMP5H		*(volatile unsigned char xdata *) 0xF13B 

#define PWMD0L		*(volatile unsigned char xdata *) 0xF140  
#define PWMD0H		*(volatile unsigned char xdata *) 0xF141 
#define PWMD1L		*(volatile unsigned char xdata *) 0xF142 
#define PWMD1H		*(volatile unsigned char xdata *) 0xF143 
#define PWMD2L		*(volatile unsigned char xdata *) 0xF144 
#define PWMD2H		*(volatile unsigned char xdata *) 0xF145 
#define PWMD3L		*(volatile unsigned char xdata *) 0xF146 
#define PWMD3H		*(volatile unsigned char xdata *) 0xF147 
#define PWMD4L		*(volatile unsigned char xdata *) 0xF148 
#define PWMD4H		*(volatile unsigned char xdata *) 0xF149 
#define PWMD5L		*(volatile unsigned char xdata *) 0xF14A 
#define PWMD5H		*(volatile unsigned char xdata *) 0xF14B   

#define PWMDD0L		*(volatile unsigned char xdata *) 0xF150  
#define PWMDD0H		*(volatile unsigned char xdata *) 0xF151 
#define PWMDD1L		*(volatile unsigned char xdata *) 0xF152 
#define PWMDD1H		*(volatile unsigned char xdata *) 0xF153 
#define PWMDD2L		*(volatile unsigned char xdata *) 0xF154 
#define PWMDD2H		*(volatile unsigned char xdata *) 0xF155 
#define PWMDD3L		*(volatile unsigned char xdata *) 0xF156 
#define PWMDD3H		*(volatile unsigned char xdata *) 0xF157 
#define PWMDD4L		*(volatile unsigned char xdata *) 0xF158 
#define PWMDD4H		*(volatile unsigned char xdata *) 0xF159 
#define PWMDD5L		*(volatile unsigned char xdata *) 0xF15A 
#define PWMDD5H		*(volatile unsigned char xdata *) 0xF15B   

#define PWMDTE		*(volatile unsigned char xdata *) 0xF160 
#define PWM01DT		*(volatile unsigned char xdata *) 0xF161 
#define PWM23DT		*(volatile unsigned char xdata *) 0xF162
#define PWM45DT		*(volatile unsigned char xdata *) 0xF163 
	
#define PWMMASKE	*(volatile unsigned char xdata *) 0xF164 
#define PWMMASKD	*(volatile unsigned char xdata *) 0xF165 
#define PWMFBKC		*(volatile unsigned char xdata *) 0xF166 
#define PWMFBKD		*(volatile unsigned char xdata *) 0xF167 
	
#define PWMPIE		*(volatile unsigned char xdata *) 0xF168   
#define PWMZIE		*(volatile unsigned char xdata *) 0xF169   
#define PWMUIE		*(volatile unsigned char xdata *) 0xF16A   
#define PWMDIE		*(volatile unsigned char xdata *) 0xF16B   
#define PWMPIF		*(volatile unsigned char xdata *) 0xF16C   
#define PWMZIF		*(volatile unsigned char xdata *) 0xF16D   
#define PWMUIF		*(volatile unsigned char xdata *) 0xF16E   
#define PWMDIF		*(volatile unsigned char xdata *) 0xF16F



/*---------------??OPA??------------*/


#define C0CON0		*(volatile unsigned char xdata *) 0xF500  
#define C0CON1		*(volatile unsigned char xdata *) 0xF501 
#define C0CON2		*(volatile unsigned char xdata *) 0xF502
#define C1CON0		*(volatile unsigned char xdata *) 0xF503
#define C1CON1		*(volatile unsigned char xdata *) 0xF504
#define C1CON2		*(volatile unsigned char xdata *) 0xF505
#define CNVRCON		*(volatile unsigned char xdata *) 0xF506 
#define CNFBCON		*(volatile unsigned char xdata *) 0xF507 
#define CNIE	  	*(volatile unsigned char xdata *) 0xF508 
#define CNIF	  	*(volatile unsigned char xdata *) 0xF509 
#define C0ADJE		*(volatile unsigned char xdata *) 0xF50A 
#define C1ADJE		*(volatile unsigned char xdata *) 0xF50B 

#define OP0CON0		*(volatile unsigned char xdata *) 0xF520 
#define OP0CON1		*(volatile unsigned char xdata *) 0xF521 
#define OP1CON0		*(volatile unsigned char xdata *) 0xF523
#define OP1CON1		*(volatile unsigned char xdata *) 0xF524
#define OP0ADJE		*(volatile unsigned char xdata *) 0xF526
#define OP1ADJE		*(volatile unsigned char xdata *) 0xF527


#define UID0		*(volatile unsigned char xdata *) 0xF5E0 
#define UID1		*(volatile unsigned char xdata *) 0xF5E1 
#define UID2		*(volatile unsigned char xdata *) 0xF5E2
#define UID3		*(volatile unsigned char xdata *) 0xF5E3
#define UID4		*(volatile unsigned char xdata *) 0xF5E4
#define UID5		*(volatile unsigned char xdata *) 0xF5E5
#define UID6		*(volatile unsigned char xdata *) 0xF5E6 
#define UID7		*(volatile unsigned char xdata *) 0xF5E7 
#define UID8		*(volatile unsigned char xdata *) 0xF5E8
#define UID9		*(volatile unsigned char xdata *) 0xF5E9
#define UID10		*(volatile unsigned char xdata *) 0xF5EA
#define UID11		*(volatile unsigned char xdata *) 0xF5EB



	
/*-------------------------------------------------------------------------
  BIT Register  
  -------------------------------------------------------------------------*/

/*  P0  */

  sbit P0_7     = P0^7;
  sbit P0_6     = P0^6;
  sbit P0_5     = P0^5;
  sbit P0_4     = P0^4;
  sbit P0_3     = P0^3;
  sbit P0_2     = P0^2;
  sbit P0_1     = P0^1;
  sbit P0_0     = P0^0;


/*  P1  */

  sbit P1_7     = P1^7;
  sbit P1_6     = P1^6;
  sbit P1_5     = P1^5;
  sbit P1_4     = P1^4;
  sbit P1_3     = P1^3;
  sbit P1_2     = P1^2;
  sbit P1_1     = P1^1;
  sbit P1_0     = P1^0;
/*  P2  */
  sbit P2_7     = P2^7;
  sbit P2_6     = P2^6;
  sbit P2_5     = P2^5;
  sbit P2_4     = P2^4;
  sbit P2_3     = P2^3;
  sbit P2_2     = P2^2;
  sbit P2_1     = P2^1;
  sbit P2_0     = P2^0;
/*  P3  */
  sbit P3_7     = P3^7;
  sbit P3_6     = P3^6;
  sbit P3_5     = P3^5;
  sbit P3_4     = P3^4;
  sbit P3_3     = P3^3;
  sbit P3_2     = P3^2;
  sbit P3_1     = P3^1;
  sbit P3_0     = P3^0;
  
/*  TCON  */
  sbit IT0      = TCON^0;
  sbit IE0      = TCON^1;
  sbit IT1      = TCON^2;
  sbit IE1      = TCON^3;
  sbit TR0      = TCON^4;
  sbit TF0      = TCON^5;
  sbit TR1      = TCON^6;
  sbit TF1      = TCON^7;


/*  SCON0 */

  sbit RI0      = SCON0^0;
  sbit TI0      = SCON0^1;
  sbit U0RB8    = SCON0^2;
  sbit U0TB8    = SCON0^3;
  sbit U0REN    = SCON0^4;
  sbit U0SM2    = SCON0^5;
  sbit U0SM1    = SCON0^6;
  sbit U0SM0    = SCON0^7;

  sbit RB08     = SCON0^2;
  sbit TB08     = SCON0^3;
  sbit REN0     = SCON0^4;
  sbit SM02     = SCON0^5;
  sbit SM01     = SCON0^6;
  sbit SM00     = SCON0^7;
  
  /*  IE */
  sbit EX0     	= IE^0;
  sbit ET0     	= IE^1;
  sbit EX1     	= IE^2;
  sbit ET1     	= IE^3;
  sbit ES0     	= IE^4;
  sbit ET2     	= IE^5;
  sbit ES1     	= IE^6;
  sbit EA      	= IE^7;
  
  
    /*  IP */
  sbit PX0     	= IP^0;
  sbit PT0     	= IP^1;
  sbit PX1     	= IP^2;
  sbit PT1     	= IP^3;
  sbit PS0     	= IP^4;
  sbit PT2     	= IP^5;
  sbit PS1     	= IP^6;
  
    /*  T2CON */
  sbit T2I0     = T2CON^0;
  sbit T2I1     = T2CON^1;
  sbit T2CM     = T2CON^2;
  sbit T2R0     = T2CON^3;
  sbit T2R1     = T2CON^4;

  sbit I3FR     = T2CON^6;
  sbit T2PS     = T2CON^7;
  
    /* PSW  */
  sbit  P     	=PSW^0;

  sbit  OV    	=PSW^2;
  sbit  RS0   	=PSW^3;
  sbit  RS1    	=PSW^4;
  sbit  F0    	=PSW^5;
  sbit  AC    	=PSW^6;
  sbit  CY    	=PSW^7;
  
/****************************************************************************************/ 
/* PCON Bit Values */
#define IDLE   		0x01
#define STOP  		0x02
#define SWE   		0x04

#define SMOD1   	0x40
#define SMOD0 		0x80

/* TMOD Bit Values */
#define T0M0  	 	0x01
#define T0M1  	 	0x02
#define CT0  	 		0x04
#define GATE0 	 	0x08
#define T1M0  	 	0x10
#define T1M1  	 	0x20
#define CT1   		0x40
#define GATE1 		0x80

/* CKCON Bit Values */

#define T0M 	 		0x08
#define T1M   	 	0x10
 
 
 /* FUNCCR Bit Values */
#define UART0_CKS 0x01
#define UART1_CKS 0x02

#define I2C_PORTS 0x10

 /* WDTCON Bit Values */
#define	WDTCLR		0x01
#define	WDTRE			0x02
#define	WDTRF			0x04
#define	WDTIF			0x08

#define	PORF			0x40
#define	SWRST			0x80

 /* EIE2 Bit Values */
#define	ET3				0x01
#define	ET4				0x02

#define	PWMIE			0x08
#define	ADCIE			0x10
#define	WDTIE			0x20
#define	I2CIE			0x40
#define	SPIIE			0x80 

 /* EIF2 Bit Values */
#define	TF3				0x01
#define	TF4				0x02

#define	PWMIF			0x08
#define	ADCIF			0x10

#define	I2CIF			0x40
#define	SPIIF			0x80 

 /* EIP1 Bit Values */
#define	PP0				0x01
#define	PP1				0x02
#define	PP2				0x04
#define	PP3PWM		0x08
	
#define	PACMP			0x80 

 /* EIP2 Bit Values */
#define	PT3				0x01
#define	PT4				0x02

#define	PPWM			0x08
#define	PADC			0x10
#define	PWDT			0x20
#define	PI2C			0x40
#define	PSPI			0x80 

 /* BUZCON Bit Values */
#define	BUZCKS0		0x01
#define	BUZCKS1		0x02

#define	BUZEN			0x80

 /* T2IF Bit Values */
#define	T2C0IF		0x01
#define	T2C1IF		0x02
#define	T2C2IF		0x04
#define	T2C3IF		0x08

#define	T2EXIF		0x40
#define	TF2				0x80

 /* T2IE Bit Values */
#define	T2C0IE		0x01
#define	T2C1IE		0x02
#define	T2C2IE		0x04
#define	T2C3IE		0x08

#define	T2EXIE		0x40
#define	T2OVIE		0x80 

 /* ADCMPC Bit Values */

#define	ADCMPO		0x10

#define	ADCMPPS		0x40
#define	ADFBEN		0x80 

 /* T34MOD Bit Values */
#define	T3M0			0x01
#define	T3M1			0x02
#define	T3M				0x04
#define	TR3				0x08
#define	T4M0			0x10
#define	T4M1			0x20
#define	T4M				0x40
#define	TR4				0x80 

 /* ADCON1 Bit Values */
#define	ADCHS0		0x01
#define	ADCHS1		0x02
#define	ADCHS2		0x04
#define	ADCHS3		0x08
#define	ADCKS0		0x10
#define	ADCKS1		0x20
#define	ADCKS2		0x40
#define	ADEN			0x80 

 /* ADCON0 Bit Values */

#define	ADGO			0x02

#define	AN15SEL0	0x10
#define	AN15SEL1	0x20
#define	ADFM			0x40

 /* ADCON2 Bit Values */

#define	ADEGS0		0x04
#define	ADEGS1		0x08
#define	ADTGS0		0x10
#define	ADTGS1		0x20
#define	ADCPX			0x40
#define	ADCEX			0x80

 /* SCON1 Bit Values */
#define	RI1				0x01
#define	TI1				0x02
#define	U1RB8			0x04
#define	U1TB8			0x08
#define	U1REN			0x10
#define	U1SM2			0x20
#define	U1SM1			0x40
#define	U1SM0			0x80

 /* SPCR Bit Values */
#define	SPR0			0x01
#define	SPR1			0x02
#define	CPHA			0x04
#define	CPOL			0x08
#define	MSTR			0x10
#define	SPR2			0x20
#define	SPEN			0x40

 /* SPSR Bit Values */
#define	SSCEN			0x01
#define	SSICS			0x02

#define	MODF			0x10

#define	WCOL			0x40
#define	SPISIF		0x80

 /* SSCR Bit Values */

#define	SSO3			0x08

#define	SSO5			0x20
#define	SSO6			0x40
#define	SSO7			0x80

 /* I2CSCR Bit Values */
#define	DA				0x01

#define	RSTS			0x80

 /* I2CMCR Bit Values */
#define	I2C_G_RUN				0x01
#define	I2C_G_RSTART		0x02
#define	I2C_G_RSTOP			0x04
#define	I2C_G_RACK			0x08

 /* I2CMSR */
#define	I2C_F_BUSY			0x01
#define	I2C_F_ERROR			0x02
#define	I2C_F_ADD_ACK		0x04
#define	I2C_F_DATA_ACK	0x08
#define	I2C_F_ARB_LOST	0x10
#define	I2C_F_IDLE			0x20
#define	I2C_F_BUS_BUSY	0x40
#define	I2C_F_I2CMIF		0x80

//----------------------------------------------------
//----------------------------------------------------
#define _nop2_()					_nop_(), _nop_()
#define _nop3_()					_nop_(), _nop2_()
#define _nop4_()					_nop_(), _nop3_()	
#define _nop5_()					_nop_(), _nop4_()	
#define _nop6_()					_nop_(), _nop5_()		

#define clrwdt()					TA=0xAA, TA=0x55, WDTCON=0x01;

#define software_reset_system()		TA=0xAA, TA=0x55, WDTCON=0x80;	

#define idle()						_nop6_(), PCON=PCON|0x01, _nop6_()
#define sleep()						_nop6_(), PCON=PCON|0x06, _nop6_()	
#define stop()						_nop6_(), PCON=PCON|0x06, _nop6_()
#define stop_forever()		_nop6_(), PCON=PCON|0x02, _nop6_()

	
	
//----------------------------------------------------
//----------------------------------------------------
   
  
  /* DSPI Select Codes For SSCR register                         */
  #define SEL_7               0x80
  #define SEL_6               0x40
  #define SEL_5               0x20
  #define SEL_4               0x10
  #define SEL_3               0x08
  #define SEL_2               0x04
  #define SEL_1               0x02
  #define SEL_0               0x01




	
//----------------------------------------------------
//----------------------------------------------------
 /*   interrupt vector all 23  */
 #define         INT0_VECTOR			0
 #define         TIMER0_VECTOR		1
 #define         INT1_VECTOR			2
 #define         TIMER1_VECTOR   	3
 #define         UART0_VECTOR    	4
 #define         TIMER2_VECTOR		5
 #define         UART1_VECTOR    	6
 #define         P0EI_VECTOR			7
 #define         P1EI_VECTOR			8
 #define         P2EI_VECTOR			9
 #define         P3EI_VECTOR			10
 #define         TIMER3_VECTOR		15
 #define         TIMER4_VECTOR		16
 #define         EEDATA_VECTOR		17
 #define         PWM_VECTOR				18
 #define         ADC_VECTOR   		19
 #define         WDT_VECTOR       20
 #define         I2C_VECTOR				21
 #define         SPI_VECTOR				22

//----------------------------------------------------	


#endif /*_CMS8S003XX_H_*/