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

#define		SECURE_SPI 	//���ܴ���IC
#ifdef	SECURE_SPI
#define	PMCLK	P07
#define	PSCLK	P11
#define	PSDA	P12
#endif


#ifndef	SECURE_SPI
#define	NVRAM_USE_EEP24CXX		//�����ⲿ�洢
#endif
#define	NVRAM_USE_EEP24CXX		//�����ⲿ�洢


#define	DISPLAY_SYSINFO	//������ʾϵͳ��Ϣ
#define	DEBUG_BYLCD

#define	USE_CPU_TEMPERATURE  //����ʹ���ڲ��¶ȼ��
#define	TEST_IR
#define	EN_UART1		DEF_ENABLE

#if 0
#define	   SensorPowerOn()	P06=1		//ȡ����Դ
#define	   SensorPowerOff()	P06=0
#else
//#define		SensorPowerOn()  	TEST_SPI(CMD_SET_P00)
//#define		SensorPowerOff()  TEST_SPI(CMD_CLR_P00)
#define		SensorPowerOn()  	{SetPWM(02,0xff);TEST_SPI(CMD_SET_P00);}
#define		SensorPowerOff()  {SetPWM(02,00);TEST_SPI(CMD_CLR_P00);}

#endif
#ifndef	  SECURE_SPI
#define	   SensorHeatPowerOn()	//P07=0	//��˿
#define	   SensorHeatPowerOff()	//P07=1
#else
#define	   SensorHeatPowerOn()
#define	   SensorHeatPowerOff()
#endif
#define	SensorStart100HZ()

#define STANDARD_REF_VOLT	3000	   //ϵͳ�����׼�ο���ѹ
#define	DEFAULT_NOCOAL_VA	1580		   //�޾ƾ�
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
#define	PIO_ADJUST_DET	 	P15  //���  �ƾ�У׼50mg  �������߷��ͽ�
#define	PIO_CFG_DET	 		0x20    //1<<5
//p00
#define	PIO_ADJUST_DET20	 	P00  //20mg calibrate
#define	PIO_CFG_DET20	 		0x01    //1<<5
//������Ϊֱ�� ֣��
//#define  FANGBO_PWR    //��ά������ ����


#define	P_LCD_ON		0
#define	P_LCD_OFF		1

#define	PO_LCD_POWER(x)		P17=x		 //lcd��Դ
#define	PIO_CHARG			P20
#define	PCHARG_DET()		(P20==1)		  //5V����Դ���
#define	PCHARG_FULL()		(P05==1)		  //
#define	PWR_G1		P04
#define	PWR_G2		P01

#endif
