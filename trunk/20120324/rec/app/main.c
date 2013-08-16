////////////////////////////////////////////////////////////////////////////
///////////////
// �ļ�����
// �����ߣ�
// ����ʱ�䣺
// ����������
////////////////////////////////////////////////////////////////////////////
///////////////

#ifndef	_MAIN_C_
#define	_MAIN_C_
#include "general.h"
#include <math.h>
//#define	DEBUG_MAIN
#ifdef	DEBUG_MAIN
#define	DPRINTF(x)  x
#else
#define	DPRINTF(x)  //
#endif
#include "spicom.h"
WORD ConONTimer=0;    		  //20���ӿ��� 30���ʱ����
BYTE PowerbeingOn=0;
BYTE PowerONcnt=0;
BYTE PowerOFFcnt=0;
//0 
//1 ��ͨ��
//2 �����о�
BYTE PWR_State=0;  
_SYSINFO SysInfo;
BYTE F_demo=1;
	extern BYTE   yuyin_time;
			extern void   SetYuyin(BYTE yuin);


//��һ��װ��ͨ�磬������ʾ
//ACC on �� 2Сʱ����  3Сʱ���� ���Ժ�ÿ30��������һ�Σ�
//ͣ�����̵����Ͽ���״̬ ת��acc on 15���ӿ��Կ��Զ���ͨ�̵��������Դ��
//�������̵�����ͨ��״̬��  ת�� acc off���趨20�ֺ󽫶Ͽ��̵���������ͣ��̬
//���ACC OFFʱ �������о�״̬	����15���Ӻ�ſ���
//���ACC ON״̬�������оƣ������г�״̬��
//�����޾�״̬,������ͨ�̵��� , 5�����ڿ��Է��������5���Ӻ�û�����Ͽ��̵�����


//2012.08 ¼��
//��¼���豸�� ��⵽RF�������� ��⵽��𿪹� ¼�� 
//ֹͣ¼�ƣ� ��𿪹ضϿ�  5���Ӻ�ֹͣ 

//���� �ȶ�a mat1

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
void Init_EPROM()
{  BYTE xdata  buf;
	_SYSINFO  tmpsysinfo;//={0,0};
	FLASH_Read(&buf,EEPROM_ID,1);
	if(buf==BOARD_ID_VER)
	{ //read from flash


	  	FLASH_Read((BYTE*)&tmpsysinfo,(FLADDR)(EEP_SYS_TBLSTART),EEP_SYS_LENTH) ;
		SysInfo= tmpsysinfo;
	}else
	{

	//��ʼ����
		SysInfo.TRANCODE=0x5555;
		SysInfo.TRANCODE1=0x0000;
		FLASH_Update((FLADDR)EEP_SYS_TBLSTART,(BYTE*)&SysInfo,EEP_SYS_LENTH);
		DPRINTF(printf("-----InitEPROM-----...\r\n"))	 ;
	//	FLASH_Clear ((FLADDR) EEP_CAL_ADDR, (WORD)MAX_TEMP_POINT*EEP_CAL_LENGTH) ;
		buf= BOARD_ID_VER;
		FLASH_Update((FLADDR)EEPROM_ID,(BYTE *)&buf,1) ;
	  

	}

	
}


void   DAPI_StoreFlashConfig(){}

//ÿ500�������һ��
void Task_500ms()
{


	
}





//ÿ50�������һ��
void Task_50ms()
{


}
BYTE ACCstate=0;	 //��⵽��ACC ״̬
BYTE relay_state=0; // �̵����ϵ�״̬  ������ͨ  0���Ͽ�
WORD RELAYTobeOFFTime=0;//  acc off ��Ҫ�Ͽ���ʱ����� �룬20�� ����󽫶ϼ̵���
WORD RelayToBeONTime=0;    //acc on ��Ҫ��ͨ��ʱ����� �룬5���� �󽫽�ͨ�̵���
WORD ACCOnTimeCnt=0;  //acc on��ʱ��
BYTE Mstate=0;//0:




void InitRelay()
{
	
   	relay_state=0;
	POWER_OFF();
	RELAYTobeOFFTime=0;
	RelayToBeONTime=0;
//	Mstate=0;
	YUYIN_NOYUYIN();
	Mstate=1;//�ϵ翪��
	TctoberecStop=0;
	REC_STOP();
}
void ACC_State()
{
 static char ACCdetectTC=0; 
 //���acc״̬ 
	if(PWRSW_DETECT)
	{
			ACCdetectTC++;
			if(	ACCdetectTC>=3)
			{	
				//if(ACCdetectTC==3)SetYuyin(YUYINT_DETECT);	
				ACCstate=1;
				ACCdetectTC=4;
			}
	}
	else
	{
			ACCdetectTC--;
			if(	ACCdetectTC<-3)
			{
				ACCstate=0;
				ACCdetectTC=-4;
			}
	 }

	
}

