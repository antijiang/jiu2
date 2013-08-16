
#include "general.h"

#include <stdlib.h>
#define		FUN_MATCH_CODE	0x0a
#define		FUN_LOCK_CAR	0x02
#define		FUN_UNLOCK_CAR	0x03
#define		FUN_DETECT_CAR	0x04
#define		FUN_CHAOSHI_CAR	0x05
#include <math.h>
//#define		DEBUG_VOL
 enum{
TEST_INIT=0,
TEST_A,
TEST_REP,
TEST_100_PREV,
TEST_100HZ_L3,
TEST_100HZ_H3,
TEST_WAIT_BLOW_L,
TEST_WAIT_BLOW_H,
TEST_MEASURE_H ,
TEST_MEASURE_L,
AL_STATE_CAL=	0x10,			   //计算
AL_STATE_EXIT=	0x20,			   //退出
AL_STATE_SLEEP_CNT=0x50,

};




//#define	AL_STATE_SLEEP_CNT	0x50		   //准备进入 idle模式 
extern BYTE NEW_KEY;
extern _SYSINFO SysInfo;
extern _JIUJING_LST Jiulist;
extern BYTE DisBuffer[5]  ;
BYTE Jdispidx=0;
char qishu=0;//吹气数
BYTE TransCmd=FUN_LOCK_CAR;
#ifdef  FANGBO_PWR
#define	MIN_QISHU	8	   //检测到的最少吹气数
#else
#define	MIN_QISHU	(500/50 /2)	   //   时间 / (50ms/次) /2次测一次  检测到的最少吹气数
#endif

WORD CountHeat=0;
BYTE StateSensor=AL_STATE_SLEEP_CNT;
extern WORD Currrentjiu_level;
extern BYTE DisplayCont;
BYTE  AdjustSensor=0;
BYTE  Insystate=0;//显示系统状态
extern void DIsplaySYSV(WORD hexnum);
void   	Init_sensor()
{
	StateSensor=AL_STATE_EXIT;
	SensorHeatPowerOff();
	SensorPowerOff();
}
BYTE PI_ADJUST_DET()
{  BYTE reval;
   PIO_ADJUST_DET=1;
   P1MDOUT&=~PIO_CFG_DET; //p15 弱开路输出
	DelayXms(1);
   if(PIO_ADJUST_DET==0)reval= 1;
   else reval= 0;

   return 		 reval;

}

