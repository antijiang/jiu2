
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

#define	T_TORANCE	0.30
#define	CYC_PER_US		12/8/TM0_RELOAD  //(24.5/12) //24500000/12/1000000us
#define TE ((DWORD )20000*CYC_PER_US) // half bit time = 1000 usec
#define TMOUT_DALI	((9170+9170-2920)*CYC_PER_US)  //48000000/12  per s  is =4 clk
//#define TMOUT_DALI	(9170*4)  //48000000/12  per s  is =4 clk
#define TMHEADFLG_START_9	(WORD)(9000*CYC_PER_US)  //48000000/12  per s  is =4 clk
#define TMHEADFLG_START_4P5	(WORD)(4500*CYC_PER_US)  //48000000/12  per s  is =4 clk


#define MIN_TE ((WORD)(TE - TE*T_TORANCE)) // minimum half bit time
#define MAX_TE ((WORD)(TE + TE*T_TORANCE)) // maximum half bit time
#define MIN_2TE ((WORD)(2*TE - 2*TE*T_TORANCE)) // minimum full bit time
#define MAX_2TE ((WORD)(2*TE + 2*TE*T_TORANCE)) // maximum full bit time

static BYTE Dovalue; // used for dali send bit

static BYTE position; // keeps track of sending bit position

static BYTE previous; // previous received bit

static BYTE SendData[4]; // holds received slave backward frame


static BYTE f_repeat; // flag command shall be repeated

//static 
BYTE f_busy; // flag DALI transfer busy
static //HLdata   HLdw
	DWORD forwardframe; 
//BYTE F_enRec=0;
#define	DALI_SEND_INDICATOR		
static BYTE gbDALI_Answer;
   

#define	DALI_ACT_FLAG	0x8000
#define	DALI_ACK		0x80


////////////////////////////////////////////////////////////////////////////
///////////////
// 函数名：
// 编写者：
// 参考资料：
// 功 能：设定定时器的时间间隔，并启动pca2定时器
// 输入参数：Newpt   时间间隔tick ，(定时器输入为48000000/12)
// 输出参数：
// 备 注：
////////////////////////////////////////////////////////////////////////////
///////////////
void ResetTmrPCA2Duratio(WORD Newpt)
{
	WORD temp=PCA0;	
	temp=temp+Newpt;		// ~ 2400 Hz
	PCA0CPM2&=~0x01;	// disable capture interrupt
	CCF2=0;
	PCA0CPL2=LOBYTE(temp);  //有可能误读
	PCA0CPH2=HIBYTE(temp);
	PCA0CPM2=0x49;	// enable timer
  }

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
#define	DALIOUT				P15
#define	DALI_SETH()			DALIOUT=1
#define	DALI_SETL()			DALIOUT=0
//#define	DISABLE_PCA2()	PCA0CPM2&=~0x41	   //禁止定时方式要eco=0 eccf=0
#define	DISABLE_PCA2()		PCA0CPM2=0	   //禁止定时方式要eco=0 eccf=0
#define	DISABLE_TX_SEND()	DISABLE_PCA2()


#define	DISABLE_PCA1()				{PCA0CPM1=0;CCF1=0;/*F_enRec=0;*/}			//不允许沿产生ccf1标志
//#define	DISABLE_PCA1()				PCA0CPM1&=~0x01
#define	ENABLE_PCA1()				{PCA0CPM1|=0x31	;CCF1=0;/*F_enRec=1;*/}// enable rx, capture on both edges
#define	ENABLE_RX_BOTH_EDGE()		ENABLE_PCA1()
#define	DISABLE_RX()				DISABLE_PCA1()	

#define SET_DALI_SEND_INDICATOR()
#define CLR_DALI_SEND_INDICATOR()

#pragma NOAREGS

