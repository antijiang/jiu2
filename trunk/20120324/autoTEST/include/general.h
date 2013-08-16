#ifndef	_GENERAL_H_
#define _GENERAL_H_

#include "config.h"

#include <stdio.h>
#include <intrins.h>
#include "mytype.h"
#include "c8051f330.h"   
#include <string.h>

#include "F410_FlashPrimitives.h"
#ifndef		NVRAM_USE_EEP24CXX
#include "F410_FlashUtils.h"// SFR declarations
#endif
#ifdef	NVRAM_USE_EEP24CXX
#include "i2c.h"
#include "Nvram.h"
#endif

#include "UART.h"
//#include "var.h"
//#include "int.h"
#include "keydef.h"
#include "system.h"
#include "debug.h"
#include "int.h"

#include "LCD_DRV.h"
#include "dali_drv.h"
#include "adapp.h"
#endif

