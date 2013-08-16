//-----------------------------------------------------------------------------
// F410_FlashPrimitives.h
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program contains several useful utilities for writing and updating
// FLASH memory.
//
// FID:            41X000051
// Target:         C8051F410/1/2/3
// Tool chain:     Keil C51 8.00
// Command Line:   None
//
//
// Release 1.0
//    -Initial Revision (GP)
//    -29 SEP 2006
//

//-----------------------------------------------------------------------------
// Open Header #define
//-----------------------------------------------------------------------------

#ifndef _F410_FLASHPRIMITIVES_H_
#define _F410_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------

//typedef unsigned long ULONG;
//typedef unsigned int  UINT;
//typedef unsigned char UCHAR;
typedef unsigned int FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 512
#endif

//#define	FLASH_SIZE	(0xffffL+1)  //
//#define	FLASH_SIZE	(0x7fffL+1)
//#define	FLASH_SIZE	(0x3fffL+1)
#define	FLASH_SIZE	(0x1fffL+1)




//空间交换
#ifndef FLASH_TEMP
//#define FLASH_TEMP (0xfa00L-FLASH_PAGESIZE)             // For 64K Flash devices
//#define FLASH_TEMP (0x7a00L-FLASH_PAGESIZE)             // For 32K Flash devices
//#define FLASH_TEMP (0x3a00L-FLASH_PAGESIZE)           // For 16K Flash devices
#define FLASH_TEMP (0x1800L )          // For 8K Flash devices	  1a00-1bff
#endif

//系统保留
#ifndef FLASH_LAST
//#define FLASH_LAST 0xfa00L             // For 64K Flash devices
//#define FLASH_LAST 0x7a00L             // For 32K Flash devices
//#define FLASH_LAST 0x3a00L           // For 16K Flash devices
#define FLASH_LAST 0x1E00L           // For 8K Flash devices   1e00-1dff
#endif

//模拟的EEPROM大小
#define		EEPROM_SIZE  512
//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------

void          FLASH_ByteWrite (FLADDR addr, char byte);
unsigned char FLASH_ByteRead  (FLADDR addr);
void          FLASH_PageErase (FLADDR addr);

//-----------------------------------------------------------------------------
// Close Header #define
//-----------------------------------------------------------------------------

#endif    // _F410_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------