//manchester 编码发送
void DALI_SendData(void)	  
{
	 BYTE temp1=0,temp;
	ResetTmrPCA2Duratio(TE); // reset timer
	return;
   // interrupt for DALI send
	{
		
		if (Dovalue)	DALI_SETH(); // DALI output pin high
		else			DALI_SETL(); // DALI output pin low

	
        if (position == 0) // 0TE second half of start bit = 1
		{
	
			Dovalue = 1;//设置下一电平1; 
		   
		
		}
		else if (position < (33+32)) // 1TE - 32+32 TE, so address + command
		{//设置8BIT ADDRESS,8BIT COMMAND
			//Dovalue 
			//forwardframe=0x55aa;
			//temp=forwardframe;
			
			temp1=(32+32 - position)/2;
			temp= forwardframe>> temp1 ;
		//	temp= _iror_( forwardframe,temp1);
			Dovalue=temp&0x01;
			//Dovalue = (forward >> ((32 - position)/2)) & 1;
			if (position & 1)
			Dovalue = !Dovalue; // invert if first half of data bit
		}
		else if (position == (33+32)) // 33TE start of stop bit (4TE)
		{ 
			Dovalue = 1;	//设置STOP位
			SET_DALI_SEND_INDICATOR();
		}

		//4te+7te =0.417ms*4+ 2.92ms(7*0.417)到达了
		else if (position == (33+32+1)) // 44TE, end stop bits 
		{
			
			DISABLE_TX_SEND(); // stop and reset timer
			CCF2=0;
			DISABLE_RX(); //禁止接受
			CLR_DALI_SEND_INDICATOR();
		
			DALI_SETL();
			f_busy = 0; // end of transmission
		//	if (f_repeat) // repeat forward frame ?
			//	f_dalitx = 1; // yes, set flag to signal application
			
		}

	position++;
//	T0IR = 0x01; // reset MR0 interrupt flag
}


}
////////////////////////////////////////////////////////////////////////////
///////////////
// 函数名：
// 编写者：
// 参考资料：
// 功 能：启动大力发送过程，在放置大力命令后调用
// 输入参数：
// 输出参数：
// 备 注：
////////////////////////////////////////////////////////////////////////////
///////////////

void DALI_Send_Start(void)
{
   return;
 	f_busy = 1; // set transfer activate flag

 //激活再生电路
	DALI_SETH();
	DelayXms(15);
	DALI_SETL();
	DelayXms(15);
	
	DALI_SETH();
	DelayXms(15);
	DALI_SETL();
	DelayXms(15);
	DALI_SETH();
	DelayXms(15);
	DALI_SETL();
	DelayXms(15)
	
	;
	DALI_SETH();
	Dovalue = 0; // 
	position = 0;


	//T0的匹配寄存器T0MR0值设为TE，T0还有捕获寄存器T0CR0
	//T0可选捕获或匹配方式进行中断，这里选匹配中断而禁止捕获中断，
	//当T0TC的值与T0MR0的值相等时产生中断，并且T0TC复位
	ResetTmrPCA2Duratio(TE*1.5);
	DALI_SETH();
	
//	while (f_busy) ; // Wait until dali port is idle
//	DelayXms(5);
	
}

#pragma NOAREGS





//*****************************************
//初始大力的变量
void DALI_Init(void)
{
                                                                                                                                
	 f_busy=0;
	 position=0;
	 DISABLE_TX_SEND();
}
void DaliSendRep()
{	
BYTE i=0;

	f_repeat=0;
	DALI_Send_Start();
	/*
	if(f_repeat)
	{
		for(i=0;i<4;i++)
		DALI_Send_Start();
		f_repeat=0;
	}
	*/
//	DelayXms(50);
	DelayXms(30); //正常状态
	//DelayXms(150);//针对NEC 
}

//组合DALI命令 ，并设定发送
//高位先发，最高位为1  gong32bit
//（1|16位随机）+4bit随机+4bit命令+4bit命令反码+4bit 固定0101
void	DALI_Send(BYTE dalicmd)
{
extern _SYSINFO SysInfo	 ;
	forwardframe=  ((WORD)dalicmd<<8)| (((~dalicmd)<<4)&0xf0)  |0x05   ;
	forwardframe|=0x80000000  ;
	forwardframe|=(SysInfo.TRANCODE<<12);
//	forwardframe=0x80000005;
	DaliSendRep();
}


#endif