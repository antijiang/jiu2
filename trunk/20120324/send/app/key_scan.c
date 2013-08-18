#include "spicom.h"
#include "general.h"

#include <stdlib.h>
#define		FUN_MATCH_CODE	0x0a
#define		FUN_LOCK_CAR	0x02
#define		FUN_UNLOCK_CAR	0x03
#define		FUN_DETECT_CAR	0x04
#define		FUN_CHAOSHI_CAR	0x05
#include <math.h>

//#define		DEBUG_VOL
enum
{
	TEST_INIT = 0,
	TEST_A,
	TEST_REP,
	TEST_100_PREV,
	TEST_100HZ_L3,
	TEST_100HZ_H3,
	TEST_WAIT_BLOW_L,
	TEST_WAIT_BLOW_H,
	TEST_MEASURE_H,
	TEST_MEASURE_L,
	AL_STATE_CAL = 0x10, //����
	AL_STATE_EXIT = 0x20, //�˳�
	AL_STATE_SLEEP_CNT = 0x50,

};

//#define	AL_STATE_SLEEP_CNT	0x50		   //׼������ idleģʽ
extern BYTE NEW_KEY;
extern _SYSINFO SysInfo;
extern _JIUJING_LST Jiulist;
extern BYTE DisBuffer[5];
BYTE Jdispidx = 0;
char qishu = 0; //������
extern BYTE BZY_on;
BYTE TransCmd = FUN_LOCK_CAR;
#ifdef  FANGBO_PWR
#define	MIN_QISHU	8	   //��⵽�����ٴ�����
#else
#define	MIN_QISHU	((500+500)/100)	   //   ʱ�� / (100ms�� ��һ��  ��⵽�����ٴ�����
#endif

BYTE CountHeat = 0;
BYTE StateSensor = AL_STATE_SLEEP_CNT;
BYTE HeatRetryTime = 0; //�궨ʱ���ȵ�2.4V �Ĵ���
extern WORD Currrentjiu_level;
extern BYTE DisplayCont;
BYTE AdjustSensor = 0;
BYTE Insystate = 0; //��Ϣ��ʾ״̬ ��־ 1����Ϣ״̬
extern void DIsplaySYSV(WORD hexnum);
void Init_sensor()
{
	StateSensor = AL_STATE_EXIT;
	SensorHeatPowerOff();
	SensorPowerOff();
}

BYTE PI_ADJUST_DET()
{
	BYTE reval;
	PIO_ADJUST_DET = 1;
	P1MDOUT &= ~PIO_CFG_DET; //p15 ����·���
	DelayXms(10);
	if (PIO_ADJUST_DET == 0)
		reval = 1;
	else
		reval = 0;
	return reval;

}
//P0.0
BYTE PI_ADJUST_DET20()
{
	BYTE reval;
	//  ��������
	P0SKIP &= ~PIO_CFG_DET20;
	P0MDIN |= PIO_CFG_DET20;
	//����
	REF0CN &= ~0x01; //�����壬����ܽ��вο���ѹ2.5V�������ʹ����Ϊ���ֶ˿�
	P0MDOUT |= PIO_CFG_DET20; //p00 ǿ��
	PIO_ADJUST_DET20 = 1;

	DelayXms(50);
	if (PIO_ADJUST_DET20 == 0)
		reval = 1;
	else
		reval = 0;
	//ģ�������׼��ѹ��
	P0SKIP |= PIO_CFG_DET20;
	P0MDIN &= ~PIO_CFG_DET20;
	P0MDOUT &= ~PIO_CFG_DET20; //p00 ����·���
	//REF0CN    |= 0x01;
	PIO_ADJUST_DET20 = 0;
	DelayXms(50);
	return reval;

}

//#define	ONLY_RELEASE_PRESS
/*********************************************************
 ����
 �ظ�������������
 100ms call once
 **********************************************************/
