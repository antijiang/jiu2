#ifndef	_SPICOM_IN_H_
#define	_SPICOM_IN_H_


//公共接口程序文件如下
void 		Set_MCLK_Mode_IN(void)		;
void 		Set_MCLK_Mode_OUT(void)	;
void		Set_SCLK_Mode_IN(void)		;
void		Set_SCLK_Mode_OUT(void)	;
void		Set_SDA_Mode_IN(void)		;
void		Set_SDA_Mode_OUT(void)		;
void		SetMCLK()	;
void		ClrMCLK()	;
void		SetSCLK() 	;
void		ClrSCLK() 	;
void		SetSDA()  	;
void		ClrSDA()	;
void 		Delay5us(void);
void 		InitSPICom(void);
char 		GetMCLK()	;
char		GetSCLK()	;	
char		GetSDA()	;	

#endif