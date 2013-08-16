////////////////////////////////////////////////////////////////////////////
///////////////
// �ļ�����
// �����ߣ�
// ����ʱ�䣺
// ����������
/*
 �ƾ�����
 1.У׼������20��	  �������޾Ƶ�ѹ�����죬��130PPM�����壬���Ե�ѹ
 2.



 */

//��ⴵ���嵽���˳�
////////////////////////////////////////////////////////////////////////////
///////////////
#ifndef	_MAIN_C_
#define	_MAIN_C_
#include "general.h"
#include <math.h>

#ifdef	SECURE_SPI
#include "spicom.h"
//#include "spicom_in.h"
#endif
//#define	DEBUG_MAIN
#ifdef	DEBUG_MAIN
#define	DPRINTF(x)  x
#else
#define	DPRINTF(x)  //
#endif
BYTE NEW_KEY = KEY_NOKEY;

_SYSINFO SysInfo;
_JIUJING_LST Jiulist;
extern void
Init_sensor();
extern BYTE StateSensor; //AL_STATE_EXIT
extern BYTE DisplayCont;
extern BYTE DisBuffer[];
extern BYTE Insystate; //���� ����ʾϵͳ״̬
BYTE BZY_on;
#if 1
WORD
CalChkSum(BYTE *p, BYTE len)
{
  BYTE i;

  WORD chksum = 0;
//	p=(BYTE*)&SysInfo;
  p = p + 2;						//�����ʼ��У��������
  for (i = 2; i < len; i++)
    {
      chksum += *p;
      p++;
    }
  return chksum;
}
#endif
////////////////////////////////////////////////////////////////////////////
///////////////
// ��������
// ��д�ߣ�
// �ο����ϣ�
// �� �ܣ�
// ���������
// ���������
// �� ע��
////////////////////////////////////////////////////////////////////////////
///////////////
void
Init_EPROM()
{
  BYTE buf, retrycount = 3;
  _SYSINFO tmpsysinfo;						//={0,0};

  WORD sum;
  BOOL eepok;
  extern BYTE Jdispidx;
  DelayXms(300);
  Jiulist.idx = 0 - 1;
  for (buf = 0; buf < MAX_JIULST; buf++)
    Jiulist.Jvol[buf] = 0xffff;

#ifndef NVRAM_USE_EEP24CXX
  do
    {
      FLASH_Read(&buf, (FLADDR) EEPROM_ID, 1);
      FLASH_Read((BYTE*) &tmpsysinfo, (FLADDR) (EEP_SYS_TBLSTART),EEP_SYS_LENTH);
      sum = CalChkSum((BYTE*) &tmpsysinfo, EEP_SYS_LENTH);
      if ((buf == BOARD_ID_VER)&&(sum==tmpsysinfo.chksum)){eepok=1;break;}
      DelayXms(300);
    }
  while (retrycount--);

  if (eepok)
#endif
    { //read from flash

      FLASH_Read((BYTE*) &SysInfo, (FLADDR) (EEP_SYS_TBLSTART), EEP_SYS_LENTH);
      FLASH_Read((BYTE*) &Jiulist, (FLADDR) (EEP_JIULST_TBLSTART),
          EEP_JIULST_LENTH);

    }
#ifndef NVRAM_USE_EEP24CXX
  else
    {
      DisplayCont = DISPLAY_ERR0;
      Display_All();	   //��ʾ��ʼ����
      //��ʼ����
      SysInfo.TRANCODE = 0x5555;
      SysInfo.ADJUST = 0;
      SysInfo.VA =
          (WORD) ((DWORD) DEFAULT_NOCOAL_VA * 1024 / STANDARD_REF_VOLT);		//У׼
      SysInfo.VB =
          (WORD) ((DWORD) DEFAULT_NOCOAL_VB * 1024 / STANDARD_REF_VOLT);		//У׼
      SysInfo.VC = (WORD) ((DWORD) DEFALT_130PPM_VC * 1024 / STANDARD_REF_VOLT); //130PPM
      SysInfo.chksum = CalChkSum((BYTE*) &SysInfo, EEP_SYS_LENTH);
      FLASH_Update((FLADDR) EEP_SYS_TBLSTART, (BYTE*) &SysInfo, EEP_SYS_LENTH);
      DPRINTF(printf("-----InitEPROM-----...\r\n")) ;
      FLASH_Update((FLADDR) EEP_JIULST_TBLSTART, (BYTE*) &Jiulist,
          EEP_JIULST_LENTH);
      buf = BOARD_ID_VER;
      FLASH_Update((FLADDR) EEPROM_ID, (BYTE *) &buf, 1);

    }
#endif
  //��ʼ������
    {
      if (Jiulist.idx >= MAX_JIULST)
        Jiulist.idx = 0 - 1;
      Jdispidx = Jiulist.idx;

    }

}