//#define	ONLY_RELEASE_PRESS
BYTE Read_Key(void)
{
	static BYTE TM_KeyHold = 0;
	static BYTE old_key;
	BYTE temp_key = KEY_NOKEY;

	//���³���100msִ��һ��
	PWR_G2 = 1;
	PWR_G1 = 1;
	_nop_();
	_nop_();
	_nop_();
	if (PWR_G1 == 0)
	{
		temp_key = KEY_JIUJING;
	}
	if (PWR_G2 == 0)
	{
		temp_key = KEY_DISPLAY;
	}
	if (PWR_G1 == 0 && PWR_G2 == 0)
	{
		temp_key = KEY_MODE;
	}

	//RELEASE key
	if (temp_key == KEY_NOKEY)
	{
#ifdef ONLY_RELEASE_PRESS

		if(TM_KeyHold == 0)
		{
			old_key = KEY_NOKEY;
		}
		else
		{

			temp_key = old_key;
			old_key = KEY_NOKEY;

		}
#else

		old_key = KEY_NOKEY;
		TM_KeyHold = 0;

#endif
	}

	//press and hold
	else if (old_key == temp_key)
	{
		//PRESS LONG TIME KEY  if press TM_KeyHold time ,process continue key,  else no key
#ifdef ONLY_RELEASE_PRESS
		if(TM_KeyHold != 0)temp_key = KEY_NOKEY;
		else
		{
			if((old_key == KEY_RIGHT) || (old_key == KEY_LEFT)

			)
			{

				temp_key = old_key;
				if(PRE_NEED_3S == 0)
				{
					temp_key = old_key;
					PRE_NEED_3S = 5; //  INC or DEC interval /5*20MS
				}
				else temp_key = KEY_NOKEY;

			}
			else temp_key = KEY_NOKEY;

		}
#else
#define	KEY_HOLD_TIME	10
#if 0
		if( old_key == KEY_MODE)
		{
#define	CONKEY_AFTER_FIRST_TIME	7 //���� 700ms���ظ�
			if(++TM_KeyHold >= CONKEY_AFTER_FIRST_TIME)
			{
				//Continuous key
				TM_KeyHold = CONKEY_AFTER_FIRST_TIME - 1;

			}
			else temp_key = KEY_NOKEY;

		}

		else

#endif

		//����2sΪ����
		if (old_key == KEY_MODE)
		{

			temp_key = KEY_NOKEY;
			TM_KeyHold++;
			//							   ���� ��ת��Ϊ��������
			if (TM_KeyHold == KEY_HOLD_TIME)
			{
				//Ϊ��ʾ�궨ֵ
				temp_key = KEY_DISPSTD;

			}
			else if (TM_KeyHold == KEY_HOLD_TIME * 5)
			{
				//Ϊ��������ʵ�ֵĹ��ܡ�����
				temp_key = KEY_RANCODE;

			}

			else if (TM_KeyHold > KEY_HOLD_TIME * 6)
			{
				TM_KeyHold = KEY_HOLD_TIME * 6 + 1;

			}

		}

		//����3s��ʾ�������� ������
		else if (old_key == KEY_DISPLAY)
		{

			temp_key = KEY_NOKEY;
			TM_KeyHold++;
			// ���� ��ת��Ϊ��������
			if (TM_KeyHold == KEY_HOLD_TIME * 2)
			{

				//Ϊ��������ʵ�ֵĹ��ܡ�����
				temp_key = KEY_EON;

			}
			else if (TM_KeyHold == KEY_HOLD_TIME * 4)
			{
				//Ϊ��ʾ�궨ֵ
				temp_key = KEY_DISPSTD;

			}

			else if (TM_KeyHold > KEY_HOLD_TIME * 4)
			{
				TM_KeyHold = KEY_HOLD_TIME * 4 + 1;

			}

		}

		else
			temp_key = KEY_NOKEY;

#endif
	}
	//first press �ͷź���
	else
	{
#ifdef ONLY_RELEASE_PRESS

		old_key = temp_key;
		temp_key = KEY_NOKEY;
		TM_KeyHold = COUNT_500_MSEC;
#else

		old_key = temp_key;
		TM_KeyHold = 0;

#endif

	}

	return temp_key;

}

//***************************************************************
//*			process the IR  key and the scan key  				*
//*		��ͨɨ��						*
//***************************************************************
BYTE ScanKey(void)
{
	BYTE scan_key;
	scan_key = Read_Key();
	//	if(StateSensor<=AL_STATE_EXIT) return KEY_NOKEY;
	return scan_key;

}

#define StepUpSound()
#define StepDownSound()
extern void Oscillator_LowCost();
extern void Oscillator_HiCost();
void GoSleep()
{
	BYTE IEx, IEy;
	//���������жϣ�

	Insystate = 0;

	SensorHeatPowerOff();
	SensorPowerOff();
#define	DALIOUT				P15
	DALIOUT = 0; //�ط���
	PIO_CHARG = 0; //�ؼ����
	PO_LCD_POWER(P_LCD_OFF);
	LCDBAKLIGHT(LCDBAK_OFF);
	{
		extern void LCD_Sleep(void);

		LCD_Sleep();

	}LED_GREEN(LED_OFF);LED_RED(LED_OFF);
	EA = 0; //2012.04
	IEx = IE;
	IEy = EIE1;

	//EA=0;
	IE = 0x05; //����IT0  IT1
	EIE1 = 0;
	TCON |= 0x05; //���ش���
	TCON &= ~0x0A; //���жϱ�־
	IT01CF = 0x41; //�͵�ƽ	p01,p04
	AD0EN = 0; //ad off
	REF0CN = 0x80; //�ر�ƫѹ���¶ȴ�
	Oscillator_LowCost();
	EA = 1; //���ж�
	PCON |= 0x01; //    �� IDLE λ�� 1
	PCON = PCON;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	//
	EA = 0; //���ж�
	Oscillator_HiCost();

	//���´� VREF
	{
		extern
		Voltage_Reference_Init();
		Voltage_Reference_Init();
	}
	AD0EN = 1;
	EIE1 = IEy;
	IE = IEx;

	PO_LCD_POWER(P_LCD_ON);
	PIO_CHARG = 1; //�����Ϊ�����
	EA = 1;
	//2012 �����ʾ����ʧ��p17û��ֵ ����
	RSTSRC |= 0x10;

}

//WORD  VS,VP;
WORD VA, VB, VC; //va :������͵�ѹ VB������ǰ��ѹ  VC���������ѹ

