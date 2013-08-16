
#include "general.h"

#include <stdlib.h>


#define		FUN_MATCH_CODE	0x0a
#define		FUN_LOCK_CAR	0x02
#define		FUN_UNLOCK_CAR	0x03
#define		FUN_DETECT_CAR	0x04
#define		FUN_CHAOSHI_CAR	0x05

extern _SYSINFO SysInfo;
extern BYTE DisBuffer[5]  ;
BYTE KMATCH=0;
BYTE KTMOUT=0;



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
	if(PWR_G1==0){temp_key=KEY_MAT1;}
	else if(PWR_G2==0){temp_key=KEY_MAT2;}
//	if(PWR_G1==0&&	PWR_G2==0) {temp_key=KEY_RANCODE;}

	
       //RELEASE key
    if(temp_key==KEY_NOKEY){	 
#ifdef ONLY_RELEASE_PRESS
       			
   		    			if(TM_KeyHold==0){
   		    				if((old_key==KEY_SEL_UP)||(old_key==KEY_SEL_DN)	)old_key=KEY_NOKEY;
 		    				else {
   		    					temp_key=old_key;
   		    					old_key=KEY_NOKEY;
   		    					}
   		    				
   		    			}else {
   		    				
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
							if(	old_key==KEY_RANCODE)
							{
									#define	CONKEY_AFTER_FIRST_TIME	7 //按下 700ms后重复
									if(++TM_KeyHold>=CONKEY_AFTER_FIRST_TIME)
									{
										//Continuous key
										TM_KeyHold=CONKEY_AFTER_FIRST_TIME-1;


									} else temp_key= KEY_NOKEY;

							}
							#define	KEY_HOLD_TIME	30
							else if(	old_key==KEY_MODE)
							{	   

								temp_key= KEY_NOKEY;
								   TM_KeyHold++;
//							   长按 键转换为其他按键
								   if(TM_KeyHold==KEY_HOLD_TIME)
									{
										//为其他长按实现的功能
										temp_key=KEY_PRESET;

									}else if(TM_KeyHold>KEY_HOLD_TIME)
									{
										TM_KeyHold=KEY_HOLD_TIME+1;
										

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


BYTE yuyin_time;
BYTE yuyin_type;
void SetYuyin(BYTE yuin)
{
extern BYTE F_demo;

 //连发三次 导致反复播放3次  {YUYIN_NOYUYIN();DelayXms(20); }
   if(yuin!=YUYINT_NOYUYIN)
   {
   if(F_demo==1)yuin=  YUYINT_CHAOBIAO;
   }

   yuyin_time=10;
   yuyin_type=yuin;
   if(yuyin_type==YUYINT_DETECT)YUYIN_DETECT() ;
   if(yuyin_type==YUYINT_CHAOBIAO)YUYIN_CHAOBIAO();
   if(yuyin_type==YUYINT_KAICHE)YUYIN_KAICHE();
   if(yuyin_type==YUYINT_CHAOSHI)YUYIN_CHAOSHI();
   if(yuyin_type==YUYINT_NOYUYIN){YUYIN_NOYUYIN();yuyin_time=0;}
	


}

 //***************************************************************
//*			process the IR  key and the scan key  				*
//*		普通扫描						*
//***************************************************************
BYTE  ScanKey(void)
{	
	extern BYTE Mstate;
	extern BYTE  GetIRkey()	  ;
	extern DWORD gbCodeKeep	; 
	BYTE NEW_KEY;
	DWORD rancode;
	WORD cmdkey	;	;
	NEW_KEY= Read_Key()	;
	if(	NEW_KEY!=KEY_NOKEY) return NEW_KEY;
	if(	 GetIRkey()==0)return KEY_NOKEY;
			
	 	rancode=gbCodeKeep>>12;
		cmdkey=gbCodeKeep;
		if((cmdkey&0x0f)!=0x05)return KEY_NOKEY; //固定标志
		if(((cmdkey>>4)&0x0f)!=(((~(cmdkey>>8))&0x0f)))return KEY_NOKEY;  //反码判断
		if(rancode==(0x80000|SysInfo.TRANCODE)||rancode==(0x80000|SysInfo.TRANCODE1))
		{
	//	YUYIN_NOYUYIN();
		//系统码匹配
		cmdkey=	(cmdkey>>8)&0x0F;
			if(cmdkey==FUN_UNLOCK_CAR)
			{
			//开锁
		//	YUYIN_KAICHE();
			SetYuyin(YUYINT_KAICHE);
			LED_RED(LED_ON);
			Mstate=5;
			
			TctoberecStop=360; //录6分钟
			}
			else if (cmdkey==FUN_LOCK_CAR)
			{
			  //不做任何动作
			// YUYIN_CHAOBIAO();
			 SetYuyin(YUYINT_CHAOBIAO);
			 LED_RED(LED_OFF);
			 Mstate=4;
			}
			else  if (cmdkey==FUN_DETECT_CAR)
			{
			  //正在检测
			 //YUYIN_DETECT();
			 SetYuyin(YUYINT_DETECT);
			 TctoberecStop=360; //录6分钟
			}
			else  if (cmdkey==FUN_CHAOSHI_CAR)
			{
			  //正在检测
			 //YUYIN_DETECT();
			 SetYuyin(YUYINT_CHAOSHI);
			 
			}

		}else
		{
			//按了匹配键
		   if(KMATCH!=0)
		   {
		   		NEW_KEY= KEY_NOKEY;	   ;
				if(	((cmdkey>>8)&0x0f)==FUN_MATCH_CODE )
				{
				 //进入对码状态
				if(KMATCH==1)SysInfo.TRANCODE=rancode;		
										  
								
				else if(KMATCH==2)SysInfo.TRANCODE1=rancode;
				LED_RED(LED_ON);	//常亮
				KMATCH=0;
			//	KTMOUT=0;
				FLASH_Update((FLADDR)EEP_SYS_TBLSTART,(BYTE*)&SysInfo,EEP_SYS_LENTH);
				SetYuyin(YUYINT_DETECT);
				}
			}
		}
		
		

	 
	 return NEW_KEY;
}




void  DO_Key_Action()
{	
  BYTE 	Tnewkey;
	Tnewkey=ScanKey();

	if(Tnewkey==KEY_NOKEY)return;
	Count_5S=5;


	switch(Tnewkey)
	{
	
		
	case	KEY_MAT1:
		
			//单键对两码

			#if 1
			KMATCH++;
			if(KMATCH>2)
			{ KMATCH=0;	KTMOUT=0;
			  LED_RED(LED_OFF);;	//熄灭
			} else KTMOUT=40;
			#else 
			KMATCH=1;
			KTMOUT=40;
			#endif

	break;

	case	KEY_MAT2:
			KMATCH=2;
			KTMOUT=40;

		break;


	default:
//		KEY_PRINTF(printf("-othkey-"));
		break;

	}
	  

}