//ÿ�����һ��
//2Сʱ����  3Сʱ���� ���Ժ�ÿ30��������һ��
void yuyintixing()
{
WORD  tmp;
BOOL chaoshi=0;
	if(sys_time.Time_1_sec%20!=1)return; //ÿ20������һ��

	//��������
	 {

		if(ACCOnTimeCnt==2*60||(ACCOnTimeCnt==3*60 ))
		{
					
			chaoshi=1;	
		}
		else if	(ACCOnTimeCnt> 3*60 )
		{
		   tmp= ACCOnTimeCnt-3*60;
		   if(tmp%30==0)chaoshi=1; //ÿ30��������
		}
	 }
	// else ACCOnTimeCnt=0;
	 if(chaoshi)SetYuyin(YUYINT_CHAOSHI);
}
//һ���Ӽ���һ��
void  chaoshijisuan()
{
	 if(ACCstate)
	 {
		ACCOnTimeCnt++;
	 }
		else
	{
		ACCOnTimeCnt=0;
	}

}

//��һ���ϵ���뿪��״̬�����贵���Ϳɴ�𣬣���⵽acc on �̵��������ϣ��Ա㰲װ
//���Ҫ��,û�д���ʱ�򿪳�,�����̵���,������
  //1 �����Ժ�,û�оƾ�,��5�������ڶ����Կ���,��������̵���

  //��3����������Ҫ�����ٴ�
  //2.�����Ժ��оƾ�,��15�������ڿ����˳�,�����̵���
  //15�����Ժ�,���Կ�������Ϊ5���ӣ�
  //ͣ��20�����Ժ�,���ܿ���,�����̵���
 //��û���ֻ����������������5�����Ժ��������

 //��ʱ��������,5�����Ժ�,����������
  
   //��ACC ONʱ,state==1
   //ACC OFFʱ,20�����Ժ�,state==0,���ܿ���
 // delay_ms(6);
  
  //for(i=0;i<100;i++)//����������ƽ��ȼ��
	//      {
   //     delay_ms(10);
     //    }
  //state==0,ͣ������10���ӵ�״̬
  //state==1,������״̬
    //state==2,�յ��оƾ���״̬
	//state=3;//  �����޷�������״̬����Ҫ��
  //ң��������,���ܷ���,ƣ�ͼ�ʻ
//Zzhuangtai
//ACC ON�� �оƣ��޶���    �޾ƣ� ��ͨ  
//ACC OFF���оƣ� �Ͽ�     �޾ƣ� �Ƚ�ͨ��5���Ӻ���ACCon�Ͽ�
//ACON-ACoff    20�����Ժ�Ͽ�
//ACoff_ACCON   ����ACCon 15���Ӻ��ͨ		  ״̬��



/*

  1.�����оƣ�15���ӿ��Կ�
  2.�����޾ƣ�5�����ڶ����Կ� 
  3.������ת��ACC OFF��20�����ڲ���ֱ�ӿ�
  4.ͣ����բ״̬��������Ϩ��20���Ӻ󣩣�ת��ACC ON����Ҫ�����޾�ʱ�ɿ�����15���Ӻ�ֱ�ӿ�
  5.������ACCON�� ״̬�������о��޾ƶ���Ϩ��
  6.����2Сʱ�������ѣ�3Сʱ��ÿ30��������һ��


*/
void TaskRelay()
{
 
  


	switch(Mstate)
	{

	 
		
//��բͣ��״̬ 
//��acc on 15����
		case	0:
		if(	ACCstate==0) 
		{				
			POWER_OFF();
			RELAYTobeOFFTime=0;
		}else 
		{
		 Mstate=3;
		 RelayToBeONTime=	15*60;
		}
		break;

//����״̬
// ת�� acc off���趨20�ֺ�ϵ�
		case	1:
		if(	ACCstate==1) //
		{				
			POWER_ON();		//һֱ��
			RelayToBeONTime=0;	
		}else 
		{
		
			 RELAYTobeOFFTime=20*60;
			 Mstate=2;
			
		}
		break;


		//׼��ͣ��״̬				accoff-��accON
		case	2:
		if(ACCstate==0)
		{
				//׼���ϵ�
				if(	RELAYTobeOFFTime)	
				{ RELAYTobeOFFTime--;
				if(RELAYTobeOFFTime==0) {	Mstate=0;}
				}
		}
		else
		 Mstate=1;
		


		break;

		//׼������״̬
		case	3:
		if(ACCstate==1)
		{
				//׼����
				if(	RelayToBeONTime)
				{ RelayToBeONTime--;
					if(RelayToBeONTime==0)
					{
						Mstate=1;
					}
				}
		}
		else Mstate= 0;
		

		break;


		//�����о�״̬	  �� 15���Ӻ�ſ�
		case	4:
		if(	ACCstate==0)
		{
			POWER_OFF();
			RelayToBeONTime=15*60;
		    Mstate=3;
		}else
		{
		  if(relay_state==1)Mstate=1;//���ɽ��뿪��״̬
		  else 	Mstate=0;			 //�о��Ҽ̵�����û���ϣ�����ض�״̬


		}
		 
	
		break;


			//�����޾�״̬,�����պϵ�Դ	 ,������5����ͣ��׼�������û�з�����
		case	5:
		POWER_ON();
		if(ACCstate==0)
		{
		    RELAYTobeOFFTime=5*60;
			Mstate=2;
		} else 		 Mstate=1;
		   
		
	  break;
	   default:
	   break;
	
	

       }

//��ֹ����
	if(relay_state==1)
	{	
		POWER_ON();
	}
	else 
	{
		 POWER_OFF();
		 }
}