//���� 0����ص�ѹ��
//
//4.0 3.9 3.8 3.7V  3.6
// ������(3V)������      1024/3    *  56K/(56K+56K)  >>2  (תΪ�ֽ�
//IO 2.5v
#define	BATVREF	 ((BYTE) ((DWORD)1024/(STANDARD_REF_VOLT/1000)*56/(56+56) /4) )
#define	BATV1	(BYTE)((WORD)BATVREF*40/10)
#define	BATV2	(BYTE)((WORD)BATVREF*39/10)
#define	BATV3	(BYTE)((WORD)BATVREF*38/10)
#define	BATV4	(BYTE)((WORD)BATVREF*37/10)
#define	BATV5	(BYTE)((WORD)BATVREF*36/10)

BYTE BatCnt = 0; //��������
void BatteryEnegyDetect()
{
	WORD tempv;
	BYTE batv;
	static BYTE LowCnt = 0;
	tempv = Force_MesureADport(ADPORT_BAT);
	batv = (tempv >> 2) & 0xff;
	if (batv >= BATV1)
	{
		BatCnt = 4;
	} //������
	else if (batv >= BATV2)
	{
		BatCnt = 3;
	}
	else if (batv >= BATV3)
	{
		BatCnt = 2;
	}
	else if (batv >= BATV4)
	{
		BatCnt = 1;
	}
	else
		BatCnt = 0; //�޵���˸���
#if  1 //   1def	EXTERNAL_POWER_NOSLEEP
	if (PCHARG_DET())
		LowCnt = 0;
#endif
	if (batv < BATV5)
	{
		if (LowCnt++ > 30) //
		{
			StateSensor = AL_STATE_SLEEP_CNT;
			DisplayCont = DISPLAY_TSTING;
			GoSleep();
			Display_All();
			DelayXms(100);
		}

	}
	else
		LowCnt = 0;
	return;
}

#if 0
//���� 0����ص�ѹ��
BOOL SensorPowerDetect()
{
	BYTE i;
	WORD tempv;
	tempv = Force_MesureADport(ADPORT_POWER);
	//��������Դ��ѹ�ͱ���
	if(tempv < NORMAL_SESOR_POWER_VAL) //1.5v

	{
		DisBuffer[0] = BH_CHAR_L;
		DisBuffer[0] = CHAR_DASH;

		Display_All();
		//����̵�һ��������������
		for(i = 0; i < 3; i++)
		{
			LED_RED(1);
			LED_GREEN(1);
			DelayXms(120);
			LED_RED(0);
			LED_GREEN(0);
			DelayXms(120);

		}
		SensorHeatPowerOff();
		return 0;
	}

	return 1;

}

#endif
#define	SENDBUF_MAX		10
BYTE SendCirBuf[SENDBUF_MAX];
BYTE SendIdxS = 0;
BYTE SendidxP = 0;

//���͵������ȵ���������ʱ������ٷ��ͣ��������ⷢ��ʱ�ȴ�
void SendCir(BYTE senddata)
{
	SendCirBuf[SendIdxS] = senddata;
	SendIdxS++;
	if (SendIdxS == (SENDBUF_MAX))
		SendIdxS = 0;

}
extern BYTE f_busy;

//ѭ�����ã�ֻҪ���Ϳ��У�ȡ�����ͻ����������ݷ���
void SendCycbuf()
{
	if (f_busy)
		return;

	if (SendidxP != SendIdxS)
	{

		DALI_Send(SendCirBuf[SendidxP]);
		SendidxP++;
		if (SendidxP == SENDBUF_MAX)
			SendidxP = 0;
	}

}

//�������ݷ���������
void SendRF(BYTE KEYFUN)
{
	BYTE i;

	for (i = 0; i < 3; i++)
	{
		//DALI_Send(KEYFUN );
		//DelayXms(100);

		SendCir(KEYFUN);
	}

}

//���������þú���������ʧ׼���貹��У׼
#define	VCC_HEAT	(2.5*1024/(STANDARD_REF_VOLT/1000))		//mv25
BOOL Fcold = 0;
BOOL ColdTest()
{
	float temp1;
	float temp2;

	WORD k;
	temp1 = ((float) VCC_HEAT / (VCC_HEAT - SysInfo.VB) - 1);
	temp2 = ((float) VCC_HEAT / (VCC_HEAT - VA) - 1);
	k = (WORD) temp1 / temp2;
	if (k > 9)
	{
		return 1;

	}
	return 0;

}
WORD ColdCompsation(WORD nppm)
{
	//	 float	temp1;
	//	 float	temp2;
	static BYTE i = 0;
	//	 WORD k;
	//	 temp1=((float) VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) ;
	//	 temp2=((float)VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
	//	 k=(WORD)temp1/temp2;
#if 1
	if (ColdTest() == 1)
	//	 if(k>9)
	{
		//nppm=nppm*10/7;
		i = 1;
	} //10/6
	else
	{
		i++;
		if (i == 1)
		{
			i = 0;
		}
		else if (i <= 3)
		{
			//nppm=nppm*10/8;
			if (VC >= VB)
				nppm = 0;
		}
		else if (i == 4)
		{
			// nppm=nppm*10/9;
			if (VC >= VB)
				nppm = 0;
			i = 0;
		}

	}
	if (Fcold)
	{

		Fcold = 0;
		if (VC >= VB)
			nppm = 0;
	}
#else
	if(ColdTest() == 1)
	//	 if(k>9)

	{
		nppm = nppm * 10 / 7; //10/6
		i = 1;
	}
	else
	{
		i++;
		if( i == 1)
		{
			i = 0;
		}
		else if( i <= 3)
		{
			nppm = nppm * 10 / 8;
		}
		else if( i == 4)
		{
			nppm = nppm * 10 / 9;
			i = 0;
		}

	}

#endif

	return nppm;

}

