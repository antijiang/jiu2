#ifndef	_LCD_DRV_H_
#define	_LCD_DRV_H
#define	SEG_A		0x08
#define	SEG_B		0x04
#define	SEG_C		0x02
#define	SEG_D		0x01
#define	SEG_E		0x80
#define	SEG_F		0x40
#define	SEG_G		0x20


#define	SEG_COL			0x10
#define	SEG_COL_ADDR	0x00
#define	SEG_PPM				0x10
#define	SEG_PPM_ADDR		0x01

#define	SEG_BAT0			0x10	//µç³Ø¿ò
#define	SEG_BAT0_ADDR		0x03
#define	SEG_BAT1			0x40
#define	SEG_BAT1_ADDR		0x04
#define	SEG_BAT2			0x20
#define	SEG_BAT2_ADDR		0x04
#define	SEG_BAT3			0x10
#define	SEG_BAT3_ADDR		0x04

#define	SEG_JB0				0x0c	 //±»ÍâºÐ
#define	SEG_JB0_ADDR		0x04
#define	SEG_JB1				0x02	 //
#define	SEG_JB1_ADDR		0x04
#define	SEG_JB2				0x01	 //
#define	SEG_JB2_ADDR		0x04
#define	SEG_JB3				0x80	 
#define	SEG_JB3_ADDR		0x04

#define	SEG_ALARM1				0x01
#define	SEG_ALARM1_ADDR		0x04
#define	SEG_ALARM2				0x02
#define	SEG_ALARM2_ADDR		0x04


#define	BH_NUM_0	(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)		
#define	BH_NUM_1	(SEG_B|SEG_C)		
#define	BH_NUM_2	(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G)		
#define	BH_NUM_3	(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G)
#define	BH_NUM_4	(SEG_B|SEG_C|SEG_G|SEG_F)		
#define	BH_NUM_5	(SEG_A|SEG_C|SEG_D|SEG_G|SEG_F)		
#define	BH_NUM_6	(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)		
#define	BH_NUM_7	(SEG_A|SEG_B|SEG_C)		
#define	BH_NUM_8	(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)		
#define	BH_NUM_9	(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G)
#define	BH_NUM_BLK	(0)	
#define	BH_CHAR_U	(SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)	
#define	BH_CHAR_DASH	(SEG_G)	
#define	BH_CHAR_H	(SEG_B|SEG_C|SEG_D|SEG_F|SEG_G)
#define	BH_CHAR_o	(SEG_C|SEG_D|SEG_E|SEG_G)
#define	BH_CHAR_n	 (SEG_C|SEG_E|SEG_G)
#define	BH_CHAR_L	 (SEG_D|SEG_E|SEG_F)


#define	BH_CHAR_A	(SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G)
#define	BH_CHAR_b	(SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define	BH_CHAR_c	(SEG_A|SEG_D|SEG_E|SEG_F)

#define	BH_CHAR_d	(SEG_B|SEG_C|SEG_D|SEG_E|SEG_G)
#define	BH_CHAR_E	(SEG_A|SEG_D|SEG_E|SEG_F|SEG_G)
#define	BH_CHAR_F	(SEG_A|SEG_E|SEG_F|SEG_D)



#define	CHAR_BLK	10
#define	CHAR_U		11
#define	CHAR_DASH	12
#define	CHAR_o	13
#define	CHAR_n	14
#define	CHAR_F	15
#define	CHAR_E	16
#define	CHAR_L	17
typedef  enum	{  
	DISPLAY_MIN,
	DISPLAY_TIME=DISPLAY_MIN,
	DISPLAY_TIME_SEC,
	DISPLAY_TIME_SET,
	DISPLAY_TIME_SET_ALARM,
	DISPLAY_TIME_SET_ALARM_EN,
	DISPLAY_MAX,
	DISPLAY_FREQUENCY,
	DISPLAY_STANDBY,
	DISPLAY_MUTE ,
	DISPLAY_AUTOSEARCH,
	DISPLAY_SOURCE,
	DISPLAY_SLEEP,
	DISPLAY_VOL, 
	DISPLAY_TIME_SET_HOUR,
	DISPLAY_TIME_SET_MIN,
	DISPLAY_TIME_SET_ALARM_HOUR,
	DISPLAY_TIME_SET_ALARM_MIN,
 
} DISPLAY_ENUM;


	
void SetPWM(U8 PwmN);
#define	PWMOUT_HI			0x3f
#define	PWMOUT_LOW			0x0
#define	LCD_DARKNESS			PWMOUT_HI
#define	LCD_LIGHTON			PWMOUT_LOW
void 	Display_All();
void Init_LCD(void);
void Freq_2_Buffer(unsigned long Freq,unsigned char * disbuffer);

#endif
