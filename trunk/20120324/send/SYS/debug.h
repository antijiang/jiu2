
#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef	_DEBUG_C_
#define	_DEBUGDEC_INI(a,init_val)	a =init_val
#define	_DEBUGDEC_(a)	a
#else
#define	_DEBUGDEC_INI(a,init_val)	extern a
#define	_DEBUGDEC_(a)	extern a
#endif

////////////////////////////////////////////////////////////////
// Debug option.If disable all, please modify project file.
////////////////////////////////////////////////////////////////
#define _DEBUG_PRINT_EN_          1  //For printf,putchar and putstr
#define _DEBUG_EXTDEV_EN_         1  //For extend device
#define _DEBUG_TESTCOMMAND_EN_    1  //For virtual IR and keypad

#define	MAX_UART_BUFLEN	100	 		//
#define MAX_SENDBUF_LEN	MAX_UART_BUFLEN

#define UART_TIMEOUT_500MS	10
//#define	DEBUG_AD_EN		//允许打印邋AD值





enum _UARTCMD_ENU_
{
    UCMD_DAC_VALUE = 0,
    UCMD_DALI_CMD = 0x01,
    UCMD_READ_EEPROM,
    UCMD_WRITE_EEPROM,

    UCMD_WRITE_FLASH,
    UCMD_READ_FLASH	,


    UCMD_ZERO_RESET = 0x0B,		//清零 和 设置零点



    UCMD_WRITE_DAC = 0x0e,
    UCMD_DAC_NORMAL,
    UCMD_DEBUG_ON = 0x10,	//调试信息打开

    UCMD_QUARYBALLASTPAR = 0x20,

};


#define _VERSION_H_
#define _CHIP_VERSION_          "[TEC4J10]"  //"[C8051F410]"
#define _CODE_VERSION_          "V1.0"
#define _CODE_DATE_             __DATE__  // ANSI format(month dd yyyy)
#define _CODE_DATE2_            __DATE2__ // Short format(mm/dd/yy)
#define _CODE_TIME_             __TIME__
#define _C51_COMPILER_VERSION_  __C51__
#define CODE_ID                 __TIME__//_CODE_VERSION_
//////////////////////////////////////////////////////
// Debug subroutines
//////////////////////////////////////////////////////


#endif