//����  ��mg/100ml ѪҺ
BYTE CalALCLevel(WORD untmg)
{
	WORD temp2 = untmg;
	BYTE jiu_level = 0;
	if (temp2 >= 80)
		jiu_level = 3;
	else if (temp2 >= 40)
		jiu_level = 2;
	else if (temp2 >= 18)
		jiu_level = 1;
	return jiu_level;
}

/***********************************************************
 ^V
 |
 *SysInfo.VB
 |
 Y
 |    *SysInfo.VC20
 |
 |                  *SysInfo.VC
 |____________________X___________________> P

 x=x1+(y-y1)*(x1-x2)/(y1-y2)
 =20+(VC-sysinfo.VC20)*(20-50)/(sysinfo.VC20-sysinfo.VC)
 //dual point calibration
 ************************************************************/
WORD CalALOTwo(WORD vol)
{
	float pcal;
	WORD Mvc = SysInfo.VC + (SysInfo.VC20 - SysInfo.VC) / 2; //A  M  B
	vol = vol;
	if (SysInfo.VB < VC)
		VC = SysInfo.VB;
	if (VC > SysInfo.VC20) //<20mg
	{
		pcal = (SysInfo.VB - VC) * 20 / (SysInfo.VB - SysInfo.VC20);
	}
	else
		pcal = 20 + (VC - SysInfo.VC20) * (20 - 50) / (SysInfo.VC20
				- SysInfo.VC);
	if (pcal < 0)
		pcal = 0;
	if (pcal > 150)
		pcal = 150;
	return ((WORD) pcal);
}

#define 	STD_ADJUST_PPM20	80
#define 	STD_ADJUST_PPM		130
//#define 	STD_ADJUST_mg		50
//y=k* dx -<  130=k* dv  ->k=130/dv  ->y=130/dv *dx
//����ƾ�����
void calALO()
{
	BYTE i;
	int32 temp1 = temp1; //,temp2 ;
	WORD temp3 = temp3;
	WORD temp2;
	BYTE jiu_level;
	if (qishu < MIN_QISHU)
	{
		DisplayCont = DISPLAY_ERR1;
		return;
	}
#if 0
	if(SysInfo.VB < VC)VC = SysInfo.VB;
	temp1 = (int32)SysInfo.VB - VC; //ʵ�ʲ�����ѹ
	temp3 = SysInfo.VB - SysInfo.VC; //�궨��ѹ
	temp1 = (WORD)((DWORD)STD_ADJUST_PPM * temp1 / temp3); //����PPM
	//	temp1=(WORD)((DWORD)STD_ADJUST_mg*temp1/temp3);
	Currrentjiu_level = 0;
	temp2 = (WORD)(temp1 * 10 / 26); //PPM unit convert to  unit  mg/100ml
#ifndef	 FANGBO_PWR
	temp2 = ColdCompsation( temp2);
#endif

#else
	temp2 = CalALOTwo(0);

#endif
#if 0
	if(temp2 <= 36) temp2 = temp2 * 70 / 100;
	else if(temp2 <= 45) temp2 = 45 - (45 - temp2) * 225 / 100 - 1;
	else if(temp2 <= 55) temp2 = temp2;
	else if(temp2 <= 75) temp2 = temp2 + (temp2 - 56) * 124 / 100; //
	//else if(temp2<=98) temp2=temp2+(temp2-76)*140/100+23;       //--128

	else temp2 = temp2 * 13 / 10;
#endif
	if (temp2 >= 15 && temp2 <= 18)
		temp2 = 18;
	Currrentjiu_level = temp2;
	jiu_level = CalALCLevel(temp2);

	//��ʾ�ƾ���
	DisplayCont = DISPLAY_ONOFF;
	if (jiu_level != 0)
	{
		//�ƾ�����
		LED_RED(LED_ON);; //�����
		LED_GREEN(LED_OFF);; //���̵�
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_o;
		DisBuffer[2] = CHAR_F;
		DisBuffer[3] = CHAR_F;
		Display_All();
		for (i = 0; i < 3; i++) //�оƾ�����
		{
			StepDownSound();
			BUZZY_ON();
			DelayXms(1000);
			BUZZY_OFF();
			DelayXms(50);
		}
		SendRF(FUN_LOCK_CAR); //����������Ϣ;
		LED_RED(LED_OFF);

		LABEL_SAVE_DISP: Jiulist.idx++;
		if (Jiulist.idx >= MAX_JIULST)
		{
			Jiulist.idx = 0;
		}
		Jiulist.Jvol[Jiulist.idx] = Currrentjiu_level; //����ƾ�mg
		//	if(	Jiulist.idx%5==0)
		FLASH_Update((FLADDR) EEP_JIULST_TBLSTART, (BYTE *) &Jiulist,
				EEP_JIULST_LENTH);
		Jdispidx = Jiulist.idx;
		DisplayCont = DISPLAY_ALO;
	}
	else
	{
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_BLK;
		DisBuffer[2] = CHAR_o;
		DisBuffer[3] = CHAR_n;
		//�޾ƾ�
		LED_RED(LED_OFF);; //�����
		LED_GREEN(LED_ON);; //���̵�
		Display_All();
		for (i = 0; i < 2; i++) //		���������� �ε�
		{
			StepUpSound();
			BUZZY_ON();
			DelayXms(50);
			BUZZY_OFF();
			DelayXms(50);
		}
		SendRF(FUN_UNLOCK_CAR); //���Ϳ�����Ϣ
		LED_GREEN(LED_OFF);
		DelayXms(1000); //��ʾ on 1��
		goto LABEL_SAVE_DISP;
		//2012 ��ʾ�ƾ�

	}
	Display_All();
}

