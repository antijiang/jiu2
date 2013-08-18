#ifndef	_IR_FUN_H_
#define	_IR_FUN_H_
enum
{
    IRKEY_NOKEY		= 0xff,
    IRKEY_TV			= 0x02,
    IRKEY_PC			= 0x10,
    IRKEY_REW		= 0x1A,
    IRKEY_PLAY		= 0x1B,
    IRKEY_FWD		= 0x44,
    IRKEY_STOP		= 0x40,
    IRKEY_REPLAY =		0x5A,
    IRKEY_PAUSE		= 0x42,
    IRKEY_SKIP		= 0x45,
    IRKEY_RECORD		= 0x48,
    IRKEY_BACK		= 0x14,
    IRKEY_MORE		= 0x15,
    IRKEY_LEFT		= 0x17,
    IRKEY_RIGHT		= 0x0C,
    IRKEY_UP			= 0x16,
    IRKEY_DOWN		= 0x12,
    IRKEY_OK			= 0x18,
    IRKEY_ASPECT		= 0x43,
    IRKEY_MOUSE		= 0x57,
    IRKEY_VIDEO		= 0x0E,
    IRKEY_MUSIC		= 0x13,
    IRKEY_PICTURE		= 0x11,
    IRKEY_MYTV		= 0x19,
    IRKEY_VOLUP		= 0x01,
    IRKEY_VOLDOWN		= 0x1E,
    IRKEY_START		= 0x0D,
    IRKEY_MUTE		= 0x00,
    IRKEY_CHUP		= 0x4B,
    IRKEY_CHDOWN		= 0x4F,
    IRKEY_1			= 0x1F,
    IRKEY_2			= 0x03,
    IRKEY_3			= 0x04,
    IRKEY_4			= 0x05,
    IRKEY_5			= 0x1C,
    IRKEY_6			= 0x06,
    IRKEY_7			= 0x07,
    IRKEY_8			= 0x08,
    IRKEY_9			= 0x1D,
    IRKEY_0			= 0x0A,
    IRKEY_ASTERISK	= 0x09,		//*
    IRKEY_NUMBER		= 0x0B,		//#
    IRKEY_AUDIO		= 0x4A,
    IRKEY_CLEAR		= 0x49,
    IRKEY_ENTER		= 0x4C,
    IRKEY_TITLE		= 0x53,

    IRKEY_DVD		= 0x52,
    IRKEY_RADIO		= 0x51,
    IRKEY_GUIDE		= 0x50,
    IRKEY_RTV		= 0x4D,


};


#ifdef	_IRFUN_C__
#define	IRFUNC_EXTN
#else
#define	IRFUNC_EXTN extern
#endif
#endif
