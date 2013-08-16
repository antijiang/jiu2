/***********************************************************************/
/*  This file is part of the C51 Compiler package                      */
/*  Copyright KEIL ELEKTRONIK GmbH 1990 - 2002                         */
/***********************************************************************/
/*                                                                     */
/*  PUTCHAR.C:  This routine is the general character output of C51.   */
/*  You may add this file to a uVision2 project.                       */
/*                                                                     */
/*  To translate this file use C51 with the following invocation:      */
/*     C51 PUTCHAR.C <memory model>                                    */
/*                                                                     */
/*  To link the modified PUTCHAR.OBJ file to your application use the  */
/*  following Lx51 invocation:                                         */
/*     Lx51 <your object file list>, PUTCHAR.OBJ <controls>            */
/*                                                                     */
/***********************************************************************/

#include "c8051F340.h"
#include "config.h"
#define XON  0x11
#define XOFF 0x13
#define	SBUF		SBUF0
#define	TI			TI0
#define	RI			RI0
#define	ES			ES0

#define	TI1_SET()	 (SCON1|=0x02)
#define	TI1_CLR()	 (SCON1&=~0x02)
#define	RI1_SET()	(SCON1|=0x01)	
#define	RI1_CLR()	(SCON1&=~0x01)	

#define	TI1()		(SCON1&0x02)	
#define	RI1()		(SCON1&0x01)	

#define	ES1_EN()	 (EIE2|=0x02)
#define	ES1_DISABLE()	 (EIE2&=~0x02)
#if 1
/*
 * putchar (full version):  expands '\n' into CR LF and handles
 *                          XON/XOFF (Ctrl+S/Ctrl+Q) protocol
 */
char putchar1 (char c)  {
  
  	ES = 0;
#if 0
	if (c == '\n')  {
	    SBUF = 0x0d;                         /* output CR  */
	} 
  else 
#endif
  	SBUF = c; // transfer to uart
    // wait transfer completing
    while(1) { if (TI) break; } // check flag

    TI = 0; // clear flag
	ES=1;
 	return c;
}

#endif

#if 0

/*
 * putchar (full version):  expands '\n' into CR LF and handles
 *                          XON/XOFF (Ctrl+S/Ctrl+Q) protocol
 */
char putchar (char c)  {

  if (c == '\n')  {
    if (RI)  {
      if (SBUF == XOFF)  {
        do  {
          RI = 0;
          while (!RI);
        }
        while (SBUF != XON);
        RI = 0; 
      }
    }
    while (!TI);
    TI = 0;
    SBUF = 0x0d;                         /* output CR  */
  }
  if (RI)  {
    if (SBUF == XOFF)  {
      do  {
        RI = 0;
        while (!RI);
      }
      while (SBUF != XON);
      RI = 0; 
    }
  }
  while (!TI);
  TI = 0;
  return (SBUF = c);
}

#endif

#if 0         // comment out versions below

/*
 * putchar (basic version): expands '\n' into CR LF
 */
char putchar (char c)  {
  if (c == '\n')  {
    while (!TI);
    TI = 0;
    SBUF = 0x0d;                         /* output CR  */
  }
  while (!TI);
  TI = 0;
  return (SBUF = c);
}


/*
 * putchar (mini version): outputs charcter only
 */
char putchar (char c)  {
  while (!TI);
  TI = 0;
  return (SBUF = c);
}

#endif
#ifdef	EN_UART1
char putchar (char c)  {
  
  	ES1_DISABLE();
#if 0
	if (c == '\n')  {
	    SBUF = 0x0d;                         /* output CR  */
	} 
  else 
#endif
  	SBUF1= c; // transfer to uart
//     wait transfer completing
    while(1) { if (TI1()) break; } // check flag

    	TI1_CLR(); // clear flag
	ES1_EN();
 	return c;
}
#endif
