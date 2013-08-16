#include "general.h"
extern _JIUJING_LST Jiulist;
extern BYTE Jdispidx;
BYTE DisplayCont = 0;
extern BYTE BatCnt;
extern BYTE BZY_on;
//
WORD code Num2BCD_Tbl[] = {

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
		BH_CHAR_A, //a
		BH_CHAR_b, //11
		BH_CHAR_c, //12
		BH_CHAR_d, //13
		BH_CHAR_E,  //14
		BH_CHAR_F, //15

		BH_NUM_BLK, //16
		BH_CHAR_U, //17
		BH_CHAR_DASH, //18
		BH_CHAR_o,	//   19
		BH_CHAR_n, //20
		BH_CHAR_L,	//21
		BH_CHAR_r,	//22

		};
BYTE DisBuffer[4];
BYTE LCD_buf[5];
BYTE PWM_buf[3];
WORD Currrentjiu_level = 0;	 //mg
//设置某一位LCD seg
//combyte ：显示数据
//addr:显示地址 lcd
void SetSeg(U8 combyte, U8 addr, U8 onbit) {
	if (onbit)
		LCD_buf[addr] |= combyte;
	else
		LCD_buf[addr] &= ~combyte;

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

void SendLCDByte(U8 senddata) {
	U8 i;
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	CLR_STB();
	NOP();
	NOP();
	for (i = 0; i < 8; i++) {
		CLR_CLK();
		NOP();
		NOP();
		NOP();
		NOP();
		if (senddata & 0x01)
			SET_DIO();
		else
			CLR_DIO();
		senddata = senddata >> 1;
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		SET_CLK();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}

	NOP();
	NOP();
	NOP();
	NOP();

}

//明亮程度
void SetPWM(BYTE portn, U8 PwmN) {
	if (portn == 0)
		PWM_buf[0] = PwmN;
	else if (portn == 1)
		PWM_buf[1] = PwmN;
	else if (portn == 2)
		PWM_buf[2] = PwmN;

	SendLCDByte(LED_PWM);
	SET_STB();

	SendLCDByte(0xc0);
	SendLCDByte(PWM_buf[0]);
	SET_STB();

	SendLCDByte(0xc2);
	SendLCDByte(PWM_buf[1]);
	SET_STB();

	SendLCDByte(0xc3);
	SendLCDByte(PWM_buf[2]);
	SET_STB();

}
void Init_LCD(void) {
	U8 i;
	PO_LCD_POWER(P_LCD_ON);
	DelayXms(20);
	DisBuffer[0] = CHAR_DASH;
	DisBuffer[1] = CHAR_DASH;
	DisBuffer[2] = CHAR_DASH;
	DisBuffer[3] = CHAR_DASH;

	for (i = 0; i < TBL_SIZE(LCD_buf); i++)
		LCD_buf[i] = 0;
	SendLCDByte(LCD_MODE);
	SET_STB();
	BUZZY_OFF()
	;

}
void DispBuf2LcdBuf(void) {

	LCD_buf[0] = Num2BCD_Tbl[DisBuffer[0]];
	LCD_buf[1] = Num2BCD_Tbl[DisBuffer[1]];
	LCD_buf[2] = Num2BCD_Tbl[DisBuffer[2]];
	LCD_buf[3] = Num2BCD_Tbl[DisBuffer[3]];

}
//#define	LCDBkligh_REAL_CHANGE
void LCD_HWSend() {

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
	} else
	{
		if(gLCDBlightTm==0)			  // 逐渐关灯
		{
			if(LCD_Reallight<LCD_DARKNESS)LCD_Reallight+=1;
			SetPWM(LCD_Reallight);
		}

	}
#endif	
}

#define	FLASH_TM_DURA		10
void ClrDisplayBuf() {
	DisBuffer[0] = CHAR_BLK;
	DisBuffer[1] = CHAR_BLK;
	DisBuffer[2] = CHAR_BLK;
	DisBuffer[3] = CHAR_BLK;
	DisBuffer[4] = CHAR_BLK;
	LCD_buf[4] = 0x00;
}

