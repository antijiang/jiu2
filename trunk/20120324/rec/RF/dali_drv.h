#ifndef _DALI_DRV_H_1
#define _DALI_DRV_H_1

void DALI_Send_Start(void);
void DALI_Init(void);	
void DALI_SendData(void)	 ;
void DALI_Send(WORD Thead,BYTE Tcode );


BYTE WaitDALIAnswer(void );
				
#ifdef	_DALI_DRV_C_	
#define	DALIDRV_TYPE_INI(a,init_val)	a =init_val
#define	DALIDRV_TYPE(a)	a 
#else
#define	DALIDRV_TYPE_INI(a,init_val)	extern a
#define	DALIDRV_TYPE(a)	extern a 
#endif

DALIDRV_TYPE(BYTE f_dalitx);                                                       
DALIDRV_TYPE(BYTE f_dalirx);


	
#endif

 
