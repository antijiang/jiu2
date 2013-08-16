
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

BYTE CountHeat=0;
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
#define	TEST_FACTORY   	//工厂测试
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
#if defined TEST_FACTORY
		if(PWR_G1==0){temp_key=KEY_TESTRF;}
#else
	if(PWR_G1==0){temp_key=KEY_JIUJING;}
#endif
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

 #if defined TEST_FACTORY
		{temp_key=KEY_TESTRF;}
#endif
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
	if(StateSensor<=AL_STATE_EXIT) return KEY_NOKEY;     
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
	WORD tempv;
	BYTE batv;
static	BYTE LowCnt=0; 
   	tempv=Force_MesureADport(ADPORT_BAT);	
	batv= (tempv>>2)&0xff;
   	if(batv>=BATV1){BatCnt=4;}		  //最大电量
	else if(batv>=BATV2){BatCnt=3;}
	else if(batv>=BATV3){BatCnt=2;}
	else if(batv>=BATV4){BatCnt=1;}
	else  BatCnt=0;			//无电闪烁外框
	if(PCHARG_DET())LowCnt=0;
	if(batv<BATV5){
					if(LowCnt++>30)		  //
					{ StateSensor=AL_STATE_SLEEP_CNT;
					  DisplayCont=DISPLAY_TSTING;
					  GoSleep();
					  Display_All();
		    		  DelayXms(100);
					}

			
				  }
	else  LowCnt=0;
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
//	 if(VCC_HEAT<SysInfo.VB)VCC_HEAT   =
	 //k=( VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) /(VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
	 temp1=((float) VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) ;
	 temp2=((float)VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
	 k=(WORD)temp1/temp2;
	 if(k>9)
	 {
	 return 0;
	
	 }
	 return 1;

}
WORD  ColdCompsation(WORD nppm)
{	 float	temp1;
	 float	temp2;
	 static   BYTE i=0;
	 WORD k;
//	 if(VCC_HEAT<SysInfo.VB)VCC_HEAT   =
	 //k=( VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) /(VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
	 temp1=((float) VCC_HEAT/(VCC_HEAT-SysInfo.VB) -1) ;
	 temp2=((float)VCC_HEAT/(VCC_HEAT-VA)-1)	  ;
	 k=(WORD)temp1/temp2;
#if 1
	 if(k>9){ 
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
	 if(k>9){ nppm=nppm*10/7; i=1;}	  //10/6
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
BYTE i;
int32 temp1;//,temp2 ;
WORD temp3;
WORD temp2;
BYTE jiu_level;
	if(qishu<MIN_QISHU)
	{
		 DisplayCont=DISPLAY_ERR1 ;
		 return;

	}
	if(SysInfo.VB<VC)VC=SysInfo.VB;
	
	temp1=(int32)SysInfo.VB-VC;		   //实际测量差压
#ifndef	 FANGBO_PWR
	//temp1=temp1+ ((int32)VB-(int32)SysInfo.VB)/3;
#endif
	temp3=SysInfo.VB-SysInfo.VC;  //标定电压 

	 //50/log(DV)=k
//	temp2=(WORD)((float )50/log(temp3)*log((float)temp1/temp3));

	temp1=(WORD)((DWORD)STD_ADJUST_PPM*temp1/temp3);	//计算PPM
//	temp1=(WORD)((DWORD)STD_ADJUST_mg*temp1/temp3);

	Currrentjiu_level=0;
  	temp2=(WORD)(temp1*10/26);	//PPM unit convert to  unit  mg/100ml
#ifndef	 FANGBO_PWR
	temp2=ColdCompsation( temp2);
#endif
  
//
//log对数较准系数
		if(temp2<=35) temp2=temp2*7/10;
		else if(temp2<=40) temp2=temp2*8/10;
		else if(temp2<=45) temp2=temp2*95/100;
		else if(temp2<=56) temp2=temp2;
		else if(temp2<=66) temp2=temp2*105/10;
		else if(temp2<=100) temp2=temp2*12/10;
		else  temp2=temp2*13/10;
	
	Currrentjiu_level=temp2;
	jiu_level=CalALCLevel(temp2) ;
   /*
	if(	temp1>MAX_ALCOHOLICITY3)	jiu_level=3;
	else if(	temp1>MAX_ALCOHOLICITY2)	jiu_level=2;
	else if(	temp1>MAX_ALCOHOLICITY1)	jiu_level=1;
  */

	//显示酒精量
	DisplayCont=DISPLAY_ONOFF ;
//	HexToBCD((WORD * )&temp1,(BYTE  *)&DisBuffer)	 ;
//	Display_All();
//	DisplayCont=DISPLAY_ALO ;
		if(jiu_level!=0)
		{
			 //酒精报警
			LED_RED(LED_ON);	  	 ;//开红灯
			LED_GREEN(LED_OFF);	  ;//关绿灯
			DisBuffer[0]=CHAR_BLK;
		    DisBuffer[1]=CHAR_o;
			DisBuffer[2]=CHAR_F;
			DisBuffer[3]=CHAR_F;
			Display_All();
			for(i=0;i<3;i++)//有酒精报警
	          {
	     		StepDownSound();
				BUZZY_ON();
	     		DelayXms(1000);
				BUZZY_OFF();
				DelayXms(50);
	  		   }
			 SendRF(FUN_LOCK_CAR);//发送锁车信息;
	     
	    	LED_RED(LED_OFF);
			Jiulist.idx++;
			
			if(Jiulist.idx>=MAX_JIULST)
			{
				Jiulist.idx=0;
				
			}
			Jiulist.Jvol[Jiulist.idx]=Currrentjiu_level;	  //保存酒精mg
		//	if(	Jiulist.idx%5==0)
			FLASH_Update((FLADDR)EEP_JIULST_TBLSTART,(BYTE*)&Jiulist,EEP_JIULST_LENTH);
			Jdispidx=Jiulist.idx;
			DisplayCont=DISPLAY_ALO ;
		}
    
   
		else
		{
			DisBuffer[0]=CHAR_BLK;
		    DisBuffer[1]=CHAR_BLK;
			DisBuffer[2]=CHAR_o;
			DisBuffer[3]=CHAR_n;
			  	 //无酒精
			LED_RED(LED_OFF);	  	 ;//开红灯
			LED_GREEN(LED_ON);	  ;//关绿灯
			Display_All();
			for(i=0;i<2;i++)//
	          {
	     		    StepUpSound();
					BUZZY_ON();
	    			DelayXms(50);
					BUZZY_OFF();
	     			DelayXms(50);
	  		   }
	     	SendRF(FUN_UNLOCK_CAR);		  //发送开车信息
	    	LED_GREEN(LED_OFF);
			


		}
			Display_All();
}
#ifdef  FANGBO_PWR
#define		HEAT_TIME_MAX	10
#else
#define		HEAT_TIME_MAX	20
#endif

//测量电压 在VS VP中
//酒精头100hz方波时，5ms执行一次 ,主程序调用
//如酒精头为直流，则50ms调用一次
void TestACHOL()
{
//	BYTE i;
	WORD tempv;
	
		
	/*	if(PCHARG_DET())
		{	//充电
			if(StateSensor ==  (AL_STATE_SLEEP_CNT+1))CountHeat=20;
		}
		*/
	
	switch(StateSensor)
	{
		 case	TEST_INIT:	
		 		BatteryEnegyDetect();
		 		VB=0;
				VC=0x3ff;
				VA=0x3ff;				   
				LED_RED(LED_ON);
				SensorHeatPowerOff();
				SensorPowerOn(); //接传感器测量电压
			
				CountHeat=HEAT_TIME_MAX;		  //加热10秒 
				#ifdef SECURE_SPI
				{  	  
				extern BYTE F_demo;
				if(F_demo)CountHeat=5;
				}
				#endif
				Insystate=0;

				
#if 0			
//测量R0的电阻，（通电不加热的电压）
				DelayXms(100);	
				tempv=Force_MesureADport(ADPORT_SENSOR);
			 	VA=min(	tempv,VA);
				//tempv=(WORD)((DWORD)VA*(1000*3)/1024);  //显示mV
			     //HexToBCD((WORD * )&tempv,(BYTE  *)&DisBuffer)	 ;
				 DIsplaySYSV(VA) ;
				DisplayCont= DISPLAY_VOLT	  ;
			
#else
				
		  		DisplayCont= DISPLAY_TIME_SEC	  ;
#endif
				SensorHeatPowerOn();

				if(AdjustSensor==1)DisplayCont= DISPLAY_ADJ;
				Display_All();
				SendRF(FUN_DETECT_CAR);

			//	Currrentjiu_level=0;	   2011.0711
  			    StateSensor++;
				qishu=0;
			
				
		 break;
		 case	TEST_A:
		
		if(AdjustSensor==1)
		{
		   //保持2秒 显示DISPLAY_ADJ   AAA代表进入校准 ，后显示秒
		   if(CountHeat< (HEAT_TIME_MAX-2))DisplayCont=   DISPLAY_TIME_SEC;
		
		}
		/*else 
		{
			DisplayCont=   DISPLAY_TIME_SEC;

		}
		*/
//测量传感器 上电最初的3秒最低下冲 值
 #ifndef	   FANGBO_PWR
 if(CountHeat>(HEAT_TIME_MAX-4))
 	{
			tempv=Force_MesureADport(ADPORT_SENSOR);
			VA=min(	tempv,VA);
		//	tempv=(WORD)((DWORD)VA*(1000*3)/1024);  //显示mV
		   // HexToBCD((WORD * )&tempv,(BYTE  *)&DisBuffer)	 ;
		    DIsplaySYSV(VA) ;		//实际没显示
			//显示下充值
			if(AdjustSensor==1)
			{
				 DisplayCont= DISPLAY_VOLT	  ;
				 Display_All();

			}else
			{
				#ifdef	DEBUG_VOL
				DisplayCont= DISPLAY_VOLT	  ;
				Display_All();
				#endif
				if((Fcold==0)	)
				{ 
				  if(0==ColdTest())	 
				  {	Fcold=1;
				  	CountHeat=60;
   				  	StateSensor=TEST_REP;
				  }
			
				} 
			}
				//
				
			
    } else  StateSensor++;
#endif

		 break;

		case TEST_REP:
		if(CountHeat>5)
		{
		 //等待时间	
		}
		else
		StateSensor++;
		BatteryEnegyDetect();
		break;
		 case	TEST_100_PREV:
		 if( CountHeat<HEAT_TIME_MAX-2)
		 
		
		 if(CountHeat==3)
		 {
		 	StateSensor++;	  //等待10秒 后，100hz 3秒
#if 0		 
		 	 if(AdjustSensor==1)
			 {
		 	//测量方波前，加热后的 漂移电压
			SensorHeatPowerOff();
			tempv=Force_MesureADport(ADPORT_SENSOR);
		 	VA=min(	tempv,VA);
			//tempv=(WORD)((DWORD)VA*(1000*3)/1024);  //显示mV
			//HexToBCD((WORD * )&tempv,(BYTE  *)&DisBuffer)	 ;
			 DIsplaySYSV(VA) ;
		
			DisplayCont= DISPLAY_VOLT	  ;
	  		Display_All();



			}
#endif
		 } else 
		 {

		 BatteryEnegyDetect();

		 }
		 break;




//***************100hz 3秒
//********************************************
		 case	TEST_100HZ_L3:
#ifdef  FANGBO_PWR
		 SensorHeatPowerOff();	  //100KZ	 3秒
#endif
		 StateSensor++ ;
		 if(CountHeat<=1)
		 {
		//方波结束前 测量方波后的0 PPM 校准电压 
	  	tempv=Force_MesureADport(ADPORT_SENSOR);
	 	VB=max(	tempv,VB);
		if(AdjustSensor==1)
 		{
	//	tempv=(WORD)((DWORD)VB*(1000*3)/1024);  //显示mV
	//	HexToBCD((WORD * )&tempv,(BYTE  *)&DisBuffer)	 ;
	    DIsplaySYSV(VB) ;
		DisplayCont= DISPLAY_VOLT	  ;
  		Display_All();

		}
	
		else
		{
#ifdef	DEBUG_VOL
		DisplayCont= DISPLAY_VOLT	  ;
  		Display_All();
#endif

		}


		}

		
		 break;
		 case	TEST_100HZ_H3:
		 		   //100hz 
		 SensorHeatPowerOn();
		 if(CountHeat!=0) 
		 {
		 	StateSensor--;
		
		 }
		 else {
		 		LED_GREEN(LED_ON);		   //等待加热完成后，绿灯蜂鸣器提醒吹气
				BUZZY_ON();
				DelayXms(100);
				BUZZY_OFF();
				CountHeat=9;//等待9秒 吹气
		 		StateSensor++;
				}

		 break;
//*************************************************************





//***************100hz 等待吹气  ，最多  至8秒 
//****************************************************
		 case	TEST_WAIT_BLOW_L:	  //等待吹气
#ifdef  FANGBO_PWR
		 SensorHeatPowerOff();	  //100KZ 
#endif
//		 if(sys_time.Time_1_20msec%25==0)LED_GREEN(LED_ON);
//		 else if(sys_time.Time_1_20msec%25==(25/2))   LED_GREEN(LED_OFF);
		      
		
		 StateSensor++ ;
		 break;
		 case	TEST_WAIT_BLOW_H:	  //等待吹气
		 SensorHeatPowerOn();	 //100KZ
		 if(CountHeat!=0)			//仍处等待时间
		 {
		 		if(SW_DETECT()==0)qishu++;
		 		 if(qishu>(MIN_QISHU))
				 { //检测吹气体到，退出
		 	  	 
				  BUZZY_ON() ;//开蜂鸣
				  DelayXms(200);
				  BUZZY_OFF() ;
				  StateSensor+=1;  //直接去测量
#ifdef	FANGBO_PWR
				CountHeat=4;//测量3秒最低值
#else

				CountHeat=2;//zhiliu
#endif
				  }	else 	 StateSensor--;
		}
		else
		{
		 		//一直没检测到吹气体	， 等待结束
		 		StateSensor=AL_STATE_EXIT;			   //直接退出
				DisplayCont=DISPLAY_ERR1 ;
				
				
		}
		break;
//****************************************************		 
		
		   
//***************测量电压***************     
//****************************************************
//测量电压
		 case	TEST_MEASURE_H:
		 //测量
		 SensorHeatPowerOn();
		
		 	  //100KZ
		 StateSensor++ ;
		 break;

		//测量
		 case	TEST_MEASURE_L:
#ifdef  FANGBO_PWR
		 SensorHeatPowerOff();
#endif
		 if(CountHeat!=0)StateSensor--;
		 else StateSensor=AL_STATE_CAL;
	 
	//	 if(F_half_sec)	LED_GREEN(LED_OFF);else LED_GREEN(LED_ON);
#ifdef  FANGBO_PWR
            if(CountHeat<=1) 
#else	
			if(CountHeat<=4)
#endif
			{
			
			//测量酒精电压
			tempv=Force_MesureADport(ADPORT_SENSOR);
			VC=min(	tempv,VC);
		//	tempv=(WORD)((DWORD)VC*(1000*3)/1024);  //显示mV
		//	HexToBCD((WORD * )&tempv,(BYTE  *)&DisBuffer)	 ;
		 DIsplaySYSV(VC) ;

			if(AdjustSensor==1)
		    {	DisplayCont= DISPLAY_VOLT	  ;
				Display_All();	
		    
			}
			
			else
			{
#ifdef	DEBUG_VOL
			 DisplayCont= DISPLAY_VOLT	  ;
			 Display_All();	
#endif
			}

			}
		 
		 break;
//****************************************************************


		 
		 // 计算状态
		 case	AL_STATE_CAL:
		 SensorHeatPowerOff();
		 SensorPowerOff() ;
		 if(AdjustSensor==0){
		 
		 calALO();

		 }
		 else 		
		 {
		 
			if(
			(VC+(0.3*1024/3))>VB	  //标定130PPM 电压要低于 0点0.6 V以上，否则无效
			)
			{
			DisplayCont= DISPLAY_ERR2;
			StateSensor=AL_STATE_EXIT;
			
			return;
			}else 
			{
			//标定正确
			SysInfo.VA=	VA;
			SysInfo.VB=	VB;
			SysInfo.VC=	VC;
			SysInfo.ADJUST=	0x55;
			{
			extern WORD CalChkSum(BYTE *p,BYTE len);

			SysInfo.chksum=CalChkSum((BYTE*)&SysInfo,  EEP_SYS_LENTH);
			}
		 	FLASH_Update((FLADDR)EEP_SYS_TBLSTART,(BYTE*)&SysInfo,EEP_SYS_LENTH);
			DisplayCont= DISPLAY_TSTING;


			}
		
		 }
		 StateSensor=AL_STATE_EXIT;
		 break;


//退出状态
		case	AL_STATE_EXIT:
		
		SensorHeatPowerOff();
		SensorPowerOff() ;

		StateSensor=AL_STATE_SLEEP_CNT;
		BatteryEnegyDetect();
		//if(AdjustSensor==1){if(CountHeat==2)DisplayCont=DISPLAY_TSTING ;   }
		break;


// **************************************
//准备休眠
		case	AL_STATE_SLEEP_CNT:
		   CountHeat=10;
		   StateSensor++;
		   BatteryEnegyDetect();
		   

		break;
		case	AL_STATE_SLEEP_CNT+1:
		if(PCHARG_DET())
		{	//充电
			if((CountHeat==2)||(CountHeat==3))
			{
			Insystate=0;
			CountHeat=20; //不休眠时间
			DisplayCont= DISPLAY_TSTING;
			}
		} 
		else BatteryEnegyDetect();
		if(CountHeat==0)
		{	
			GoSleep();	
			//初始程序必须放在开中断后，延时程序使用了中断
			{
			extern void Init_LCD(void)	 ;
			Init_LCD()	;
			}
			
 			Jdispidx=Jiulist.idx;		 //重新从当前的测量位置烧写
			StateSensor=AL_STATE_SLEEP_CNT;
			DisplayCont= DISPLAY_TSTING;
		

		}
		
		break;
		default:
			BatteryEnegyDetect();
		break;
	}


}


void  MeasurePS()
{
	StateSensor=0;
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
#if defined TEST_FACTORY
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
        if(PI_ADJUST_DET())
		{
		   DelayXms(100);
	  		if(PI_ADJUST_DET())goto LABEL_ADJUST_ALO;
		}
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
	case	KEY_DISPLAY:	
	if(Insystate==0)
	{
		if(DisplayCont==DISPLAY_ALO){
			Jdispidx++;
			if(Jdispidx>=MAX_JIULST)Jdispidx=0;
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