extern BYTE KMATCH;
extern BYTE KTMOUT;
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
void main (void)
{	

	PCA0MD &= ~0x40;                    // Disable Watchdog timer
	Init_VAR();
	

	Init_Device();
	Init_patch();
	REC_STOP();
    P2MDOUT|=0x01; //����
	SetYuyin(YUYINT_NOYUYIN)	;
	DelayXms(300);
	Init_EPROM();
#ifdef	LCD_EN
	Init_LCD();
#endif
	InitRelay();
	DPRINTF(printf("MAIN Program\n" ))	 ;
	PIORFIN=1;
	{extern void InitSPICom(void) ;
	extern char TEST_SPI(char CMD_a);
	InitSPICom()  ;
	
	if(1==TEST_SPI(CMD_RD_ID)){
		F_demo=0;
		SetYuyin(YUYINT_DETECT)	 ;

		}
	else {
			F_demo=1;
			SetYuyin(YUYINT_CHAOBIAO);
			}
	}

	while(1)
	{
		
	
		if(F_5ms)
		{
			F_5ms=0;
			

		}
	//	if(F_10ms)
	//	{
	//		F_10ms=0;

	//	}
		if(F_50ms)
		{	F_50ms=0;
			
			Task_50ms();
		    if(KTMOUT)
			{
				if(KMATCH==2) 
				{
				if(sys_time.Time_1_20msec>80)LED_RED(LED_OFF); //����
				else LED_RED(LED_ON);
				}
				
			}	
			
		}
		if(F_100ms)
		{  
			F_100ms=0;
			DO_Key_Action()	 ;

			Display_All();
		    ACC_State(); 
			
			
		}
	/*	if(F_200ms)
		{	 
			

			F_200ms=0;
		
			
			
		}
		*/
		if(F_500ms)
		{	F_500ms=0;
			Task_500ms();
		
		{
			if(KTMOUT )
			{
			  
			  if(KMATCH==1) LEDIO=!LEDIO;
			  	KTMOUT--;
			 	if(KTMOUT==0)
				{
			 		LED_RED(LED_OFF);;	//Ϩ��
					KMATCH=0;
				 }
			  
			}
		
				
		}
			
			
		}

		
		if(F_1000ms)
		{	
		
			F_1000ms=0;
			yuyintixing();
			if(KTMOUT==0)TaskRelay();	   //�Ƕ���״̬
			if(yuyin_time)
			{
			 yuyin_time--;
			 if(yuyin_time==0)SetYuyin(YUYINT_NOYUYIN)	;

			}
 
 //¼��ʱ��
			if(ACCstate){TctoberecStop=360;}
			
			if(TctoberecStop!=0)
			{
			REC_START();  //ֻҪֹͣʱ��δ��������¼��
			TctoberecStop--;
			if(TctoberecStop==0){REC_STOP();}
			}

		
		//	DPRINTF(printf("time=%bd \n" ,sys_time.Time_1_sec))	 ;

		}

		if(F_1MIN)
		{
		 F_1MIN=0;
		 chaoshijisuan();

		}
		
		
	}
	
}




#endif
