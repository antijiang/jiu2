/********************************************************************
; DALI master

; For transmit, this module uses GPIO - P0.28 (DALI send pin)

; DALI forward frame format:
; 		| S	| 	8 address bits | 8 command bits | stop |
; 		|  1  |  1  |  0  |  0  |  0  |  0  |  0  |  1  |  1  |  0  |  1  |  1  |  1  |  1  |  0  |  0  |  0  | | |
;	++++ +-+	
;		|  |
;	      +-+
; |2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE| 4TE |

; For receive, this module uses T0-CAP0 input (capture and interrupt on both edges)
; CAP0.0 (P0.30) is connected to P0.29 (to check high / low level by software)
; DALI slave backward frame format:
; | S | 8 data bits | stop |
; | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | | |
; +---------------+ +-+ +---+ +-+ +-+ +-+ +-+ +-+ +-------------
; | | | | | | | | | | | | | | | | |
; -+ +-+ +-+ +-+ +-+ +-+ +-+ +---+ +-+
; |4 + 7 to 22 TE |2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE|2TE| 4TE |

; 2TE = 834 usec (1200 bps)

************************************************************************************/
#ifndef	_DALI_DRV_C_
#define	_DALI_DRV_C_
#pragma 
#include "GENERAL.H "


static WORD low_time; // captured puls low time

static WORD  high_time; // captured puls high time

static BYTE Dovalue; // used for dali send bit

static BYTE position; // keeps track of sending bit position

static BYTE previous; // previous received bit

static DWORD backwardframe; // holds received slave backward frame

static BYTE f_repeat; // flag command shall be repeated

static BYTE f_busy; // flag DALI transfer busy
static //HLdata 
	DWORD forwardframe; 
//BYTE F_enRec=0;
#define	DALI_SEND_INDICATOR		P04
static BYTE gbDALI_Answer;
   

#define	DALI_ACT_FLAG	0x8000
#define	DALI_ACK		0x80




////////////////////////////////////////////////////////////////////////////
///////////////
// 函数名：
// 编写者：
// 参考资料：
// 功 能：在中断中调用
// 输入参数：
// 输出参数：
// 备 注：
////////////////////////////////////////////////////////////////////////////
///////////////
//#define	DISABLE_PCA2()	PCA0CPM2&=~0x41	   //禁止定时方式要eco=0 eccf=0
#define	DISABLE_PCA2()		PCA0CPM2=0	   //禁止定时方式要eco=0 eccf=0
#define	DISABLE_TX_SEND()	DISABLE_PCA2()


#define	DISABLE_PCA1()				{PCA0CPM1=0;CCF1=0;/*F_enRec=0;*/}			//不允许沿产生ccf1标志
//#define	DISABLE_PCA1()				PCA0CPM1&=~0x01
#define	ENABLE_PCA1()				{PCA0CPM1|=0x31	;CCF1=0;/*F_enRec=1;*/}// enable rx, capture on both edges
#define	ENABLE_RX_BOTH_EDGE()		ENABLE_PCA1()
#define	DISABLE_RX()				DISABLE_PCA1()	
#pragma NOAREGS





//*****************************************
//初始大力的变量
void DALI_Init(void)
{
                                                                                                                                
	backwardframe     = 0;                                     // DALI forward frame 
	position=0;

}


#pragma NOAREGS

#if 1
#define IR_CUSTOM	2
#define IR_USERCODE	1
#define IR_START	0
#define IRREP
#define IR_FIRST_BIT    3
//def IR_PHILIPS
#define	CYC_PER_US1		12/8/TM0_RELOAD
//#define	TMS	(CYC_PER_US*1000)
#define	TM1MS		((DWORD)1000*CYC_PER_US1)
#define	TMS		((DWORD)20000*CYC_PER_US1)
#define T_MIN         (TMS-0.2*TMS)
#define T_MAX         (TMS+0.2*TMS)

#define T20_MIN      (WORD)(2*TMS-0.2*TMS)
#define T20_MAX      (WORD)(2*TMS+0.2*TMS)