void Display_jiubei(WORD mg100) {
	BYTE jiu_level;
	LCD_buf[SEG_JB0_ADDR] &= ~SEG_JB0;
	LCD_buf[SEG_JB1_ADDR] &= ~SEG_JB1;
	LCD_buf[SEG_JB2_ADDR] &= ~SEG_JB2;
	LCD_buf[SEG_JB3_ADDR] &= ~SEG_JB3;
	if (DisplayCont == DISPLAY_ONOFF || DisplayCont == DISPLAY_ALO) {
		jiu_level = CalALCLevel(mg100);
		/*  20110711
		 if(jiu_level==0)
		 {

		 DisBuffer[0]= CHAR_BLK;
		 DisBuffer[1]= CHAR_BLK;
		 DisBuffer[2]= CHAR_o;
		 DisBuffer[3]= CHAR_n;

		 }
		 else  if(jiu_level!=-1)
		 {


		 }
		 */
		if (jiu_level >= 0)
			LCD_buf[SEG_JB0_ADDR] |= SEG_JB0;
		if (jiu_level >= 1)
			LCD_buf[SEG_JB1_ADDR] |= SEG_JB1;
		if (jiu_level >= 2)
			LCD_buf[SEG_JB2_ADDR] |= SEG_JB2;
		if (jiu_level >= 3)
			LCD_buf[SEG_JB3_ADDR] |= SEG_JB3;
	}

}

//显示电量，充电
void DispBAT()

{
	LCD_buf[SEG_BAT1_ADDR] &= ~SEG_BAT1;
	LCD_buf[SEG_BAT2_ADDR] &= ~SEG_BAT2;
	LCD_buf[SEG_BAT3_ADDR] &= ~SEG_BAT3;
	LCD_buf[SEG_BAT0_ADDR] |= SEG_BAT0;		//外框
#if 1	
	//	if(PCHARG_DET())  //2012.0402 硬件问题，
	if (!PCHARG_FULL())	   //低电平充电
	{	//充电

		LCD_buf[SEG_BAT1_ADDR] |= SEG_BAT1;
		LCD_buf[SEG_BAT2_ADDR] |= SEG_BAT2;
		LCD_buf[SEG_BAT2_ADDR] |= SEG_BAT3;
		//	if(!PCHARG_FULL())
		{
			if (F_half_sec) {
				LCD_buf[SEG_BAT2_ADDR] &= ~SEG_BAT3;
			}

		};
	} else {
		//没充电

		if (BatCnt >= 1)
			LCD_buf[SEG_BAT1_ADDR] |= SEG_BAT1;
		if (BatCnt >= 2)
			LCD_buf[SEG_BAT2_ADDR] |= SEG_BAT2;
		if (BatCnt >= 3)
			LCD_buf[SEG_BAT2_ADDR] |= SEG_BAT3;

		//电量低闪烁 外框
		if (BatCnt == 0 && F_half_sec) {
			LCD_buf[SEG_BAT0_ADDR] &= ~SEG_BAT0;

		}
	}

#endif

#ifdef	EXTERNAL_POWER_NOSLEEP
	if(PCHARG_DET())
	{	//充电

		LCD_buf[SEG_BAT1_ADDR ]|= SEG_BAT1;
		LCD_buf[SEG_BAT2_ADDR ]|= SEG_BAT2;
		LCD_buf[SEG_BAT2_ADDR ]|= SEG_BAT3;
		if(!PCHARG_FULL())
		{
			if(F_half_sec)
			{
				LCD_buf[SEG_BAT2_ADDR ]&= ~SEG_BAT3;
			}

		};
	} else
	{
		//没充电

		if(BatCnt>=1)LCD_buf[SEG_BAT1_ADDR ]|= SEG_BAT1;
		if(BatCnt>=2)LCD_buf[SEG_BAT2_ADDR ]|= SEG_BAT2;
		if(BatCnt>=3)LCD_buf[SEG_BAT2_ADDR ]|= SEG_BAT3;

		//电量低闪烁 外框
		if(BatCnt==0&&F_half_sec)
		{
			LCD_buf[SEG_BAT0_ADDR ]&= ~SEG_BAT0;

		}
	}

#endif

}
#ifdef	DISPLAY_SYSINFO
extern _SYSINFO SysInfo;
void DisplaySYSRAN() {

	DisBuffer[0] = (SysInfo.TRANCODE >> 12) & 0x0f;
	DisBuffer[1] = (SysInfo.TRANCODE >> 8) & 0x0f;
	DisBuffer[2] = (SysInfo.TRANCODE >> 4) & 0x0f;
	DisBuffer[3] = SysInfo.TRANCODE & 0x0f;

}

//更新显示缓冲区 ，实际没显示，可能被其他数据覆盖
void DIsplaySYSV(WORD hexnum) {
	WORD tmp = hexnum;
	// tmp=SysInfo.VB-SysInfo.VC;
	tmp = (WORD) ((DWORD) tmp * (1000 * 3) / 1024);  //显示mV
	HexToBCD((WORD *) &tmp, (BYTE *) &DisBuffer);
}
#endif