//#define	ONLY_RELEASE_PRESS
/*********************************************************
	读键
	重复键及长按处理
100ms call once
**********************************************************/
//#define	ONLY_RELEASE_PRESS
BYTE	Read_Key(void )
{
	static BYTE 	TM_KeyHold=0;
	static   BYTE old_key;
	BYTE temp_key=KEY_NOKEY;

//以下程序100ms执行一次		

    PWR_G2=1; PWR_G1=1;_nop_(); _nop_();_nop_();
	if(PWR_G1==0)	{StateSensor=TEST_INIT;}  //传感器 加热电压加测不到
	if(PWR_G2==0){temp_key=KEY_DISPLAY;}
    if(PWR_G1==0&&PWR_G2==0) {temp_key=KEY_MODE;}

	
       //RELEASE key
    if(temp_key==KEY_NOKEY){	 
#ifdef ONLY_RELEASE_PRESS
       			
 		    if(TM_KeyHold==0)
			{
   				old_key=KEY_NOKEY;
   		    }else 
			{
   		    				
				temp_key=old_key;
				old_key=KEY_NOKEY;
				
			}
#else

			old_key=KEY_NOKEY;
       		TM_KeyHold=0;				

#endif
       				}		
       
       
       //press and hold
         else if(old_key==temp_key){
         				//PRESS LONG TIME KEY  if press TM_KeyHold time ,process continue key,  else no key
#ifdef ONLY_RELEASE_PRESS
         				if(TM_KeyHold!=0)temp_key=KEY_NOKEY;
         				else{
       						if((old_key==KEY_RIGHT)||(old_key==KEY_LEFT)
       						      					
       						){   						
       							
				
       							temp_key=old_key;
       							if(PRE_NEED_3S==0){temp_key=old_key;
       												PRE_NEED_3S=5;//  INC or DEC interval /5*20MS
       									}else	temp_key=KEY_NOKEY;
       									
       									
       							 } else temp_key=KEY_NOKEY;
       				
       			
       		          			
    	  					}
#else
	#define	KEY_HOLD_TIME	10
#if 0
							if(	old_key==KEY_MODE)
							{
									#define	CONKEY_AFTER_FIRST_TIME	7 //按下 700ms后重复
									if(++TM_KeyHold>=CONKEY_AFTER_FIRST_TIME)
									{
										//Continuous key
										TM_KeyHold=CONKEY_AFTER_FIRST_TIME-1;


									} else temp_key= KEY_NOKEY;

							}
						
							else 
							
#endif							
						
						//长按2s为对码
							if(	old_key==KEY_MODE)
							{	   

								temp_key= KEY_NOKEY;
								 TM_KeyHold++;
//							   长按 键转换为其他按键
								   if(TM_KeyHold==KEY_HOLD_TIME)
									{
										//为显示标定值
										temp_key=KEY_DISPSTD;

									}
									else if(TM_KeyHold==KEY_HOLD_TIME*5)
									{
										//为其他长按实现的功能
										temp_key=KEY_RANCODE;

									}
									
								
									else if(TM_KeyHold>KEY_HOLD_TIME*6)
									{
										TM_KeyHold=KEY_HOLD_TIME*6+1;
										

									}

							}
						
							
							else temp_key= KEY_NOKEY;

#endif
                  }
       //first press 释放后按下       			  		
          else {
#ifdef ONLY_RELEASE_PRESS
		  	
			old_key=temp_key;
			temp_key= KEY_NOKEY;
       		TM_KeyHold=COUNT_500_MSEC;
#else
		  		
			
				old_key=temp_key;
				TM_KeyHold=0;
				
		
#endif
          
       		}


return temp_key;       		
       
}

 //***************************************************************
//*			process the IR  key and the scan key  				*
//*		普通扫描						*
//***************************************************************
BYTE  ScanKey(void)
{	 
	BYTE scan_key;
	scan_key= Read_Key()	;
	return scan_key;
	
}


#define StepUpSound()
#define StepDownSound()
extern void Oscillator_LowCost();
extern void Oscillator_HiCost()  ;
void GoSleep()
{
BYTE IEx,IEy;
//仅允许按键中断，

Insystate=0;
			
SensorHeatPowerOff();
SensorPowerOff() ;
#define	DALIOUT				P15
DALIOUT=0;						   //关发射
PIO_CHARG=0;			//关检测电池
PO_LCD_POWER(P_LCD_OFF);
LCDBAKLIGHT(LCDBAK_OFF);
{
extern void LCD_Sleep(void)	;

LCD_Sleep();

 }
LED_GREEN(LED_OFF);
LED_RED(LED_OFF);

IEx=IE;
IEy= EIE1;
EA=0;
IE=0x05; //允许IT0  IT1
EIE1=0;
TCON|=	0x05;//边沿触发
TCON&=	~0x0A;//清中断标志
IT01CF    = 0x41; //低电平	p01,p04
AD0EN=0;		  //ad off
REF0CN=0x80; //关比偏压，温度传
Oscillator_LowCost();
EA=1;				//开中断
PCON |= 0x01  ;  	//    将 IDLE 位置 1 
_nop_()	 ;
_nop_()	 ;
_nop_()	 ;
_nop_()	 ;
_nop_()	 ;
_nop_()	 ;

//
EA=0;				//关中断
Oscillator_HiCost();

//重新打开 VREF
{
extern Voltage_Reference_Init();
Voltage_Reference_Init();
}
AD0EN=1;
EIE1=IEy;
IE=IEx;


PO_LCD_POWER(P_LCD_ON);
PIO_CHARG=1; //开检测为输入口
EA=1;

}

  