void QishuDetect();
void Qishu_EN();
//������5msѭ������
//ÿ100ms��ֻҪ��⵽һ�Σ�����Ч������1��
BYTE qistate_EN = 0;
void QishuDetect()
{

	if (SW_DETECT() == 0 && qistate_EN)
	{
		qishu++;
		qistate_EN = 0;

	}
}
//100ms����һ��
void Qishu_EN()
{
	if ((StateSensor == TEST_WAIT_BLOW_H) || (StateSensor == TEST_WAIT_BLOW_L))
		qistate_EN = 1;
}

#ifdef  FANGBO_PWR
#define		HEAT_TIME_MAX	10
#else
#define		HEAT_TIME_MAX	20
#endif

//������ѹ ��VS VP��
//�ƾ�ͷ100hz����ʱ��5msִ��һ�� ,���������
//��ƾ�ͷΪֱ������50ms����һ��
void TestACHOL()
{
	BYTE i;
	WORD tempv;
	DWORD Dtempv;
	/*	if(PCHARG_DET())
	 {	//���
	 if(StateSensor ==  (AL_STATE_SLEEP_CNT+1))CountHeat=20;
	 }
	 */

	switch (StateSensor)
	{

	case TEST_INIT:
		SensorPowerOff(); //Ϊ��ͬ�� ���Դ�����װ��	�ص�
		//BatteryEnegyDetect();
		VB = 0;
		VC = 0x3ff;
		VA = 0x3ff;
		LED_RED(LED_ON);
		SensorHeatPowerOff();

		CountHeat = HEAT_TIME_MAX; //����10��
#ifdef SECURE_SPI
		{
			extern BYTE F_demo;
			if (F_demo)
				CountHeat = 5;
		}
#endif
		Insystate = 0;

		DisplayCont = DISPLAY_TIME_SEC;
		SensorHeatPowerOn();

		if (AdjustSensor == 1)
			DisplayCont = DISPLAY_ADJ;
		else if (AdjustSensor == 2)
			DisplayCont = DISPLAY_ADJ20;
		else
		{
			//not calibrate
			if (SysInfo.ADJUST != 0x55)
			{
				DisplayCont = DISPLAY_NCAL;
				Display_All();
				//StateSensor=AL_STATE_EXIT;return;
			}

		}

		Display_All();
		SendRF(FUN_DETECT_CAR);
		CountHeat = HEAT_TIME_MAX; //����10��
		//	Currrentjiu_level=0;	   2011.0711
		StateSensor++;
		qishu = 0;
		TEST_SPI(CMD_SET_P54);
		SensorPowerOn(); //�Ӵ�����������ѹ  //ͬ������װ��
		if (i != 0)
		{
			DelayXms(200);

		}
		DelayXms(200); //�ȴ������ѹ  �ȶ�

		break;
	case TEST_A:

		if (AdjustSensor)
		{
			//����2�� ��ʾDISPLAY_ADJ   AAA�������У׼ ������ʾ��
			if (CountHeat < (HEAT_TIME_MAX - 2))
				DisplayCont = DISPLAY_TIME_SEC;

		}

		//���������� �ϵ������3������³� ֵ
#ifndef	   FANGBO_PWR
		if (CountHeat > (HEAT_TIME_MAX - 4))
		{
			tempv = Force_MesureADport(ADPORT_SENSOR);
			VA = min( tempv, VA);
			DIsplaySYSV(VA); //ʵ��û��ʾ   //��ʾmV
			//��ʾ�³�ֵ
			if (AdjustSensor)
			{
				DisplayCont = DISPLAY_VOLT;
				Display_All();
			}
			else
			{
#ifdef	DEBUG_VOL
				DisplayCont = DISPLAY_VOLT;
				Display_All();
#endif
			}
			//
#if 0
			if((Fcold == 0) )
			{
				if(1 == ColdTest())
				{
					Fcold = 1;
					CountHeat = 60;
					StateSensor = TEST_REP;

				}

			}
#endif

		}
		else
			StateSensor++;
#endif
		break;

	case TEST_REP:
		if (CountHeat > 5)
		{
			//�ȴ�ʱ��
			DisplayCont = DISPLAY_TIME_SEC;
			if (CountHeat % 20 == 0)
			{
				SensorPowerOff(); //Ϊ��ͬ�� ���Դ�����װ��
				DelayXms(200);
				SensorPowerOn(); //�Ӵ�����������ѹ  //ͬ������װ?
				VB = 0;
			}
		}
		else
		{
			StateSensor++;
		}
		//BatteryEnegyDetect();
		break;
	case TEST_100_PREV:
		if (CountHeat < HEAT_TIME_MAX - 2)
		{
			DisplayCont = DISPLAY_TIME_SEC;
			if (CountHeat <= 4)
			{
				StateSensor++; //�ȴ�10�� ��100hz 3��

			}
			else
			{
				//BatteryEnegyDetect();
			}
		}
		break;

		//***************100hz 3��
		//********************************************
	case TEST_100HZ_L3:
#ifdef  FANGBO_PWR
		SensorHeatPowerOff(); //100KZ	 3��
#endif
		StateSensor++;
		if (CountHeat <= 3)
		{
			//��������ǰ �����������0 PPM У׼��ѹ
			tempv = Force_MesureADport(ADPORT_SENSOR);
			VB = max( tempv, VB);
			//�궨ʱ��ѹ���ȵ�2.1V����
			if (AdjustSensor)
			{
				DIsplaySYSV(VB); //��ʾmV
				DisplayCont = DISPLAY_VOLT;
				Display_All();

			}
			else
			{
#ifdef	DEBUG_VOL
				DisplayCont = DISPLAY_VOLT;
				Display_All();
#endif
			}
			Dtempv = ((DWORD) (1024 * 1.8 * 1000) / (STANDARD_REF_VOLT));
			//��ѹ����2.1
			if (VB < (WORD) Dtempv)
			{
				VB = 0;
				StateSensor = TEST_REP; //�������Ѹ�
				HeatRetryTime++;
				if (AdjustSensor)
				{
					CountHeat = 20;
					if (HeatRetryTime > 5)
					{
						DisplayCont = DISPLAY_ERR2;
						StateSensor = AL_STATE_EXIT;
					}

				}
				else
				{
					CountHeat = 20;
					if (HeatRetryTime >= 2)
					{
						StateSensor = TEST_100HZ_H3;
						goto LABEL_TEST_READY;
					}
				}
			}

		}

		break;
	case TEST_100HZ_H3:
		//100hz
		SensorHeatPowerOn();
		if (CountHeat != 0)
		{
			StateSensor--;

		}
		else
		{
			LABEL_TEST_READY: LED_GREEN(LED_ON); //�ȴ�������ɺ��̵Ʒ��������Ѵ���
			BUZZY_ON();
			DelayXms(100);
			BUZZY_OFF();
			CountHeat = 9; //�ȴ�9�� ����
			StateSensor++;
		}

		break;
		//*************************************************************

		//***************100hz �ȴ�����  �����  ��8��
		//****************************************************
	case TEST_WAIT_BLOW_L: //�ȴ�����
#ifdef  FANGBO_PWR
		SensorHeatPowerOff(); //100KZ
#endif
		//		 if(sys_time.Time_1_20msec%25==0)LED_GREEN(LED_ON);
		//		 else if(sys_time.Time_1_20msec%25==(25/2))   LED_GREEN(LED_OFF);

		StateSensor++;
		break;
	case TEST_WAIT_BLOW_H: //�ȴ�����
		SensorHeatPowerOn(); //100KZ
		if (CountHeat != 0) //�Դ��ȴ�ʱ��
		{
			tempv = CountHeat;
			HexToBCD((WORD *) &tempv, (BYTE *) &DisBuffer);
			DisBuffer[1] = CHAR_BLK;
			DisBuffer[0] = CHAR_b;
			DisplayCont = DISPLAY_NUM;
			Display_All();

			if (qishu > (MIN_QISHU))
			{
				//��ⴵ���嵽���˳�
				BUZZY_ON(); //������
				DelayXms(200);
				BUZZY_OFF();
				StateSensor += 1; //ֱ��ȥ����
				DisplayCont = DISPLAY_TIME_SEC;
#ifdef	FANGBO_PWR
				CountHeat = 4; //����3�����ֵ
#else
				CountHeat = 2; //zhiliu
#endif
			}
			else
			{
				StateSensor--;
			}
		}
		else
		{
			//һֱû��⵽������	�� �ȴ�����
			StateSensor = AL_STATE_EXIT; //ֱ���˳�
			DisplayCont = DISPLAY_ERR1;

		}
		break;
		//****************************************************

		//***************������ѹ***************
		//****************************************************
		//������ѹ
	case TEST_MEASURE_H:
		//����
		SensorHeatPowerOn();

		//100KZ
		StateSensor++;
		break;

		//����
	case TEST_MEASURE_L:
#ifdef  FANGBO_PWR
		SensorHeatPowerOff();
#endif
		if (CountHeat != 0)
			StateSensor--;
		else
			StateSensor = AL_STATE_CAL;

		//	 if(F_half_sec)	LED_GREEN(LED_OFF);else LED_GREEN(LED_ON);
#ifdef  FANGBO_PWR
		if(CountHeat <= 1)
#else
		if (CountHeat <= 4)
#endif
		{
			//�����ƾ���ѹ
			tempv = Force_MesureADport(ADPORT_SENSOR);
			VC = min( tempv, VC);
			DIsplaySYSV(VC);

			if (AdjustSensor)
			{
				DisplayCont = DISPLAY_VOLT;
				Display_All();
			}

			else
			{
#ifdef	DEBUG_VOL
				DisplayCont = DISPLAY_VOLT;
				Display_All();
#else
				DisplayCont = DISPLAY_TIME_SEC;
#endif
			}

		}

		break;
		//****************************************************************

		// ����״̬
	case AL_STATE_CAL:
		SensorHeatPowerOff();
		SensorPowerOff();
		switch (AdjustSensor)
		{
		default:
		case 0:
			calALO();
			break;
		case 2:
			Dtempv = (DWORD) VC + ((0.2 * 1000) * 1024 / STANDARD_REF_VOLT);
			goto LABEL_CMP;
		case 1:
			Dtempv = (DWORD) VC + ((0.4 * 1000) * 1024 / STANDARD_REF_VOLT);
			LABEL_CMP: if ((WORD) Dtempv > VB)//�궨130PPM ��ѹҪ���� 0��0.6 V���ϣ�������Ч

			{
				DisplayCont = DISPLAY_ERR2;
				StateSensor = AL_STATE_EXIT;
				return;
			}
			else
			{
				//�궨��ȷ
				SysInfo.VA = VA;
				SysInfo.VB = VB;
				if (AdjustSensor == 1)
				{
					SysInfo.VC = VC;
					SysInfo.ADJUST |= 0x50;
				}
				else
				{
					SysInfo.VC20 = VC;
					SysInfo.ADJUST |= 0x05;
				}
				LABLE_SAVE_SINFO:
				{
					extern WORD CalChkSum(BYTE * p, BYTE len);
					SysInfo.chksum
							= CalChkSum((BYTE *) &SysInfo, EEP_SYS_LENTH);
				}
				FLASH_Update((FLADDR) EEP_SYS_TBLSTART, (BYTE *) &SysInfo,
						EEP_SYS_LENTH);

			}
			break;

		}

		StateSensor = AL_STATE_EXIT;
		break;

		//�˳�״̬
	case AL_STATE_EXIT:
		SensorHeatPowerOff();
		SensorPowerOff();
		StateSensor = AL_STATE_SLEEP_CNT;
		//BatteryEnegyDetect();
		//if(AdjustSensor==1){if(CountHeat==2)DisplayCont=DISPLAY_TSTING ;   }
		break;

		// **************************************
		//׼������
	case AL_STATE_SLEEP_CNT:
		CountHeat = 10;
		StateSensor++;
		//BatteryEnegyDetect();

		break;
	case AL_STATE_SLEEP_CNT + 1:
		LABEL_AL_STATE_SLEEP_CNT:
#ifdef	EXTERNAL_POWER_NOSLEEP		//Ӳ��������⣬��ӵ�Դ ��ΪҲ����
		if(PCHARG_DET())
		{
			//���
			if((CountHeat == 2) || (CountHeat == 3))
			{
				Insystate = 0;
				CountHeat = 20; //������ʱ��
				DisplayCont = DISPLAY_TSTING;
			}
		}
		else
#endif
		//BatteryEnegyDetect();
		if (CountHeat == 0)
		{
			GoSleep();
			//��ʼ���������ڿ��жϺ���ʱ����ʹ�����ж϶�ʱ
			{
				extern void Init_LCD(void);
				Init_LCD();
			}

			Jdispidx = Jiulist.idx; //���´ӵ�ǰ�Ĳ���λ����д
			StateSensor = AL_STATE_SLEEP_CNT;
			DisplayCont = DISPLAY_TSTING;

		}

		break;
	default:
		//BatteryEnegyDetect();
		goto LABEL_AL_STATE_SLEEP_CNT;
		break;
	}

}