void Display_All() {
	WORD tempd;
	extern BYTE CountHeat;
	switch (DisplayCont) {
	case DISPLAY_TIME_SEC:
		tempd = CountHeat;
		HexToBCD((WORD *) &tempd, (BYTE *) &DisBuffer);
		DisBuffer[0] = DisBuffer[1] = CHAR_BLK;
		break;
	case DISPLAY_DEMO:
	case DISPLAY_ERR0:
	case DISPLAY_ERR1:
	case DISPLAY_ERR2:
		//	DisBuffer[0]= CHAR_BLK;
		DisBuffer[0] = CHAR_E;
		DisBuffer[1] = CHAR_r;
		DisBuffer[2] = CHAR_r;
		DisBuffer[3] = DisplayCont & 0x0f;
		break;

	case DISPLAY_ALO: {
		WORD tmp;
		BYTE tempidx;

		if (Jdispidx >= MAX_JIULST)
			Jdispidx = 0;

		if (F_half_sec) {

			tmp = Jiulist.Jvol[Jdispidx];

			if (tmp == 0xffff) {
				DisBuffer[0] = CHAR_BLK;
				DisBuffer[1] = CHAR_DASH;
				DisBuffer[2] = CHAR_DASH;
				DisBuffer[3] = CHAR_DASH;

			} else {
				if (tmp > 200)
					tmp = 200; //mg/100ml
				{
					extern BYTE CalALCLevel(WORD tmp); //显示level

					CalALCLevel(tmp); //显示level
					Currrentjiu_level = tmp;	//07.20 历史数据显示酒杯
				}
				HexToBCD((WORD *) &tmp, (BYTE *) &DisBuffer);

			}

		} else {
			tempidx = (Jdispidx + 1);
			tmp = tempidx;
			HexToBCD((WORD *) &tmp, (BYTE *) &DisBuffer);
		}

	}
		break;
	case DISPLAY_TSTING:
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_DASH;
		DisBuffer[2] = CHAR_DASH;
		DisBuffer[3] = CHAR_DASH;
		break;
	case DISPLAY_ADJ:
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_A;
		DisBuffer[2] = 5;
		DisBuffer[3] = 0;
		break;
	case DISPLAY_ADJ20:
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_A;
		DisBuffer[2] = 2;
		DisBuffer[3] = 0;
		break;
	case DISPLAY_NCAL:
		DisBuffer[0] = CHAR_n;
		DisBuffer[1] = CHAR_c;
		DisBuffer[2] = CHAR_A;
		DisBuffer[3] = CHAR_L;
		break;

	case DISPLAY_SLEEP:
		break;
	case DISPLAY_MATSEND:
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_BLK;
		DisBuffer[2] = BH_CHAR_U;
		break;
	case DISPLAY_SYS_RAN:
		DisplaySYSRAN();
		break;
	case DISPLAY_SYS_VS:
		DIsplaySYSV(SysInfo.VB - SysInfo.VC);
		break;

	case DISPLAY_SYS_VB:
		DIsplaySYSV(SysInfo.VB);

		break;
	case DISPLAY_SYS_VC:
		DIsplaySYSV(SysInfo.VC);

		break;
	case DISPLAY_EONOFF:
	case DISPLAY_ONOFF:
		break;
	case DISPLAY_NUM:
		break;
	default:
		break;
	}
	if (DisplayCont >= DISPLAY_SYS_RAN && DisplayCont <= DISPLAY_SYS_VC) {
		if (F_half_sec) {
			WORD tmp = DisplayCont;
			HexToBCD((WORD *) &tmp, (BYTE *) &DisBuffer);
		}
	}
	DispBuf2LcdBuf();
	if (DISPLAY_EONOFF != DisplayCont)
		Display_jiubei(Currrentjiu_level);
	if (DISPLAY_ALO == DisplayCont) {

		if (F_half_sec == 0) {
			//显示位置
			LCD_buf[0] = LCD_buf[1] = 0;
			LCD_buf[SEG_PPM_ADDR] &= ~SEG_PPM;
		}

		else
			LCD_buf[SEG_PPM_ADDR] |= SEG_PPM;
	}

	DispBAT();
	LCD_HWSend();
	LCDBAKLIGHT(LCDBAK_ON);

}

void LCD_Sleep() {
	SendLCDByte(0x80);
	SET_STB();
	//模式漏电
	// 	SET_CLK();
//	SET_DIO()	;

	//置低防漏电
	CLR_STB();
	CLR_CLK();
	CLR_DIO();

}