//WORD  VS,VP;
WORD  VA,VB,VC;		   //va :冷制最低电压 VB：吹气前电压  VC：出气后电压 

//返回 0，电池电压低
//
//4.0 3.9 3.8 3.7V  3.6
// 满量程(3V)量化：      1024/3    *  56K/(56K+56K)  >>2  (转为字节
//IO 2.5v  
#define	BATVREF	 ((BYTE) ((DWORD)1024/3*56/(56+56) /4) )
#define	BATV1	(BYTE)((WORD)BATVREF*40/10) 
#define	BATV2	(BYTE)((WORD)BATVREF*39/10) 
#define	BATV3	(BYTE)((WORD)BATVREF*38/10) 
#define	BATV4	(BYTE)((WORD)BATVREF*37/10) 
#define	BATV5	(BYTE)((WORD)BATVREF*36/10) 

BYTE BatCnt=0;	//电量级数
void BatteryEnegyDetect()
{	

	return;
}

#if 0
//返回 0，电池电压低
BOOL SensorPowerDetect()
{   BYTE i;
	WORD tempv;
   	tempv=Force_MesureADport(ADPORT_POWER);
	 	//传感器电源电压低报警
	   if(tempv<NORMAL_SESOR_POWER_VAL)		   //1.5v
		{	
			DisBuffer[0]=BH_CHAR_L;
			DisBuffer[0]=CHAR_DASH;

			Display_All();
		   //红灯绿灯一起闪，发警报声
		   for(i=0;i<3;i++)
		   {
		   	LED_RED(1);
		   	LED_GREEN(1);
		  	DelayXms(120);
		  	LED_RED(0);
		   	LED_GREEN(0)	;
			DelayXms(120);
	
		   }  
		   SensorHeatPowerOff();
		   return  0;
		 }
	
	return 1;

}

#endif
#define	SENDBUF_MAX		10
BYTE SendCirBuf[SENDBUF_MAX];
BYTE SendIdxS=0;
BYTE SendidxP=0;

//发送的数据先到缓冲区，时间空闲再发送，这样避免发送时等待
void SendCir(BYTE senddata)
{
 SendCirBuf[SendIdxS]= senddata;
 SendIdxS++;
 if( SendIdxS==(SENDBUF_MAX))SendIdxS=0;

}
extern BYTE f_busy;

//循环调用，只要发送空闲，取出发送缓冲区的数据发送
void SendCycbuf()
{
 if( f_busy)return;

 if( SendidxP!=SendIdxS )
 {
 	
 	DALI_Send(SendCirBuf[SendidxP]);
	SendidxP++;
	if(SendidxP==SENDBUF_MAX)SendidxP=0;
 }


}

//仅将数据发到缓冲区
void  	SendRF(BYTE KEYFUN)
{	  BYTE i;
	
		for(i=0;i<3;i++)
			{
	    	//DALI_Send(KEYFUN );
			//DelayXms(100);

			SendCir(KEYFUN);
			}


}

