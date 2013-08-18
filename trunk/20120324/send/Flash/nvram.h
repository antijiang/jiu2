
#ifndef _NVRAM_H_
#define _NVRAM_H_

//-----------EEprom Define-------------------
#define RM_TYPE_24C01			0
#define RM_TYPE_24C02			1
#define RM_TYPE_24C04			2
#define RM_TYPE_24C08			3
#define RM_TYPE_24C16			4
#define RM_TYPE_24C32			6
#define RM_TYPE_SST512			6
#define RM_TYPE_SST010			7
#define RM_TYPE_PMC512			8
#define RM_TYPE_PMC010			9
#define RM_TYPE_SAI512			10
#define RM_TYPE_SAI010			11

#define RM_EEPROM_TYPE	 		RM_TYPE_24C02  //RM_TYPE_24C32
#define NVRAM_DEVICE			0xA0

#if RM_EEPROM_TYPE>=RM_TYPE_24C32
#define rmCalDeviceAdr(wIndex)    NVRAM_DEVICE|((HIBYTE(wIndex)&0x07)<<1)
#else
#define rmCalDeviceAdr(wIndex)    NVRAM_DEVICE
#endif

#if (RM_EEPROM_TYPE <= RM_TYPE_24C02)
#define RM_PAGE_WRITE_LEN    8
#elif (RM_EEPROM_TYPE <= RM_TYPE_24C16)
#define RM_PAGE_WRITE_LEN    16
#elif (RM_EEPROM_TYPE == RM_TYPE_24C32)
#define RM_PAGE_WRITE_LEN    32
#endif



#ifdef _NVRAM_C_
#define _NVRAMDEC_
#else
#define _NVRAMDEC_ extern
#endif

_NVRAMDEC_ void NVRam_WriteByte(WORD addr, BYTE value);
_NVRAMDEC_ void NVRam_ReadByte(WORD addr, BYTE *value);
_NVRAMDEC_ void rmBurstWriteBytes(WORD wIndex, BYTE *pBuf, WORD ucBufLen);
_NVRAMDEC_ void NVRam_WriteTbl(WORD addr, BYTE *buffer, WORD count);
_NVRAMDEC_ void NVRam_ReadTbl(WORD addr, BYTE *buffer, WORD count);
void FLASH_Update (WORD dest, BYTE *src, unsigned int numbytes) ;
char *FLASH_Read (BYTE *dest, WORD  src, unsigned int numbytes);


#endif
