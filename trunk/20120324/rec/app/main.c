////////////////////////////////////////////////////////////////////////////
///////////////
// 文件名：
// 创建者：
// 创建时间：
// 内容描述：
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
WORD ConONTimer=0;    		  //20分钟开及 30秒计时器关
BYTE PowerbeingOn=0;
BYTE PowerONcnt=0;
BYTE PowerOFFcnt=0;
//0 
//1 吹通过
//2 吹起有酒
BYTE PWR_State=0;  
_SYSINFO SysInfo;
BYTE F_demo=1;
	extern BYTE   yuyin_time;
			extern void   SetYuyin(BYTE yuin);


//第一次装即通电，语音提示
//ACC on 后 2小时提醒  3小时提醒 ，以后每30分钟提醒一次，
//停车（继电器断开）状态 转到acc on 15分钟可以开自动接通继电器，可以打火
//开车（继电器接通）状态，  转到 acc off，设定20分后将断开继电器，进入停车态
//如果ACC OFF时 ，吹气有酒状态	，则15分钟后才开；
//如果ACC ON状态，吹气有酒，保持行车状态。
//吹气无酒状态,立即接通继电器 , 5分钟内可以发动；如果5分钟后没打火，则断开继电器。


//2012.08 录像
//打开录制设备： 检测到RF检测命令或 检测到打火开关 录制 
//停止录制： 点火开关断开  5分钟后停止 

//对吗 先对a mat1

////////////////////////////////////////////////////////////////////////////
///////////////
// 函数名：
// 编写者：
// 参考资料：
// 功 能：
// 输入参数：
// 输出参数：
// 备 注：
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

	//初始数据
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

//每500毫秒调用一次
void Task_500ms()
{


	
}





//每50毫秒调用一次
void Task_50ms()
{


}
BYTE ACCstate=0;	 //检测到得ACC 状态
BYTE relay_state=0; // 继电器断电状态  ！；接通  0：断开
WORD RELAYTobeOFFTime=0;//  acc off 将要断开的时间计数 秒，20分 归零后将断继电器
WORD RelayToBeONTime=0;    //acc on 将要接通的时间计数 秒，5分钟 后将接通继电器
WORD ACCOnTimeCnt=0;  //acc on的时间
BYTE Mstate=0;//0:




void InitRelay()
{
	
   	relay_state=0;
	POWER_OFF();
	RELAYTobeOFFTime=0;
	RelayToBeONTime=0;
//	Mstate=0;
	YUYIN_NOYUYIN();
	Mstate=1;//上电开车
	TctoberecStop=0;
	REC_STOP();
}
void ACC_State()
{
 static char ACCdetectTC=0; 
 //检测acc状态 
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

//每秒调用一次
//2小时提醒  3小时提醒 ，以后每30分钟提醒一次
void yuyintixing()
{
WORD  tmp;
BOOL chaoshi=0;
	if(sys_time.Time_1_sec%20!=1)return; //每20秒提醒一次

	//语音提醒
	 {

		if(ACCOnTimeCnt==2*60||(ACCOnTimeCnt==3*60 ))
		{
					
			chaoshi=1;	
		}
		else if	(ACCOnTimeCnt> 3*60 )
		{
		   tmp= ACCOnTimeCnt-3*60;
		   if(tmp%30==0)chaoshi=1; //每30分钟提醒
		}
	 }
	// else ACCOnTimeCnt=0;
	 if(chaoshi)SetYuyin(YUYINT_CHAOSHI);
}
//一分钟计算一次
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

//第一次上电进入开车状态，无需吹气就可打火，（检测到acc on 继电器就吸合）以便安装
//设计要求,没有吹的时候开车,会跳继电器,开不了
  //1 吹了以后,没有酒精,在5分钟以内都可以开车,不会断跳继电器

  //在3分钟以外需要重新再吹
  //2.吹了以后有酒精,在15分钟以内开不了车,会跳继电器
  //15分钟以后,可以开车（改为5分钟）
  //停车20分钟以后,不能开车,会跳继电器
 //在没有手机的情况下启动车辆5分钟以后可以启动

 //当时不能启动,5分钟以后,将可以启动
  
   //在ACC ON时,state==1
   //ACC OFF时,20分钟以后,state==0,不能开车
 // delay_ms(6);
  
  //for(i=0;i<100;i++)//在这里做电平宽度检测
	//      {
   //     delay_ms(10);
     //    }
  //state==0,停车超过10分钟的状态
  //state==1,开车的状态
    //state==2,收到有酒精的状态
	//state=3;//  汽车无法发动的状态，需要用
  //遥控器吹起,才能发动,疲劳驾驶
//Zzhuangtai
//ACC ON： 有酒：无动作    无酒： 接通  
//ACC OFF：有酒： 断开     无酒： 先接通，5分钟后无ACCon断开
//ACON-ACoff    20分钟以后断开
//ACoff_ACCON   保持ACCon 15分钟后接通		  状态，



/*

  1.吹气有酒，15分钟可以开
  2.吹气无酒，5分钟内都可以开 
  3.开车后，转到ACC OFF，20分钟内不吹直接开
  4.停车跳闸状态，（比如熄火20分钟后），转到ACC ON，需要吹气无酒时可开，或15分钟后直接开
  5.开车（ACCON） 状态，吹气有酒无酒都不熄火
  6.开车2小时语音提醒，3小时后，每30分钟提醒一次


*/
void TaskRelay()
{
 
  


	switch(Mstate)
	{

	 
		
//跳闸停车状态 
//到acc on 15分钟
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

//开车状态
// 转到 acc off，设定20分后断电
		case	1:
		if(	ACCstate==1) //
		{				
			POWER_ON();		//一直开
			RelayToBeONTime=0;	
		}else 
		{
		
			 RELAYTobeOFFTime=20*60;
			 Mstate=2;
			
		}
		break;


		//准备停车状态				accoff-》accON
		case	2:
		if(ACCstate==0)
		{
				//准备断电
				if(	RELAYTobeOFFTime)	
				{ RELAYTobeOFFTime--;
				if(RELAYTobeOFFTime==0) {	Mstate=0;}
				}
		}
		else
		 Mstate=1;
		


		break;

		//准备开车状态
		case	3:
		if(ACCstate==1)
		{
				//准备开
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


		//吹气有酒状态	  ， 15分钟后才开
		case	4:
		if(	ACCstate==0)
		{
			POWER_OFF();
			RelayToBeONTime=15*60;
		    Mstate=3;
		}else
		{
		  if(relay_state==1)Mstate=1;//依旧进入开车状态
		  else 	Mstate=0;			 //有酒且继电器还没吸合，进入关断状态


		}
		 
	
		break;


			//吹气无酒状态,立即闭合电源	 ,并进入5分钟停车准备（如果没有发动）
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

//防止干扰
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
// 函数名：
// 编写者：
// 参考资料：
// 功 能：
// 输入参数：
// 输出参数：
// 备 注：
////////////////////////////////////////////////////////////////////////////
///////////////
void main (void)
{	

	PCA0MD &= ~0x40;                    // Disable Watchdog timer
	Init_VAR();
	

	Init_Device();
	Init_patch();
	REC_STOP();
    P2MDOUT|=0x01; //推挽
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
				if(sys_time.Time_1_20msec>80)LED_RED(LED_OFF); //快闪
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
			 		LED_RED(LED_OFF);;	//熄灭
					KMATCH=0;
				 }
			  
			}
		
				
		}
			
			
		}

		
		if(F_1000ms)
		{	
		
			F_1000ms=0;
			yuyintixing();
			if(KTMOUT==0)TaskRelay();	   //非对码状态
			if(yuyin_time)
			{
			 yuyin_time--;
			 if(yuyin_time==0)SetYuyin(YUYINT_NOYUYIN)	;

			}
 
 //录制时间
			if(ACCstate){TctoberecStop=360;}
			
			if(TctoberecStop!=0)
			{
			REC_START();  //只要停止时间未到，继续录制
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
