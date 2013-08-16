#include "general.h"

//
#ifdef LCD_EN
WORD code Num2BCD_Tbl[]=
{

	BH_NUM_0, //0
	BH_NUM_1, //1
	BH_NUM_2, //2
	BH_NUM_3, //3
	BH_NUM_4, //4
	BH_NUM_5, //5
	BH_NUM_6, //6
	BH_NUM_7, //7
	BH_NUM_8, //8
	BH_NUM_9, //9




	BH_NUM_BLK, //10
	BH_CHAR_U, //11
	BH_CHAR_DASH,//12
	BH_CHAR_o,
	BH_CHAR_n,
	BH_CHAR_F,
	BH_CHAR_E,
	BH_CHAR_L,
	
}	 ;
BYTE DisBuffer[4]  ;
BYTE LCD_buf[5];
BYTE PWM_buf[3];
char jiu_level=-1;
//设置某一位LCD seg
//combyte ：显示数据
//addr:显示地址 lcd
void SetSeg(U8 combyte,U8 addr,U8 onbit)
{
	if(onbit)LCD_buf[addr]|=combyte;
		else LCD_buf[addr]&=~combyte;

}

			   

#define	SET_STB()		P16=1	
#define	CLR_STB()		P16=0
#define	SET_CLK()		P12=1
#define	CLR_CLK()		P12=0
#define	SET_DIO()		P11=1
#define	CLR_DIO()		P11=0
#define	NOP()		_nop_()

#define	LCD_MODE	0x30
#define	LED_PWM	  0x45
#define	PWM0_ADDR	  0
#define	PWM2_ADDR	  2
#define	PWM3_ADDR	  3

void SendLCDByte(U8 senddata)
{
		U8 i;
		 NOP();NOP()	; NOP()	; NOP()	;   NOP();NOP()	; NOP()	; NOP()	;
		CLR_STB();
		NOP(); NOP()	;
		for(i=0;i<8;i++)
		{
		CLR_CLK(); NOP()	;NOP()	;NOP()	;NOP()	;
		if(senddata&0x01)SET_DIO();
		else CLR_DIO();
		senddata=senddata>>1;
		NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;	
		SET_CLK();	
		NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;NOP()	;	
	  }
	  
	  NOP();NOP()	; NOP()	; NOP()	;
		
	
	
}


void Init_LCD(void)
{
	U8 i;
	DisBuffer[0]=0;
	DisBuffer[1]=1;
	DisBuffer[2]=2;
	DisBuffer[3]=3;

	
	for(i=0;i<TBL_SIZE(LCD_buf);i++)LCD_buf[i]=0;
	SendLCDByte(LCD_MODE);
	SET_STB();
	
	
	
}
void DispBuf2LcdBuf(void)
{
	
	LCD_buf[0]=Num2BCD_Tbl[DisBuffer[0]];
	LCD_buf[1]=Num2BCD_Tbl[DisBuffer[1]];
	LCD_buf[2]=Num2BCD_Tbl[DisBuffer[2]];
	LCD_buf[3]=Num2BCD_Tbl[DisBuffer[3]];
	
}
//#define	LCDBkligh_REAL_CHANGE
void LCD_HWSend()
{
	

	SendLCDByte(LCD_MODE);
	SET_STB();
	SendLCDByte(0x44);
	SET_STB();

	SendLCDByte(0xc2);			  //s4	 s5
	SendLCDByte(LCD_buf[0]);
	SET_STB();
	
	SendLCDByte(0xc3);			  //  s6 s7
	SendLCDByte(LCD_buf[1]);
	SET_STB();
	
	SendLCDByte(0xc6);			 //s12 s13
	SendLCDByte(LCD_buf[2]);
	SET_STB();
	
	SendLCDByte(0xc7);			   //s14 s15
	SendLCDByte(LCD_buf[3]);
	SET_STB();
	
	SendLCDByte(0xce);			  //s28 s29
	SendLCDByte(LCD_buf[4]);
	SET_STB();
	
	
//	SendLCDByte(0x97);	
  SendLCDByte(0x97);
	SET_STB();
#ifdef	LCDBkligh_REAL_CHANGE
	if(F_STANDBY==0)
		{	
			#ifdef CYCLE_BACKLIGHT_ON_OFF
			if(LCD_Reallight==LCD_LIGHTON)LCD_Reallight=LCD_DARKNESS;
			else LCD_Reallight--;
			#else
			LCD_Reallight=gSYSinfo.LcdBlight;
			#endif
			SetPWM(LCD_Reallight);
		}else 
		{
			if(gLCDBlightTm==0)// 逐渐关灯
			{
			if(LCD_Reallight<LCD_DARKNESS)LCD_Reallight+=1;
			SetPWM(LCD_Reallight); 
			}

		}
#endif	
}

#define	FLASH_TM_DURA		10
void ClrDisplayBuf()
{
					DisBuffer[0]=CHAR_BLK;
					DisBuffer[1]=CHAR_BLK;
					DisBuffer[2]=CHAR_BLK;
					DisBuffer[3]=CHAR_BLK;
					DisBuffer[4]=CHAR_BLK;
					LCD_buf[4]=0x00;
}


void Display_jiubei(char jiu_level)
{
	LCD_buf[SEG_JB0_ADDR ]&= ~SEG_JB0;
	LCD_buf[SEG_JB1_ADDR ]&= SEG_JB1;
	LCD_buf[SEG_JB2_ADDR ]&= SEG_JB2;
	LCD_buf[SEG_JB3_ADDR ]&= SEG_JB3;

	if(jiu_level>=0)LCD_buf[SEG_JB0_ADDR ]|= SEG_JB0;
	if(jiu_level>=1)LCD_buf[SEG_JB1_ADDR ]|= SEG_JB1;
	if(jiu_level>=2)LCD_buf[SEG_JB2_ADDR ]|= SEG_JB2;
	if(jiu_level>=3)LCD_buf[SEG_JB3_ADDR ]|= SEG_JB3;


}



#endif

void 	Display_All()
{
#if 0	
	 DispBuf2LcdBuf();
	 Display_jiubei(jiu_level) ;
	 LCD_HWSend();
#endif
}