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
#define SECURE_SPI  //安装加密IC

#define	USE_CPU_TEMPERATURE  //定义使用内部温度检测
#define	TEST_IR
#define	EN_UART1		DEF_ENABLE


#define	EN_AUTO_MESURE_LIGHT_SENS  	DEF_ENABLE//允许自动测量光强
#define	EN_AUTO_DETECT_MOVEMENT		DEF_ENABLE    	//允许自动检测
#define	DEFAULT_AUDETECT_MODE		DEF_DISABLE//上电自动模式
#define	AUTO_OFF_TIME_CNTMS			(1/0.5)  //(加上PIR 模块已延时10 s)   30s  无人自动熄灭时间 n 秒


#define	   SensorPowerOn()	P06=1	
#define	   SensorPowerOff()	P06=0
#define	SensorStart100HZ() 

#define MAX_ALCOHOLICITY	  (1000/2.2*2.5	)

#define	NORMAL_SESOR_POWER_VAL	 (1024/2.44*1.5)
#define	LED_ON		0
#define	LED_OFF		1
#define LEDIO			P11 
#define	LED_RED(x)		P11=x	
#define	LED_GREEN(x)	P04=x	

#define	PWRON	1
#define	PWROFF	0
#define	YUINIO1		P12
#define	YUINIO2		P14
#define	YUINIO3		P15

#define	POWER_ON()	{P13=1;relay_state=1;LED_RED(LED_ON);}
#define	POWER_OFF()	{P13=0 ;relay_state=0;LED_RED(LED_OFF);}
#define	PWR_G1		P01
#define	PWR_G2		P06
#define	PIORFIN		P16

//我的平台
//#define	PWRSW_DETECT	P17
#define	PWRSW_DETECT	P07
#define	YUYIN_DETECT()	 {YUINIO1=0;YUINIO2=1;YUINIO3=0;}
#define	YUYIN_CHAOBIAO() {YUINIO1=1;YUINIO2=0;YUINIO3=0;}
#define	YUYIN_KAICHE()	 {YUINIO1=0;YUINIO2=0;YUINIO3=1;}
#define	YUYIN_CHAOSHI()	 {YUINIO1=0;YUINIO2=1;YUINIO3=1;}
#define	YUYIN_NOYUYIN()	 {YUINIO1=1;YUINIO2=1;YUINIO3=1;}

#define	YUYINT_DETECT  0
#define	YUYINT_CHAOBIAO 1
#define	YUYINT_KAICHE 	 2
#define	YUYINT_CHAOSHI 	 3
#define	YUYINT_NOYUYIN 	 4


#define		FUN_MATCH_CODE	0x0a
#define		FUN_LOCK_CAR	0x02
#define		FUN_UNLOCK_CAR	0x03

#define		PO_REC_CTRL   P20	
#ifdef	IO_HIGH_LEVEL_ON
#define	REC_START()	 {PO_REC_CTRL=0;}
#define	REC_STOP()	 {PO_REC_CTRL=1;}
#else
//低电平开
#define	REC_START()	 {PO_REC_CTRL=1;}
#define	REC_STOP()	 {PO_REC_CTRL=0;}
#endif


#endif
