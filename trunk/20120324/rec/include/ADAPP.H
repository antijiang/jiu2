#ifndef	_ADAPP_H_
#define	_ADAPP_H_

#ifdef	_ADAPP_C_	
#define	ADAPP_TYPE_INI(a,init_val)	a =init_val
#define	ADAPP_TYPE(a)	a 
#else
#define	ADAPP_TYPE_INI(a,init_val)	extern a
#define	ADAPP_TYPE(a)	extern a 
#endif

 /********************************************
******************************************/
enum _AD_PORT_
{	MINNUMBER_PORT_AD=0,
	ADPORT_TempaSensor=MINNUMBER_PORT_AD ,
	
	ADPORT_POWER,
	ADPORT_SENSOR,
	ADPORT_GND,
	ADPORT_BAT,
	MAXNUMBER_PORT_AD,

};

typedef struct _SYSINFO_
{   
  
  DWORD  TRANCODE ;
  DWORD  TRANCODE1 ;
  BYTE chechsum;
        
}_SYSINFO; 

	

//#define	CODE_RESERVE_SPACE	512
typedef enum _EEPADDR_IDX{
    
 
	EEPROM_ADDR_START=FLASH_TEMP-512,  //从最上部向下 保留空间 CODE_RESERVE_SPACE
	EEP_VIRT_ADD= EEPROM_ADDR_START,


	EEP_SYS_TBLSTART,
	EEP_SYS_LENTH=sizeof(_SYSINFO),	 //WORD 
	EEP_SYS_TBLEND=EEP_SYS_TBLSTART+EEP_SYS_LENTH,

	//-----------------------------------------------------------------------------------------
	


	
	EEPROM_ID,
	EEPROM_ID_LENGTH=1,
	EEPROM_ID_END=EEPROM_ID+EEPROM_ID_LENGTH,
	EEP_ADDR_END, //长度
	EEP_ADDR_ALIG_LEN=EEP_ADDR_END/FLASH_PAGESIZE*FLASH_PAGESIZE+FLASH_PAGESIZE,
	EEP_ADDR_MAX_END=FLASH_LAST,//0x7A00,	//实际的最后地址

	
//	EEPROM_ADDR_START=EEP_ADDR_MAX_END-	EEP_ADDR_END	 , //flash 中的起始地址


};
#define	EEP_LENGTH  (EEP_ADDR_END-EEP_VIRT_ADD+1)
#define	EEP_ADDR_ALIG_LEN  EEP_LENGTH/FLASH_PAGESIZE*FLASH_PAGESIZE+FLASH_PAGESIZE

#define	CODE_RESERVE_SPACE	EEP_ADDR_ALIG_LEN



//#if  CODE_RESERVE_SPACE< EEP_LENGTH
//#error "EEPROM not Full to Allocate"
//#endif







#endif
