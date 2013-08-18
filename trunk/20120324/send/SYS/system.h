#ifndef	_SYSTEM_H_
#define	_SYSTEM_H_

#ifdef	_SYSTEM_C_
#define	VARSYS_TYPE_INI(a,init_val)	a =init_val
#define	VARSYS_TYPE(a)	a
#else
#define	VARSYS_TYPE_INI(a,init_val)	extern a
#define	VARSYS_TYPE(a)	extern a
#endif
#define	BOARD_ID_VER	(0xfc)

#define SYSCLK       48000000          // SYSCLK frequency in Hz
#define BAUDRATE0     115200           // Baud rate of UART0 in bps
#define BAUDRATE1     115200  //250000           // Baud rate of UART1 in bps

#define	TIMER0_CLK_IN	 (SYSCLK/48)
#define	TIMER1_CLK_IN	 (SYSCLK/48)
#define	TIMER0_10MS_CNT		(TIMER0_CLK_IN*10/1000)
#define	TIMER0_1MS_CNT		(TIMER0_CLK_IN*10/1000)
#define		TM0_RELOAD		7
//#define	TIMER0_100MS_CNT		((100/1000)* TIMER0_CLK_IN)

#define	CNT_POWER_ON_TO_SET	50
VARSYS_TYPE_INI(BOOL F_1ms, 0);
VARSYS_TYPE_INI(BOOL F_50ms, 0);
VARSYS_TYPE_INI(BOOL F_5ms, 0);

//VARSYS_TYPE_INI(BOOL F_10ms,0);
VARSYS_TYPE_INI(BOOL F_100ms, 0);
VARSYS_TYPE_INI(BOOL F_500ms, 0);
VARSYS_TYPE_INI(BOOL F_FLASHLAMP, 0);
VARSYS_TYPE_INI(BYTE  Flash_TM, 0);

VARSYS_TYPE_INI(BOOL F_200ms, 0);
VARSYS_TYPE_INI(BOOL F_5S, 0);
VARSYS_TYPE_INI(BOOL F_1000ms, 0);
VARSYS_TYPE_INI(BOOL F_half_sec, 0);
VARSYS_TYPE_INI(BOOL F_1MIN, 0);
VARSYS_TYPE_INI(BYTE Count_5S, 0);
VARSYS_TYPE (idata   data_time sys_time) ;

VARSYS_TYPE_INI(BOOL F_STANDBY, 0);

VARSYS_TYPE_INI(BYTE  sys_time10ms_CNT, 0);


VARSYS_TYPE_INI(xdata BYTE  Display_content, 0);


VARSYS_TYPE_INI( BYTE F_FlashSave, 0); //Óû´æ´¢Êý¾Ý

VARSYS_TYPE_INI(BOOL F_ADInit, 1);
VARSYS_TYPE_INI(BOOL F_ADOK, 0);
VARSYS_TYPE( WORD xdata Current_temperatureV);
VARSYS_TYPE( BYTE xdata g_CurrentADport);


VARSYS_TYPE(void SwitchADport(BYTE adport));
//void Pause_Measure(void)	;
//void Restart_Measure(void);

VARSYS_TYPE(WORD Force_MesureADport(BYTE portm ));
#define	DEBUG_BUF
#define	MESURE_TIMES	20
#define	MEASUREMENT_PRE_COUNT	-5
#define	AVR_2TH_TIME		10





VARSYS_TYPE(void Init_Device(void));
VARSYS_TYPE(void Init_patch(void));
VARSYS_TYPE(void DelayXms(WORD t));
VARSYS_TYPE(void DelayXus(WORD t));
VARSYS_TYPE(void Init_VAR(void));
BYTE   get_AD_result(BYTE ch);
VARSYS_TYPE (void HexToBCD(WORD  *ptran_data, BYTE  *BCD_buf));

#endif