void MeasurePS()
{
	StateSensor = TEST_INIT;
	HeatRetryTime = 0;
}

//У׼�ƾ�	������ 	V2=	0.05%BAC������ѹ��
//δͨ����ѹ 		V1
//DV1= V1-V2

//ʵ�ʲ���ʱ
//V3 ����ʱ��ѹ
//V4:δ����ʱ��ѹ
//DV2=V4-V3

void DO_Key_Action()
{
	BYTE newkey;

	if (NEW_KEY == KEY_NOKEY)
		NEW_KEY = ScanKey();
	if (StateSensor < AL_STATE_EXIT)
	{
		if (NEW_KEY == KEY_DISPLAY)
		{
			StateSensor = AL_STATE_EXIT;
			DisplayCont = DISPLAY_TSTING;
			Insystate = 0;
		} //��ʾ�������Ƕ���  �˳�����
		NEW_KEY = KEY_NOKEY;
	}
	if (NEW_KEY == KEY_NOKEY)
		return;
	newkey = NEW_KEY;
	NEW_KEY = KEY_NOKEY;

	Count_5S = 5;

	switch (newkey)
	{
	//����
	case KEY_RANCODE:
	{
		extern _SYSINFO SysInfo;
		WORD tmpran;
		DisplayCont = DISPLAY_NUM;

		tmpran = rand();
		SysInfo.TRANCODE = tmpran;
		SysInfo.TRANCODE = SysInfo.TRANCODE << 12;
		tmpran = rand() & 0x0fff;
		SysInfo.TRANCODE |= tmpran;

#ifdef	DEBUG_BYLCD
		DisBuffer[0] = (SysInfo.TRANCODE >> 12) & 0x0f;
		DisBuffer[1] = (SysInfo.TRANCODE >> 8) & 0x0f;
		DisBuffer[2] = (SysInfo.TRANCODE >> 4) & 0x0f;
		DisBuffer[3] = SysInfo.TRANCODE & 0x0f;
		Display_All();
#endif
		TransCmd = FUN_MATCH_CODE; //��������
		SendRF(TransCmd);
		{
			extern WORD
			CalChkSum(BYTE * p, BYTE len);
			SysInfo.chksum = CalChkSum((BYTE *) &SysInfo, EEP_SYS_LENTH);
		}
		FLASH_Update((FLADDR) EEP_SYS_TBLSTART, (BYTE *) &SysInfo,
				EEP_SYS_LENTH);
		StateSensor = AL_STATE_SLEEP_CNT;
	}
		break;

	case KEY_EON:
	{
		BYTE i;
		//����Ʊ�����
		DisplayCont = DISPLAY_TSTING;
		Display_All();
		DisplayCont = DISPLAY_EONOFF;
		DisBuffer[0] = CHAR_BLK;
		DisBuffer[1] = CHAR_E;
		DisBuffer[2] = CHAR_o;
		DisBuffer[3] = CHAR_n;
		//�޾ƾ�
		LED_RED(LED_OFF);; //�����
		LED_GREEN(LED_ON);; //���̵�
		Display_All();
		for (i = 0; i < 2; i++) //		���������� �ε�
		{
			StepUpSound();
			BUZZY_ON();
			DelayXms(50);
			BUZZY_OFF();
			DelayXms(50);
		}
		SendRF(FUN_UNLOCK_CAR); //���Ϳ�����Ϣ
		LED_GREEN(LED_OFF);

		DelayXms(1000); //��ʾ on 1��
	}
		break;

	case KEY_TESTRF:
#if 0 //def TEST_FACTORY
		{
			static BYTE test1 = 0;
			BYTE tmpcode;
			test1++;
			if(test1 == 4)test1 = 0;
			if(test1 == 0) tmpcode = FUN_LOCK_CAR;
			else if(test1 == 1) tmpcode = FUN_UNLOCK_CAR;
			else if(test1 == 2) tmpcode = FUN_DETECT_CAR;
			else if(test1 == 3) tmpcode = FUN_CHAOSHI_CAR;
			else tmpcode = FUN_DETECT_CAR;

			DisBuffer[3] = test1;
			DisplayCont = DISPLAY_MATSEND;
			Display_All();
			DALI_Send(tmpcode);
			StateSensor = AL_STATE_SLEEP_CNT;

		}
#endif
		break;

	case KEY_JIUJING:
		if (PI_ADJUST_DET() || PI_ADJUST_DET20())
		{
			DelayXms(100);
			if (PI_ADJUST_DET())
			{
				AdjustSensor = 1;
				SysInfo.ADJUST &= ~0x50;
				DisplayCont = DISPLAY_ADJ;
				goto LABEL_ADJUST_ALO;
			}
			else if (PI_ADJUST_DET20())
			{
				AdjustSensor = 2;
				SysInfo.ADJUST &= ~0x05;
				DisplayCont = DISPLAY_ADJ20;
				goto LABEL_ADJUST_ALO;
			}

		}

		DisplayCont = DISPLAY_TSTING;
		AdjustSensor = 0;
		goto LABEL_TEST_ALO;
		break;
	case KEY_ADJUST:
		LABEL_ADJUST_ALO: //DisplayCont = DISPLAY_ADJ;
		LABEL_TEST_ALO: Display_All();
		MeasurePS();
		break;

	case KEY_DISPSTD:
	{
		extern void diaplay_std();
		diaplay_std();
	}
		break;

		//��ʾ
	case KEY_DISPLAY:
		if (Insystate == 0)
		{
			if (DisplayCont == DISPLAY_ALO)
			{
				//Jdispidx++;
				//if(Jdispidx>=MAX_JIULST)Jdispidx=0;

				//�ؿ�����
				if (Jdispidx != 0)
					Jdispidx--;
				else
					Jdispidx = MAX_JIULST - 1;

				//��
				//	Jdispidx++;
				//	if(Jdispidx>=MAX_JIULST)Jdispidx=0;
			}
			DisplayCont = DISPLAY_ALO;
		}
		else
		{
			DisplayCont++;
			if (DisplayCont > DISPLAY_SYS_VCEND || DisplayCont
					< DISPLAY_SYS_RAN)
				DisplayCont = DISPLAY_SYS_RAN;

		}
		StateSensor = AL_STATE_SLEEP_CNT;

		break;
	default:
		//		KEY_PRINTF(printf("-othkey-"));
		break;

	}

}

