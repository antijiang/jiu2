//程序文件如下
//spicom.c
#include "general.h"
#define	_SPICOM_C_


//其他平台包含文件
#define ENABLE_COM_3_LINE


#include "spicom.h"
#include "spicom_in.h"

#ifdef	ENABLE_COM_3_LINE  
//#define	DEBUG_SPI
#ifdef	DEBUG_SPI
#define	DPRINTF1(a)	a

#define	DPRINTFX(a) a
#else
#define	DPRINTF1(a)
#define	DPRINTFX(a)
#endif

#if 1
//其他地方计时器减
#define	SPIIsTMOut()		SPITimeOut
#else
//本地自身延时减		
//宏 BASE_COUNT 为10ms ，对应Delay_BASE_COUNT()的延时函数
BYTE	SPIIsTMOut()		{if(SPITimeOut!=0)SPITimeOut-elay_BASE_COUNT();return 	SPITimeOut;}	
#endif

unsigned char    SPITimeOut=0;  //500ms超时 变量 SPITimeOut需要自己设置,利用计时器减到0为止
extern  unsigned char xdata   gbTimeOutConst;
#define  TimeOutCNT	gbTimeOutConst

#define	WAIT_WHILE_MCK_IS_H()	{SPITimeOut=TimeOutCNT;   \
								while(GetMCLK()&&SPIIsTMOut()); \
        						if(	SPITimeOut==0){InitSPICom();DPRINTF1(printf("TOUTr1%",0)); return (0); 	}  \
								}

#define	WAIT_WHILE_MCK_IS_L()	{SPITimeOut=TimeOutCNT;   \
								while(!GetMCLK()&&SPIIsTMOut()); \
        						if(	SPITimeOut==0){InitSPICom();DPRINTF1(printf("TOUTr ",0)); return (0); 	}  \
								}

#define	WAIT_WHILE_SCK_IS_H() 	{SPITimeOut=TimeOutCNT; \  
                                while(GetSCLK()&&SPIIsTMOut()); \ 
		                        if(SPITimeOut==0){InitSPICom();DPRINTF1(printf("TOUTs ",0));return (0); }	\  
								}
#define	WAIT_WHILE_SCK_IS_L()	{SPITimeOut=TimeOutCNT; \  
                                while(!GetSCLK()&&SPIIsTMOut()); \ 
		                        if(SPITimeOut==0){InitSPICom();DPRINTF1(printf("TOUTs1 ",0));return (0); }	\  
								}

/*****************************************
// 完整发送一个字节
*******************************************/
char PutBytespi(BYTE  SendDATA)
{

    BYTE i;
  	 
 //   MCLK=1;SCLK=1;_nop_();_nop_(); 
    if(!GetMCLK())return FALSE; //    
    	
  	DPRINTFX(printf("-OPD-=%x \r\n",SendDATA));
  	
  	
  	Set_MCLK_Mode_OUT();
  	
  	Set_SDA_Mode_OUT();
  	//申请通讯
  	ClrMCLK();
      
    for(i=8;i!=0;i--)
    {   DPRINTFX(printf("Tbit=%bx ",i));
		
		//检测从机的 SCL 1-->0电平响应, 
		//TimeOut=TimeOutCNT;	  //超时处理
    // while(SCLK&&TimeOut); //检测从机响应
	  //if(	TimeOut==0){DPRINTF1(printf("TOUTs "));return (0); }
		WAIT_WHILE_SCK_IS_H();

        DPRINTFX(printf(" SCLK 0%bx",0));
        if(SendDATA&0x01)SetSDA();
				else ClrSDA();
           //放一个bit到SDA
        Delay5us();        //等待数据稳定
        SetMCLK(); 			//指示从机读，从机检测到MCLK=1 才读数据
		
		       
        SendDATA=SendDATA>>1; //准备下一个bit
	//Delay5us();            //需要延时

		//检测从机的SCL 0--->1响应,从机读完应答SCLK=1
		WAIT_WHILE_SCK_IS_L();
		//TimeOut=TimeOutCNT;		 //超时处理
        //while(!SCLK&&TimeOut); 
		//if(	TimeOut==0){DPRINTF1(printf("TOUTs1 "));return (0); }

        DPRINTFX(printf(" %bx ",1));
       // SDA=SendDATA&0x01;   //放一个bit到SDA
        Delay5us();        //等待数据稳定
         
        ClrMCLK();      
       // SendDATA=SendDATA>>1; //准备下一个bit
    }

	//检测从机收完
	
	WAIT_WHILE_SCK_IS_H();
 
	SetMCLK();
	Set_MCLK_Mode_IN();
	
	//等待从机准备好
	WAIT_WHILE_SCK_IS_L();
	
   
    Set_SDA_Mode_IN();
    return TRUE;

}