#define T30_MIN      (WORD)(3*TMS-0.2*TMS)
#define T30_MAX      (WORD)(3*TMS+0.2*TMS)
#define T40_MIN      (WORD)(4*TMS-0.2*TMS)
#define T40_MAX      (WORD)(4*TMS+0.2*TMS)
#define T_START_MIN  (WORD)(1.5*TMS-0.2*TMS)
#define T_START_MAX  (WORD)(1.5*TMS+0.2*TMS)
//falling edge
const BYTE aIRMap[]=
{
  0xff,0xff

};


DWRD  dataIR;
static BYTE  stateIR;
static BYTE codeIR_Valid ;
 DWRD gbCodeKeep;
void DALI_Receive(WORD capture_period )

//void Receive_PHILIPS(WORD capture_period)
{
    	
	WORD width=capture_period;
    BYTE bit_time;
	BOOL last_bit;
    static cntIRbits=0;	
	 
 //stateIR =IR_CUSTOM;
  if(stateIR == IR_START)
  {
           /* is syn? when rising edge /\__/\ */
             if( width>T_START_MIN&&width<T_START_MAX)
             {	   if(PIORFIN==0)  //抓到高电平为码头
			 		{
                 	stateIR = IR_CUSTOM;
	         		cntIRbits=1;
	         		dataIR = 1;
				
					//设为下降沿
					PCA0CPM0&=~0x30;
					PCA0CPM0|=0x10;
					}
              }
              else
             {	dataIR=0x01;
                stateIR = IR_START;
              }

    }
     else if(stateIR ==IR_CUSTOM)
     {
#if 1

          if( (width >= T20_MIN) && (width <= T20_MAX))bit_time=2;	
          else if ( (width >= T30_MIN) && (width <= T30_MAX))bit_time=3;
          else if( (width >= T40_MIN)  && (width <= T40_MAX))bit_time=4;
          //else if( width<T_START_MIN||width>T_START_MAX)bit_time=0;
          else bit_time=0;
#endif

        last_bit=dataIR&0x01;
    	dataIR <<=1;
    	cntIRbits++;
	if (bit_time==2){
	//00 or 11
	        dataIR |= last_bit;

	} else if(bit_time==3) {
	  
#ifdef	FALLING_IS_1 	
//		10 001(falling)
 	     if(last_bit==0)
		 {  dataIR <<= 1;
 	               dataIR |= 1;
			       cntIRbits++;
		 }
#else
////01 or 110--- 
 		  dataIR |= 1;
		  if(last_bit==1)
		  { 
			dataIR <<= 1;
  			cntIRbits++;
		  }

#endif

	} else if(bit_time==4) {
#ifdef	FALLING_IS_1 
	//010(rising) or 101(falling edge)
	     if(last_bit==1){
	     		     dataIR<<=1;
	     		     dataIR|= 1;	
 		    	     cntIRbits++;
	   		} else stateIR = IR_START;
#else
	//010(rising) 
	     if(last_bit==0){
	     		     dataIR|= 1;
					 dataIR<<=1;
 		    	     cntIRbits++;
	   		} else stateIR = IR_START;
#endif
	}
//	else if(bit_time==1) {	}
	else 	stateIR = IR_START;
	if(cntIRbits==18){
			extern _SYSINFO SysInfo;
			if(	0 )//dataIR!= SysInfo.TRANCODE)
			{
			stateIR = IR_START;
			}

	}else 
    if(cntIRbits==34){
//起始位2个		   
	   gbCodeKeep=dataIR;
//	   if( gbCodeKeep==0x80000005)LEDIO=!LEDIO;
	   codeIR_Valid=1;
	   stateIR = IR_START;
	   //IO_LED=1;
	     }
	
   }else stateIR = IR_START;

      if(stateIR == IR_START){
         //上升下降沿  \__/
		 PCA0CPM0|=0x30;
				
            cntIRbits=1;
	  		dataIR = 1;
          }
}


BYTE  GetIRkey()
{
	
	   if( codeIR_Valid)
	   {
	   	 codeIR_Valid=0;

	     return 1 ;
	   }
   return 0;
}
#endif

#endif