//传感器放置久后吹气，可能失准，需补偿校准
#define	VCC_HEAT	(2.5*1024/3)		//mv25
BOOL Fcold=0;
BOOL	ColdTest()
{
	  float	temp1;
	  float	temp2;
	 
	 WORD k;
	 temp1=((float) VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) ;
	 temp2=((float)VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
	 k=(WORD)temp1/temp2;
	 if(k>9)
	 {
	 return 1;
	
	 }
	 return 0;

}
WORD  ColdCompsation(WORD nppm)
{//	 float	temp1;
//	 float	temp2;
	 static   BYTE i=0;
//	 WORD k;
//	 temp1=((float) VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) ;
//	 temp2=((float)VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
//	 k=(WORD)temp1/temp2;
#if 1
if(ColdTest()==1)
//	 if(k>9)
	 { 
	 		//nppm=nppm*10/7; 
	 		i=1;
		}	  //10/6
	 else {
	 		i++;
		   if( i==1){ i=0;}
		   else if( i<=3){ 
		   //nppm=nppm*10/8;
		   if(VC>=VB)nppm=0;
		   }
		   else if( i==4){
		   // nppm=nppm*10/9;  
		   if(VC>=VB)nppm=0;
		   i=0;}
		   
	       }
	   if(Fcold)
	   {

		Fcold=0;
		if(VC>=VB)nppm=0;
	   }
#else
if(ColdTest()==1)
//	 if(k>9)
	 { nppm=nppm*10/7; i=1;}	  //10/6
	 else {
	 		i++;
		   if( i==1){ i=0;}
		   else if( i<=3){ nppm=nppm*10/8;}
		   else if( i==4){ nppm=nppm*10/9;i=0;}
		   
	       }

#endif

	  return nppm;

}

//输入  ，mg/100ml 血液
BYTE CalALCLevel(WORD untmg)
{
   	WORD temp2=untmg;
   	BYTE   	jiu_level=0;
	if(temp2>=80) jiu_level=3;
	else if (temp2>=40) jiu_level=2;
	else if (temp2>=18) jiu_level=1;
   	return  jiu_level;
}
#define 	STD_ADJUST_PPM		130
//#define 	STD_ADJUST_mg		50
//y=k* dx -<  130=k* dv  ->k=130/dv  ->y=130/dv *dx
//计算酒精看量
void 	calALO()
{

}


#define		HEAT_TIME_MAX	35


//测量电压 在VS VP中
//酒精头100hz方波时，5ms执行一次 ,主程序调用
//如酒精头为直流，则50ms调用一次
#define		QITI_ON()		 SensorPowerOn()
#define		QITI_OFF()		 SensorPowerOff()


#define		KAIGUAN_ON()		P15=1
#define		KAIGUAN_OFF()		P15=0


void TestACHOL()
{
//	BYTE i;
	WORD tempv;
	
	
	switch(StateSensor)
	{
	default:  
		 case	TEST_INIT:
		 		StateSensor=TEST_INIT	;

		 		VB=0;
				VC=0x3ff;
				VA=0x3ff;				   
			
				CountHeat=0;		  //加热10秒 
			
				Insystate=0;


				QITI_OFF();
				KAIGUAN_OFF();

		  		DisplayCont= DISPLAY_TIME_SEC	  ;
				Display_All();
	
  			    StateSensor++;
				qishu=0;
			
				
		 break;
		 
		  #define	TM_QITI_POINT	 220
		  #define	TM_KAIGUAN_POINT	(TM_QITI_POINT+20) 
		  #define	TM_OVER_POINT	(TM_KAIGUAN_POINT+30)
		 case	TEST_A:
		if( CountHeat>=TM_OVER_POINT)
		{
			KAIGUAN_OFF()	;
			QITI_OFF();
		//	DisplayCont=DISPLAY_ERR2  ;
		   DisplayCont= DISPLAY_TIME_SEC	  ;
			
		}
		 else if( CountHeat>=TM_KAIGUAN_POINT)
		{
			KAIGUAN_ON()	;
			if(F_half_sec==0)DisplayCont=DISPLAY_ERR0  ;
			//Display_All();
			//DelayXms(200);
		else	DisplayCont=DISPLAY_TIME_SEC  ;
			
		}	
		 else if(CountHeat>=TM_QITI_POINT)
		{ if(F_half_sec==0)	DisplayCont=DISPLAY_ERR1  ;
		//	Display_All();
	//		DelayXms(200);
			else DisplayCont=DISPLAY_TIME_SEC  ;
			QITI_ON();

		}
		
		else   
 	    

	break;

	 }
}


void  MeasurePS()
{
	StateSensor=TEST_INIT;
	}

//校准酒精	灵敏度 	V2=	0.05%BAC测量电压，
//未通气电压 		V1
//DV1= V1-V2

//实际测量时
//V3 吹气时电压  
//V4:未吹气时电压
//DV2=V4-V3

void  DO_Key_Action()
{	
   BYTE newkey;
    if(NEW_KEY==KEY_NOKEY)NEW_KEY=ScanKey();

	if(NEW_KEY==KEY_NOKEY)return;
	newkey=	NEW_KEY;
	NEW_KEY=KEY_NOKEY;
	
	Count_5S=5;

	switch(newkey)
	{
	case	KEY_RANCODE:
			{
			extern _SYSINFO SysInfo	;
			WORD  tmpran	;
			DisplayCont= DISPLAY_NUM   ;
			
			tmpran=rand();
			SysInfo.TRANCODE=tmpran;
			SysInfo.TRANCODE=SysInfo.TRANCODE<<12;
			tmpran=rand()&0x0fff;
			SysInfo.TRANCODE|= tmpran;

#ifdef	DEBUG_BYLCD
			DisBuffer[0]= (SysInfo.TRANCODE>>12)&0x0f;
			DisBuffer[1]= (SysInfo.TRANCODE>>8)&0x0f;
			DisBuffer[2]= (SysInfo.TRANCODE>>4)&0x0f;
			DisBuffer[3]= SysInfo.TRANCODE&0x0f;
			Display_All();
#endif
			TransCmd=FUN_MATCH_CODE; //对码命令
			SendRF(TransCmd)  ;
			{
			extern WORD CalChkSum(BYTE *p,BYTE len);
			SysInfo.chksum=CalChkSum((BYTE*)&SysInfo,  EEP_SYS_LENTH);
			}
			FLASH_Update((FLADDR)EEP_SYS_TBLSTART,(BYTE*)&SysInfo,EEP_SYS_LENTH);
			StateSensor=AL_STATE_SLEEP_CNT;
			}  
	break;
		

	case	KEY_TESTRF:
#if 0 //def TEST_FACTORY
	{ static BYTE test1=0; 
	    BYTE tmpcode;		
		test1++;
		if(test1==4)test1=0;
		if(test1==0)	tmpcode=FUN_LOCK_CAR;
		else if(test1==1)	tmpcode=FUN_UNLOCK_CAR;
		else if(test1==2)	tmpcode=FUN_DETECT_CAR;
		else if(test1==3)	tmpcode=FUN_CHAOSHI_CAR;
		else  tmpcode=FUN_DETECT_CAR;

		
		DisBuffer[3]= test1;
		DisplayCont=DISPLAY_MATSEND ;
		Display_All();
		DALI_Send(tmpcode);
		StateSensor=AL_STATE_SLEEP_CNT;
	
   }
#endif
		break;

	case	KEY_JIUJING:
     
		DisplayCont=DISPLAY_TSTING;
		AdjustSensor=0;
		goto LABEL_TEST_ALO	;
	

	break;
	case	KEY_ADJUST:
LABEL_ADJUST_ALO:
	AdjustSensor=1;
	DisplayCont=DISPLAY_ADJ;
LABEL_TEST_ALO	:

	Display_All();
	MeasurePS();
	break;

	case KEY_DISPSTD:
  {	 
		extern void  	diaplay_std ();
	 	diaplay_std ();
  }
	break;
	
	
	//显示 
	case	KEY_DISPLAY:	
	if(Insystate==0)
	{
		if(DisplayCont==DISPLAY_ALO){
			//Jdispidx++;
			//if(Jdispidx>=MAX_JIULST)Jdispidx=0;			
			
			//回看功能
			if(Jdispidx!=0)Jdispidx--;
			else 	Jdispidx= MAX_JIULST-1 ;

		  //向看
		//	Jdispidx++;
		//	if(Jdispidx>=MAX_JIULST)Jdispidx=0;
		}
		
		DisplayCont= DISPLAY_ALO	  ;
	}else

	{
			DisplayCont++;
			if(DisplayCont>DISPLAY_SYS_VC
			||DisplayCont<DISPLAY_SYS_RAN)DisplayCont=DISPLAY_SYS_RAN;
	
	}
	StateSensor=AL_STATE_SLEEP_CNT;

	break;
	default:
//		KEY_PRINTF(printf("-othkey-"));
		break;

	}
	  

}