/******************************************************************************
//接收一个数据
//调用之前最好检测MCLK是否为0（对方是否申请数据通讯）再进入,否则里面是死等此信号!
*******************************************************************************/
char GetBytespi( BYTE  * pbuf)
{


    BYTE i,RxDATA;

    if(GetMCLK())return 0;
    	 //接受通讯请求
  	Set_SCLK_Mode_OUT();
  	ClrSCLK();
    for(i=8;i!=0;i--)
    {   
		DPRINTFX(printf("Rb%x ",i));
		
        RxDATA=RxDATA>>1; //准备收下一个bit
	//	Delay5us();        //等待SCLK 稳定
        
		 //检测主机放数据
		 WAIT_WHILE_MCK_IS_L();
		//TimeOut=TimeOutCNT;
		//while(!MCLK&&TimeOut);
        //if(	TimeOut==0){DPRINTF1(printf("TOUTr ")); return (0); 	}
        
		DPRINTFX(printf("M%x ",1));
		if(GetSDA())RxDATA|=0x80;//收一个bit
        //SCLK=1;
        SetSCLK();
//	Delay5us();                //需要延时
 
        //RDATA=RDATA>>1; //准备收下一个bit
		WAIT_WHILE_MCK_IS_H();
	    //TimeOut=TimeOutCNT;
		//while(MCLK&&TimeOut); //检测主机放数据
		//if(	TimeOut==0){DPRINTF1(printf("TOUTr1 ")); return (0); 	}
//		Delay5us();                 //需要延时

        DPRINTFX(printf("M%x ",0));
        //if(SDA)RDATA|=0x80;;  //收一个bit
        //SCLK=0;   
        ClrSCLK(); 
    }
	//检测主机发完
	WAIT_WHILE_MCK_IS_L();

	SetSCLK();
	Set_SCLK_Mode_IN();
  
    *pbuf=RxDATA;
	DPRINTFX(printf("-Rd--%x \r\n",RxDATA));
	
    return(1);    
}




 //TEST_SPI(CMD_CLR_P54)	 ;	//p5.4H
	// TEST_SPI(CMD_SET_P54)	 ;	//p54=0
	// TEST_SPI(CMD_CLR_P00)	 ;	//p00=H
	// TEST_SPI(CMD_SET_P00)	 ;	//p00=0
	// TEST_SPI(CMD_RD_P11)	 ;
//	TEST_SPI(CMD_RD_ID)
/*****************************************
返回-1； 通信失败
返回 0:  低电平或ID错误
返回 1:	正常或高电平
****************************************/
char TEST_SPI(char CMD_a)
{
	BYTE i; 
	BYTE RecBuf[10];

	
 	if(!PutBytespi(0x55))return -1;
	if(!PutBytespi(CMD_a))return -1;
   	switch(CMD_a)
	{
	case	CMD_RD_ID:
		
    RecBuf[0]=0;
	WAIT_WHILE_MCK_IS_H();
	GetBytespi( &RecBuf);
	

	for(i=0;i<RecBuf[0];i++)
	{ 
	  WAIT_WHILE_MCK_IS_H();	
	  GetBytespi( &RecBuf[i+1]) ;
 	}

	if( (RecBuf[1]!='E')||
		(RecBuf[2]!='S')||
		(RecBuf[3]!='0')||
		(RecBuf[4]!='9')
	)return 0;

	break;
	case	CMD_RD_P11:
	if(0==GetBytespi( &RecBuf))return -1; ;
	DPRINTFX(printf("P11=%x \r\n",RecBuf[0]));
	if(RecBuf[0]==0){
	 //low level
	 return 0;
	}	else return 1;
	break;
	default:
	
	break;
	}

   return 1;
}



#endif