void
DAPI_StoreFlashConfig()
{
}

//ÿ1000�������һ��
void
Task_1000ms()
{

  /*1 SECOND*/

  if (F_FlashSave)
    {
      F_FlashSave--;
      if (F_FlashSave == 0)
        DAPI_StoreFlashConfig();
    }
  if (++sys_time.Time_1_sec >= 60) //60sec
    {

      sys_time.Time_1_sec = 0;
      F_1MIN = 1;
#if 0

        {
          if(++sys_time.Time_1_min>=60) //60min
            {
              sys_time.Time_1_min=0;
              if(++sys_time.Time_1_hour>=24)sys_time.Time_1_hour=0;

            }
        }
#endif
    }

}

//ÿ50�������һ��
void
Task_50ms()
{

}
//��ʾ�궨��ֵ��ѹ mv��λ
void
diaplay_std()
{

  extern void
  DIsplaySYSV(WORD tmp);
  DisplayCont = DISPLAY_SYS_VS;
  Display_All();
  Insystate = 1;

}
#ifdef	SECURE_SPI
BYTE F_demo = 1;
#endif

void
QishuDetect();
void
Qishu_EN();
////////////////////////////////////////////////////////////////////////////
///////////////
// ��������
// ��д�ߣ�
// �ο����ϣ�
// �� �ܣ�
// ���������
// ���������
// �� ע��
////////////////////////////////////////////////////////////////////////////
///////////////
void
main(void)
{
  PCA0MD &= ~0x40;                    // Disable Watchdog timer
  Init_VAR();
  Init_Device();
  Init_patch();
  BUZZY_OFF()
  ;
  Init_sensor();
  P1MDOUT &= ~0x06;	//p11 p12 ������	 LCDʱ�����ݺͼ���
  P1MDOUT &= ~0x40;	//p16 ������	LCDѡͨ
  P1MDOUT &= ~0x80;	//p17 ������  LCD��Դ�ܿ�����
#ifdef	SECURE_SPI
  P0MDOUT &= ~0x80;	//p07 ������   //����MCLK
  P07 = 1;
#endif
  P0MDOUT &= ~0x20;  //������� ���ߣ�������δ��  �ͣ����
  P1 |= 0x46;	  //srb clk data
  PO_LCD_POWER(P_LCD_ON);
  Init_LCD();
  Init_EPROM();
#ifdef	SECURE_SPI
    {
      extern void
      InitSPICom(void);
      extern char
      TEST_SPI(char CMD_a);
      BYTE tryc = 3;
      InitSPICom();
      F_demo = 1;
      do
        {
          if (1 == TEST_SPI(CMD_RD_ID))
            {
              F_demo = 0;
              break;
            }

          DelayXms(300);

        }
      while (tryc--);
      VDM0CN |= 0x80; // �͵�ѹ��λ����
      if (1 == F_demo)
        {

          DisplayCont = DISPLAY_DEMO;
          Display_All();	   //��ʾ��ʼ����

        }

    }

#endif
  DPRINTF(printf("MAIN Program\n" )) ;

  while (1)
    {

        {
//WORD i  ;extern 	  BYTE StateSensor	  ;
//     	if(StateSensor==-1)
//		LEDIO=!LEDIO;
        }
      if (F_5ms)
        {
          F_5ms = 0;
          QishuDetect();

#ifdef FANGBO_PWR
            { extern void TestACHOL();
              TestACHOL();

            }
#endif
        }
      //	if(F_10ms)
      //	{
      //		F_10ms=0;

      //	}

      if (F_50ms)
        {
          F_50ms = 0;

          Task_50ms();
#ifndef FANGBO_PWR
            {
              extern void
              TestACHOL();
              TestACHOL();
            }
#endif

        }
      if (F_100ms)
        {
          extern void
          BatteryEnegyDetect();

          F_100ms = 0;
          Qishu_EN();
          DO_Key_Action();
          Display_All();
            {
              extern void
              SendCycbuf(void);
              SendCycbuf();

            }
          BatteryEnegyDetect();	 //201204

        }
      if (F_200ms)
        {

          F_200ms = 0;

        }
      if (F_500ms)
        {
          F_500ms = 0;

        }

      if (F_1000ms)
        {
          extern BYTE CountHeat;
          F_1000ms = 0;
          Task_1000ms();

          if (CountHeat != 0)
            {
              CountHeat--;
            }
          //	DPRINTF(printf("time=%bd \n" ,sys_time.Time_1_sec))	 ;

        }

    }

}

#endif
