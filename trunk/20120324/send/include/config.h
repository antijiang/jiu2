/******************************************************************

 *******************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__
#define DEBUG0
#define DEBUG1
//#define DEBUGn
#define	DEF_ENABLE	1
#define	DEF_DISABLE	0
#define DEBUG_VERSION

#define		SECURE_SPI 	//加密处理IC
#ifdef	SECURE_SPI
#define	PMCLK	P07
#define	PSCLK	P11
#define	PSDA	P12
#endif


#ifndef	SECURE_SPI
#define	NVRAM_USE_EEP24CXX		//适用外部存储
#endif
//#define	NVRAM_USE_EEP24CXX		//适用外部存储


#define	DISPLAY_SYSINFO	//允许显示系统信息
#define	DEBUG_BYLCD

#define	USE_CPU_TEMPERATURE  //定义使用内部温度检测
#define	TEST_IR
#define	EN_UART1		DEF_ENABLE

#if 0
#define	   SensorPowerOn()	P06=1		//取样电源
#define	   SensorPowerOff()	P06=0
#else
//#define		SensorPowerOn()  	TEST_SPI(CMD_SET_P00)
//#define		SensorPowerOff()  TEST_SPI(CMD_CLR_P00)
#define		SensorPowerOn()  	{SetPWM(02,0xff);TEST_SPI(CMD_SET_P00);}
#define		SensorPowerOff()  {SetPWM(02,00);TEST_SPI(CMD_CLR_P00);}

#endif
#ifndef	  SECURE_SPI
#define	   SensorHeatPowerOn()	//P07=0	//灯丝
#define	   SensorHeatPowerOff()	//P07=1
#else
#define	   SensorHeatPowerOn()
#define	   SensorHeatPowerOff()
#endif
#define	SensorStart100HZ()

#define STANDARD_REF_VOLT	3000	   //系统输出标准参考电压
#define	DEFAULT_NOCOAL_VA	1580		   //无酒精
#define	DEFAULT_NOCOAL_VB	2180
#define	DEFALT_130PPM_VC	1650
#define	DEFALT_52PPM_VC20	1900

#define MAX_ALCOHOLICITY1	  (40)
#define MAX_ALCOHOLICITY2	  (104)
#define MAX_ALCOHOLICITY3	  (156)

#define	NORMAL_SESOR_POWER_VAL	 (WORD((DWORD)2000*1024/STANDARD_REF_VOLT))
#if 0
#define	LED_ON		0
#define	LED_OFF		1
#define	LEDIO	P05

//#define	LED_RED(x)		P05=x
//#define	LED_GREEN(x)	P20=x
//#define	BUZZY_ON()		P17=1
//#define	BUZZY_OFF()		P17=0
#endif

#define	LED_ON		0xff
#define	LED_OFF		0x0
#define	LEDIO	P05
#define	LED_RED(x)
#define	LED_GREEN(x)	//SetPWM(02,x)
#define	BUZZY_ON()		{SetPWM(00,0xff);BZY_on=1;}
#define	BUZZY_OFF()		{SetPWM(00,0x00);BZY_on=0;}

#define	LCDBAK_ON		0xff
#define	LCDBAK_OFF		0
#define	LCDBAKLIGHT(x)	SetPWM(1,x)

#define	SW_DETECT()		P13

//p15
#define	PIO_ADJUST_DET	 	P15  //检测  酒精校准50mg  复用无线发送脚
#define	PIO_CFG_DET	 		0x20    //1<<5
//p00
#define	PIO_ADJUST_DET20	 	P00  //20mg calibrate
#define	PIO_CFG_DET20	 		0x01    //1<<5
//不定义为直流 郑州
//#define  FANGBO_PWR    //戴维来方波 定义


#define	P_LCD_ON		0
#define	P_LCD_OFF		1

#define	PO_LCD_POWER(x)		P17=x		 //lcd电源
#define	PIO_CHARG			P20
#define	PCHARG_DET()		(P20==1)		  //5V充电电源检测
#define	PCHARG_FULL()		(P05==1)		  //
#define	PWR_G1		P04
#define	PWR_G2		P01

#endif
