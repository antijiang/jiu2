#ifndef	_KEY_DEF_H_
#define	  _KEY_DEF_H_



enum{

KEY_RANCODE	  ,
KEY_JIUJING	  ,
KEY_DISPLAY,
KEY_MODE	  ,
KEY_ADJUST,
KEY_TESTRF,
KEY_DISPSTD,
KEY_EON,	
KEY_NOKEY=0xff,
IRKEY_NOKEY=KEY_NOKEY,
};
void  DO_Key_Action();
BYTE PI_ADJUST_DET();
BYTE PI_ADJUST_DET20();
BYTE CalALCLevel(WORD untmg);
#endif
