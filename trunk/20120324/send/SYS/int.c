#ifndef	_INT_C
#define	_INTC_
#include "general.h"

// define IR_NEC if you use NEC IR
/*
中断源
0 外部中断0
1 定时器0 溢出
2 外部中断1
3 定时器1 溢出
4 串行口中断
5 定时器2 溢出
表0-6

*/

//sbit	 btIR=P3^3;
//sbit	 btIR=P3^2;

void   IR_core_NEC(WORD width) ;



#pragma NOAREGS

//***************************************************************
//************the continued key  is valid?     ******************
//if valid return  value 1   else return 0    *******************
//***************************************************************
BYTE repeat_IRkey_allowed(void)
{

    if(REMOTE_CF)
    {

        if(
            0

        )
            return 1;
    }



    // ****************************************************************


    return 0;
}

#define IR_CUSTOM	2
#define IR_USERCODE	1
#define IR_START	0
#define IRREP

#define		IR_TMOUT	150  //ms 释放后120ms
#define		IRTMOUT_BASE	50			//计时单位20ms
#define		IRTMOUT_COUNT	(IR_TMOUT/IRTMOUT_BASE)
//unsigned  char  stateIR,codeIR;
void   IR_core_NEC(WORD PulseWidth )
{

#ifdef	PCA_FOR_IR
#define	IRCLOCK_BASE_PERMS_COUNT		   	(48000000/12/1000)
#else
#define	IRCLOCK_BASE_PERMS_COUNT		   	(12000000/12/1000)
#endif

#define LEADER_MIN	(WORD) (13.5*IRCLOCK_BASE_PERMS_COUNT-0.5*IRCLOCK_BASE_PERMS_COUNT  )
#define LEADER_MAX	(WORD) (13.5*IRCLOCK_BASE_PERMS_COUNT+0.5*IRCLOCK_BASE_PERMS_COUNT )
#define DATA_1_MIN	(WORD) (2.24*IRCLOCK_BASE_PERMS_COUNT-0.3*IRCLOCK_BASE_PERMS_COUNT )
#define DATA_1_MAX	(WORD) (2.24*IRCLOCK_BASE_PERMS_COUNT+0.3*IRCLOCK_BASE_PERMS_COUNT )
#define DATA_0_MIN	(WORD) (1.12*IRCLOCK_BASE_PERMS_COUNT-0.3*IRCLOCK_BASE_PERMS_COUNT)
#define DATA_0_MAX	(WORD) (1.12*IRCLOCK_BASE_PERMS_COUNT+0.3*IRCLOCK_BASE_PERMS_COUNT )
#ifdef IRREP
#define REPEAT_MIN	(WORD) (11.5*IRCLOCK_BASE_PERMS_COUNT-0.5*IRCLOCK_BASE_PERMS_COUNT )
#define REPEAT_MAX	(WORD) (11.5*IRCLOCK_BASE_PERMS_COUNT+0.5*IRCLOCK_BASE_PERMS_COUNT)
#endif /* IRREP */
#define THX	TH1		/*TIME COUNT  */
#define TLX	TL1		/*TIME COUNT  */
#define TRX	TR1

    static BYTE  data     dataIR;
    static BYTE  data     cntIRbits;
    WORD data     width = PulseWidth;
    bit reset;
    reset = 0;
    RemoteCTM = IRTMOUT_COUNT;
    switch(stateIR)
    {
    default:
    case IR_START:
        stateIR = IR_USERCODE;
        break;
    case IR_USERCODE:
        if (	(width >= LEADER_MIN) && (width <= LEADER_MAX)
           )
        {
            stateIR = IR_CUSTOM;
            dataIR = 0;
            cntIRbits = 0;
        }
#ifdef IRREP

        else if ((width >= REPEAT_MIN) && (width <= REPEAT_MAX))
        {
            /* if the width is 2.25 ms, it is repeat code leader */
            if (repeat_IRkey_allowed())
            {
                if(REMOTE_CF)codeIR_Valid = 1;
            }
#ifdef CHECK_IR_CUSTOMER_CODE
            if(User_Code != CUSTOMER_CODE && User_Code != CUSTOMER_CODE1)codeIR_Valid = 0;
#endif

            REMOTE_CF = 1;
            RemoteCTM = IRTMOUT_COUNT * 2;
        }
#endif
        //  else   reset = 1;	   //重复码出问题  ，所以去掉
        break;


    case IR_CUSTOM:
        dataIR = dataIR >> 1;
        if ((width >= DATA_1_MIN) && (width <= DATA_1_MAX))
        {
            dataIR |= 0x80;
        }
        else if ((width < DATA_0_MIN) || (width > DATA_1_MAX))
        {
            reset = 1;
            break;		 //加上此句防误码
        }

        cntIRbits++;
        /* First 16 bits are syscode */
        if(cntIRbits == 8)
        {
            User_Code = dataIR;
        }
        else if(cntIRbits == 16)
        {
            User_Code1 = dataIR;
            //if(User_Code!=~dataIR)reset=1;
        }
        else if (cntIRbits == 24)
        {
            IR_code = dataIR;
            //gbCodeKeep=aIRMap[dataIR];
            //	{extern  BYTE Search_RCKEYTBL(BYTE RecIRkey);
            //	gbCodeKeep=Search_RCKEYTBL(IR_code);
            //  }
            gbCodeKeep = dataIR;
#ifdef CHECK_IR_CUSTOMER_CODE
            if(User_Code != CUSTOMER_CODE && User_Code != CUSTOMER_CODE1)
                codeIR_Valid = 0;
            else
#endif
                codeIR_Valid = 1;

        }
        else if (cntIRbits == 32)reset = 1;
        break;

    }


    if (reset)
    {
        /* Reset all, start from the very beginning */
        stateIR = IR_START;
    }
    if(codeIR_Valid)
    {
        codeIR_Valid = 0;
        RC_Key = gbCodeKeep;
        //RC_Key=IR_code;
    }

    return ;
}

void Init_IR(void)
{
    stateIR = IR_START;
    User_Code = 0x00;	/*the custom code*/
    User_Code1 = 0xff;	/*the custom code*/
    IR_code = KEY_NOKEY;		/*****IR code ********/
    gbCodeKeep = KEY_NOKEY; /* Record the previous ID code  */
    RemoteCTM = 0;	//按键释放后计时
    RC_Key = KEY_NOKEY;		//
    codeIR_Valid = 0;
    REMOTE_CF = 0;	//按键保持标志
    gbPressHoldTime = 0; //遥控按键保持的时间    长按键

    gbKEYRepTime = 0; //遥控按键连续加减的的时间间隔50*x ms    0表示没有间隔50ms
    gbNEW_KEY = KEY_NOKEY;

}

#endif