
#ifndef __MY_TYPE_H__
#define __MY_TYPE_H__


/*=====================================================================
  basic type definition
=====================================================================*/
typedef bit               BIT;
typedef bit               BOOL;
typedef unsigned char     BYTE;
typedef unsigned int      WORD;
typedef unsigned long     DWRD;
typedef unsigned long     DWORD;
typedef signed char       CHAR;
typedef signed int        INT;
typedef signed long       LONG;
typedef unsigned long     ULONG;
typedef float             FLOAT;
typedef double            DOUBLE;
typedef long double       LDOUBLE;
typedef unsigned int	  UINT;
typedef    unsigned char   INT8U;
typedef    unsigned int    INT16U;
typedef    unsigned char   U8;
typedef    unsigned int    U16;
typedef unsigned int      uint	;
typedef unsigned char     uchar	;
typedef unsigned char     UCHAR;


typedef signed char	int8;
typedef signed int	int16;
typedef signed long	int32;
typedef unsigned char	uint8;
typedef unsigned int	uint16;
typedef unsigned long	uint32;




typedef unsigned char     *PUCHAR;
typedef BYTE              *PBYTE;
typedef UINT              *PUINT;
typedef WORD              *PWORD;
typedef ULONG             *PULONG;
typedef DWORD             *PDWORD;
typedef void              *PVOID;
#define LOBYTE(w)	    ((BYTE)(w))
#define HIBYTE(w)	    ((BYTE)(((UINT)(w) >> 8) & 0xFF))

#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8 )))

#define LOWORD(l)	    ((WORD)(DWORD)(l))
#define HIWORD(l)	    ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))

#define SBIT  sbit
#define LONG 	long








#define DATA              data
#define BDATA             bdata
#define IDATA             idata
#define PDATA             pdata
#define XDATA             xdata
#define RDATA             code

#define CBYTE             ((unsigned char volatile code  *) 0)
#define DBYTE             ((unsigned char volatile data  *) 0)
#define PBYTE             ((unsigned char volatile pdata *) 0)
#define XBYTE             ((unsigned char volatile xdata *) 0)
#define IBYTE             ((unsigned char volatile idata *) 0)

//////////////////////////////////////////////
// Macro
//////////////////////////////////////////////
#define HIBYTE_REF(addr)          (*((BYTE *) & (addr)))
#define LOBYTE_REF(addr)          (*((BYTE *) & (addr + 1)))
#define MAKEUINT(value1, value2)  (((WORD)(value1)) * 0x100) + (value2)
#define H2BYTE(value)             ((BYTE)((value) / 0x10000))

#define HINIBBLE(value)           ((value) / 0x10)
#define LONIBBLE(value)           ((value) & 0x0f)

#define max(a, b)                 (((a) > (b)) ? (a) : (b))
#define min(a ,b)                 (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////
// Others
//////////////////////////////////////////////
#define _ENABLE      1
#define _DISABLE     0


#define		FALSE		0
#define	  	TRUE		1


#define _HIGH    1
#define _LOW     0

typedef union _HiLo
{
    struct _b_
    {

        BYTE h;
        BYTE l;
    } b;
    BYTE bte[2];
    WORD w;
} HLdata;

typedef union _WB3data
{

    BYTE bte[3];
    struct _b
    {

        WORD w;
        BYTE b;

    } b;


} WB3data;
typedef union _HiLoDW
{
    BYTE b[4];
    WORD w[2];
    DWORD d;
} HLdw;
#define     H   0
#define     L   1

#define I2C_ACKNOWLEDGE        _LOW
#define I2C_NON_ACKNOWLEDGE    _HIGH

#define _BIT0	0x0001
#define _BIT1	0x0002
#define _BIT2	0x0004
#define _BIT3	0x0008
#define _BIT4	0x0010
#define _BIT5	0x0020
#define _BIT6	0x0040
#define _BIT7	0x0080
#define _BIT8	0x0100
#define _BIT9	0x0200
#define _BIT10	0x0400
#define _BIT11	0x0800
#define _BIT12	0x1000
#define _BIT13	0x2000
#define _BIT14	0x4000
#define _BIT15	0x8000

#define _bit0_(val)  ((bit)(val & _BIT0))
#define _bit1_(val)  ((bit)(val & _BIT1))
#define _bit2_(val)  ((bit)(val & _BIT2))
#define _bit3_(val)  ((bit)(val & _BIT3))
#define _bit4_(val)  ((bit)(val & _BIT4))
#define _bit5_(val)  ((bit)(val & _BIT5))
#define _bit6_(val)  ((bit)(val & _BIT6))
#define _bit7_(val)  ((bit)(val & _BIT7))
#define _bit8_(val)  ((bit)(val & _BIT8))
#define _bit9_(val)  ((bit)(val & _BIT9))
#define _bit10_(val) ((bit)(val & _BIT10))
#define _bit11_(val) ((bit)(val & _BIT11))
#define _bit12_(val) ((bit)(val & _BIT12))
#define _bit13_(val) ((bit)(val & _BIT13))
#define _bit14_(val) ((bit)(val & _BIT14))
#define _bit15_(val) ((bit)(val & _BIT15))
#define TBL_SIZE(tbl)   (sizeof(tbl)/sizeof(tbl[0]))



typedef struct
{

    BYTE Time_1_hour;
    BYTE Time_1_min;
    BYTE Time_1_sec;
    BYTE Time_1_20msec;



} data_time ;
#endif
