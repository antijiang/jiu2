
#define	INIT_UART_RECBUF g_UartCommand.Index=0		
/******/
//2 seconde time out			
void UartRecTimeOutCheck (void)
{			
 				if (ucUartCheckTick)
                if ((--ucUartCheckTick)==0)
                    g_UartCommand.Index = 0;	
}										
///////////////////////////////////////
// Uart Interrupt
///////////////////////////////////////
void uartInterrupt(void) interrupt 4
{
//    EA = 0; // disable all interrupt
#if _DEBUG_EXTDEV_EN_
    if (!_testbit_(TI)) // check translate interrupt
    {
        ucUartCheckTick = 2;

        RI = 0; // clear recieve interrupt

        g_UartCommand.Buffer[g_UartCommand.Index] = SBUF; // recieve byte

        // check command buffer index
        if (UART_EXT)
        {
            if (g_UartCommand.Index == 0) // check 1st data
                g_UartCommand.Index++; // for get UART_CMD_EXT_LENGTH
            else
            {
                if (g_UartCommand.Index < UART_CMD_EXT_LENGTH) // still read command
                    g_UartCommand.Index++; // next index of command buffer
                if (g_UartCommand.Index >= UART_CMD_EXT_LENGTH) // read command ok
                {
                xx
                    g_bUartDetected = TRUE; // command  buffer recieve ok
                    g_UartCommand.Index = 0; // reset index of command buffer
                    ucUartCheckTick = 0;
                }
            }
        }
        else
        {
            if (g_UartCommand.Index < UART_CMD_LENGTH) // still read command
                g_UartCommand.Index++; // next index of command buffer
            if (g_UartCommand.Index >= UART_CMD_LENGTH) // read command ok
            {
                g_bUartDetected = TRUE; // command  buffer recieve ok
                g_UartCommand.Index = 0; // reset index of command buffer
                ucUartCheckTick = 0;
            }
        }
    } // if TI
#endif		
//    EA = 1; // release EA bit
}
