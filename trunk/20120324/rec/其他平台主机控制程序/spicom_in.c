#ifndef	_SPICOM_IN_C_

//�����ļ�����
//spicom.c
#include "general.h"
#define	_SPICOM_IN_C_
#include "spicom_IN.h"

#if      defined CUSTOM_ACD01_KENIANDA
//������ IO ����
void 	Set_MCLK_Mode_IN(void)		{msWriteGPIO_12(GPI);}
void 	Set_MCLK_Mode_OUT(void)	{}
void		Set_SCLK_Mode_IN(void)		{msWriteGPIO_19(GPI);}
void		Set_SCLK_Mode_OUT(void)	{} 
void		Set_SDA_Mode_IN(void)		{msWriteGPIO_20(GPI);}
void		Set_SDA_Mode_OUT(void)		{}
void		SetMCLK()	{msWriteGPIO_12(GPO_High);}
void		ClrMCLK()	{msWriteGPIO_12(GPO_Low);}
void		SetSCLK() 	{msWriteGPIO_19(GPO_High);}
void		ClrSCLK() 	{msWriteGPIO_19(GPO_Low);}
void		SetSDA()  	{msWriteGPIO_20(GPO_High);}
void		ClrSDA()  	{msWriteGPIO_20(GPO_Low);}
char		GetMCLK()	{return msReadGPIO_12();}
char		GetSCLK()	{return msReadGPIO_19();}	
char		GetSDA()		{return msReadGPIO_20();}	
#elif defined EPD1081

//epd1081
void 		Set_MCLK_Mode_IN(void)		{msWriteGPIO_12(GPI);}
void 		Set_MCLK_Mode_OUT(void)	{}
void		Set_SCLK_Mode_IN(void)		{msWriteGPIO_17(GPI);}
void		Set_SCLK_Mode_OUT(void)	{} 
void		Set_SDA_Mode_IN(void)		{msWriteGPIO_18(GPI);}
void		Set_SDA_Mode_OUT(void)		{}
void		SetMCLK()	{msWriteGPIO_12(GPO_High);}
void		ClrMCLK()	{msWriteGPIO_12(GPO_Low);}
void		SetSCLK() 	{msWriteGPIO_17(GPO_High);}
void		ClrSCLK() 	{msWriteGPIO_17(GPO_Low);}
void		SetSDA()  	{msWriteGPIO_18(GPO_High);}
void		ClrSDA()  	{msWriteGPIO_18(GPO_Low);}
char		GetMCLK()	{return msReadGPIO_12();}
char		GetSCLK()	{return msReadGPIO_17();}	
char		GetSDA()	{return msReadGPIO_18();}	
#else
//�ҵ�ƽ̨

#ifndef	 PMCLK

#define	PMCLK	P05

#endif
#ifndef	PSCLK
			  
#define	PSCLK	P10
#endif

#ifndef	PSDA

#define	PSDA	P17
#endif
  //8051F330
void 		Set_MCLK_Mode_IN(void)		{PMCLK=1;}
void 		Set_MCLK_Mode_OUT(void)	{}
void		Set_SCLK_Mode_IN(void)		{PSCLK=1;}
void		Set_SCLK_Mode_OUT(void)	{} 
void		Set_SDA_Mode_IN(void)		{}
void		Set_SDA_Mode_OUT(void)		{PSDA=1;}

void		SetMCLK()	{PMCLK=1;}
void		ClrMCLK()	{PMCLK=0;}
void		SetSCLK() 	{PSCLK=1;}
void		ClrSCLK() 	{PSCLK=0;}
void		SetSDA()  	{PSDA=1;}
void		ClrSDA()  	{PSDA=0;}
char		GetMCLK()	{return ((PMCLK==1)?1:0);}
char		GetSCLK()	{return ((PSCLK==1)?1:0);}	
char		GetSDA()	{return ((PSDA==1)?1:0);}	
#endif


#define	COM_TIME_OUT	500		//���峬ʱms  �˴�����Ϊ500ms ��һ�㲻�Ķ�
#define	BASE_COUNT	50		//ÿ�����������ʱ��ms ���˴�Ϊ10ms��ÿ��ƽ̨��һ����Ҫ��

//500ms ��ʱ
//500ms �ļ�����50*10
#define	COSTN_TimeOutCNT		COM_TIME_OUT/BASE_COUNT   // ��Ҫ�ļ��� ֵ=500ms/BASE_COUNT=�����綨ʱ������COSTN_TimeOutCNT =500ms/BASE_COUNT=500/10=50
unsigned char  xdata   gbTimeOutConst;		//��ʱʱ�� ���ӿ�����
void Delay5us()
{
BYTE i;
for (i=0;i<10;i++);

// _nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();
//  _nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();

}

//������ͨѶǰ���ã��ǵ����п�Ϊ����״̬���˿ڵ�ƽһ��Ϊ��
void InitSPICom(void)
{
		Set_MCLK_Mode_IN();
		Set_SCLK_Mode_IN();
		Set_SDA_Mode_IN();
		gbTimeOutConst=COSTN_TimeOutCNT;
	
}

#endif
