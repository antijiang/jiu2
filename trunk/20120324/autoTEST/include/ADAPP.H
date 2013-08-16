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
  
#ifndef	 NVRAM_USE_EEP24CXX
  WORD chksum; //必须放前头
#else
  BYTE chksum ;
#endif
  BYTE ADJUST;
  WORD VA;
  WORD VB;
  WORD VC;
  DWORD  TRANCODE ;

        
}_SYSINFO; 

#define	MAX_JIULST	20			   //最大存储个数
typedef struct _JIUJING_LST_
{   
  
 
  BYTE idx;		//0;10
  WORD Jvol[MAX_JIULST];
          
}_JIUJING_LST; 
	

//#define	CODE_RESERVE_SPACE	512
typedef enum _EEPADDR_IDX{
    		 
#ifdef	NVRAM_USE_EEP24CXX	 
	EEPROM_ADDR_START=0 ,//外部从零开始
	
#else
//	EEPROM_ADDR_START=FLASH_TEMP-EEPROM_SIZE,  //从最上部向下 保留空间 CODE_RESERVE_SPACE
	EEPROM_ADDR_START=FLASH_TEMP+FLASH_PAGESIZE,  //从最上部向下 保留空间 CODE_RESERVE_SPACE
	
#endif
	EEP_VIRT_ADD= EEPROM_ADDR_START,


	EEP_SYS_TBLSTART,
	EEP_SYS_LENTH=sizeof(_SYSINFO),	 //WORD 
	EEP_SYS_TBLEND=EEP_SYS_TBLSTART+EEP_SYS_LENTH,

	EEP_JIULST_TBLSTART,
	EEP_JIULST_LENTH=sizeof(_JIUJING_LST),	 //WORD 
	EEP_JIULST_TBLEND=EEP_JIULST_TBLSTART+EEP_JIULST_LENTH,


	//-----------------------------------------------------------------------------------------
	


	
	EEPROM_ID,
	EEPROM_ID_LENGTH=1,
	EEPROM_ID_END=EEPROM_ID+EEPROM_ID_LENGTH,
	EEP_ADDR_END, //长度
	EEP_ADDR_ALIG_LEN=EEP_ADDR_END/FLASH_PAGESIZE*FLASH_PAGESIZE+FLASH_PAGESIZE,
	EEP_ADDR_MAX_END=(FLASH_SIZE-FLASH_IC_RESEVERD),//0x7A00,	//实际的最后地址

	
//	EEPROM_ADDR_START=EEP_ADDR_MAX_END-	EEP_ADDR_END	 , //flash 中的起始地址


};
#define	EEP_LENGTH  (EEP_ADDR_END-EEP_VIRT_ADD+1)
#define	EEP_ADDR_ALIG_LEN  EEP_LENGTH/FLASH_PAGESIZE*FLASH_PAGESIZE+FLASH_PAGESIZE

#define	CODE_RESERVE_SPACE	EEP_ADDR_ALIG_LEN



//#if  CODE_RESERVE_SPACE< EEP_LENGTH
//#error "EEPROM not Full to Allocate"
//#endif







#endif
