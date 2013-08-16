#ifdef _GLOBAL_C_
  #define _GLOBALDEC_
#else
  #define _GLOBALDEC_ extern
#endif
_GLOBALDEC_ XDATA BYTE ucUartCheckTick;
_GLOBALDEC_ XDATA UartCommadType g_UartCommand; // Uart command struct
_DEBUGDEC_ bit g_bUartDetected; // decode command flag

_DEBUGDEC_ void uartDecodeCommand(void);
_DEBUGDEC_ void uartDecodeNormalCommand(void);
_DEBUGDEC_ void uartDecodeExtCommand(void);


#define UART_CMD_LENGTH     (g_UartCommand.Buffer[_UART_CMD_CODE_] >> 5)
#define UART_CMD            (g_UartCommand.Buffer[_UART_CMD_CODE_] & 0x1F)
#define UART_CMD_MS_REG     (g_UartCommand.Buffer[_UART_CMD_INDEX1_])
#define UART_CMD_MS_DAT     (g_UartCommand.Buffer[_UART_CMD_INDEX2_])


#define UART_CMD_MS_BANK     (g_UartCommand.Buffer[_UART_CMD_INDEX1_])
#define UART_CMD_MS_REGINDEX (g_UartCommand.Buffer[_UART_CMD_INDEX2_])
#define UART_CMD_MS_REGDATA  (g_UartCommand.Buffer[_UART_CMD_INDEX3_])

#define UART_CMD_XDATA_ADD_H (g_UartCommand.Buffer[_UART_CMD_INDEX2_])
#define UART_CMD_XDATA_ADD_L (g_UartCommand.Buffer[_UART_CMD_INDEX3_])
#define UART_CMD_XDATA_DATA  (g_UartCommand.Buffer[_UART_CMD_INDEX4_])

// for MST Banks
#define UART_CMD_MCU_IDX_H   (g_UartCommand.Buffer[_UART_CMD_INDEX1_])
#define UART_CMD_MCU_IDX_L   (g_UartCommand.Buffer[_UART_CMD_INDEX2_])
#define UART_CMD_MCU_DATA    (g_UartCommand.Buffer[_UART_CMD_INDEX3_])

// for EXT Command
#define UART_EXT             (UART_CMD_LENGTH == 7)
#define UART_CMD_EXT_LENGTH  (g_UartCommand.Buffer[_UART_CMD_INDEX1_])
#define UART_EXT_DEV         (g_UartCommand.Buffer[_UART_CMD_INDEX2_])
#define UART_EXT_CMD         (g_UartCommand.Buffer[_UART_CMD_INDEX3_])
#define UART_EXT_CMD_MS_DAT1 (g_UartCommand.Buffer[_UART_CMD_INDEX4_])
#define UART_EXT_CMD_MS_DAT2 (g_UartCommand.Buffer[_UART_CMD_INDEX5_])
#define UART_EXT_CMD_MS_DAT3 (g_UartCommand.Buffer[_UART_CMD_INDEX6_])
#define UART_EXT_CMD_MS_DAT4 (g_UartCommand.Buffer[_UART_CMD_INDEX7_])
#define UART_EXT_CMD_MS_DAT5 (g_UartCommand.Buffer[_UART_CMD_INDEX8_])
#define UART_EXT_CMD_MS_DAT6 (g_UartCommand.Buffer[_UART_CMD_INDEX9_])
#define UART_EXT_CMD_MS_DAT7 (g_UartCommand.Buffer[_UART_CMD_INDEX10_])
#define UART_EXT_CMD_MS_DAT8 (g_UartCommand.Buffer[_UART_CMD_INDEX11_])
#define UART_EXT_CMD_MS_DAT9 (g_UartCommand.Buffer[_UART_CMD_INDEX12_])
#define UART_EXT_CMD_MS_DAT10 (g_UartCommand.Buffer[_UART_CMD_INDEX13_])
#define UART_EXT_CMD_MS_DAT11 (g_UartCommand.Buffer[_UART_CMD_INDEX14_])
#define UART_EXT_CMD_MS_DAT12 (g_UartCommand.Buffer[_UART_CMD_INDEX15_])
#define UART_EXT_CMD_MS_DAT13 (g_UartCommand.Buffer[_UART_CMD_INDEX16_])

#define UART_CMD_FLASH_LENGTH    (g_UartCommand.Buffer[_UART_CMD_CODE_] >> 3)
#define UART_CMD_FLASH_ADDR0    (g_UartCommand.Buffer[_UART_CMD_INDEX1_])
#define UART_CMD_FLASH_ADDR1    (g_UartCommand.Buffer[_UART_CMD_INDEX2_])
#define UART_CMD_FLASH_ADDR2    (g_UartCommand.Buffer[_UART_CMD_INDEX3_])
#define UART_CMD_FLASH_DATA     (g_UartCommand.Buffer[_UART_CMD_INDEX1_])
#define UART_FLASH_CMD          (g_UartCommand.Buffer[_UART_CMD_CODE_] & 0x07)

#define _EOS_ '\0' // end of string

/////////////////////////////////////////////
// Uart command structure
/////////////////////////////////////////////
typedef enum _UartCommadEnumType
{
    _UART_CMD_CODE_,
    _UART_CMD_INDEX1_,
    _UART_CMD_INDEX2_,
    _UART_CMD_INDEX3_,
    _UART_CMD_INDEX4_,
    _UART_CMD_INDEX5_,
    _UART_CMD_INDEX6_,
    _UART_CMD_INDEX7_,
    _UART_CMD_INDEX8_,
    _UART_CMD_INDEX9_,
    _UART_CMD_INDEX10_,
    _UART_CMD_INDEX11_,
    _UART_CMD_INDEX12_,
    _UART_CMD_INDEX13_,
    _UART_CMD_INDEX14_,
    _UART_CMD_INDEX15_,
    _UART_CMD_INDEX16_,

    _UART_CMD_LENGTH_
}UartCommadEnumType;

typedef struct _UartCommadType
{
    BYTE Buffer[_UART_CMD_LENGTH_];  // command buffer
                                     // [Length+Command] - [Byte 2] - [Byte 3] - [Byte 4] - [Byte 5]
    BYTE Index;                     // buffer index

}UartCommadType;

