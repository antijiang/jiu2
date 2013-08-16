
#ifndef __UART_H__
#define __UART_H__


void InitUART(void);
void UartPutChar1(uint8);
void UartPutChar(uint8);
void Prints(uint8 *);
void PrintLongInt(uint32);
void PrintShortIntHex(uint16 x);
void Printc(uint8);
void PrintHex(uint8 x);
char putchar1 (char c);
#endif
