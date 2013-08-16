#define _NVRAM_C_

#include "general.h"

///////////////////////////////////////////////////////////////////////////////
// <Function>: rmBurstReadBytes
//
// <Description>: EEPROM burst read bytes
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
void rmBurstReadBytes(WORD wIndex, BYTE *pBuf, WORD ucBufLen)
{
#if (RM_EEPROM_TYPE == RM_TYPE_24C16)
    i2cBurstReadBytes(rmCalDeviceAdr(wIndex), LOBYTE(wIndex), pBuf, ucBufLen);
#elif (RM_EEPROM_TYPE == RM_TYPE_24C32)
    BYTE ucDummy; // loop dummy

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(NVRAM_DEVICE, I2C_WRITE) == FALSE)
            continue;

        if (i2c_SendByte(HIBYTE(wIndex)) == I2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        if (i2c_SendByte(LOBYTE(wIndex)) == I2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        if (i2c_AccessStart(NVRAM_DEVICE, I2C_READ) == FALSE)
            continue;

        while(ucBufLen--) // loop to burst read
        {
            *pBuf = i2c_ReceiveByte(ucBufLen); // receive byte

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    i2c_Stop();
#endif
}
///////////////////////////////////////////////////////////////////////////////
// <Function>: rmPageWrite
//
// <Description>: EEPROM page write

// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
#if (RM_EEPROM_TYPE == RM_TYPE_24C32)
void rmPageWrite(WORD wIndex, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(NVRAM_DEVICE, I2C_WRITE) == FALSE)
            continue;

        if (i2c_SendByte(HIBYTE(wIndex)) == I2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        if (i2c_SendByte(LOBYTE(wIndex)) == I2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        while(ucBufLen--) // loop of writting data
        {
            i2c_SendByte(*pBuf); // send byte

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    i2c_Stop();
}
#endif
///////////////////////////////////////////////////////////////////////////////
// <Function>: rmBurstWriteBytes
//
// <Description>: EEPROM burst write bytes
//
// <Parameter>:  -  <Flow>  -  <Description>
//   wIndex      -    In    -   index
//   pBuf        -    Out   -   buffer pointer
//   ucBufLen    -    In    -   buffer length
//-----------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////////
void rmBurstWriteBytes(WORD wIndex, BYTE *pBuf, WORD ucBufLen)
{
    BYTE ucWriteCount; // write counter
    BYTE ucRemainLen; // i2c word address

    while(1)
    {
        // check page write roll over
        if (ucBufLen > RM_PAGE_WRITE_LEN)
            ucWriteCount = RM_PAGE_WRITE_LEN;
        else
            ucWriteCount = ucBufLen;

        // check word address higher than page write bits
        ucRemainLen = LOBYTE(wIndex) % RM_PAGE_WRITE_LEN;
        if (ucRemainLen) // check ramain
        {
            ucRemainLen = RM_PAGE_WRITE_LEN - ucRemainLen;
            if (ucWriteCount > ucRemainLen) // check not roll over
                ucWriteCount = ucRemainLen;
        }

        // EEPROM page write
#if (RM_EEPROM_TYPE == RM_TYPE_24C16)
        i2cBurstWriteBytes(rmCalDeviceAdr(wIndex), LOBYTE(wIndex), pBuf, ucWriteCount);
#elif (RM_EEPROM_TYPE == RM_TYPE_24C32)
        rmPageWrite(wIndex, pBuf, ucWriteCount);
#endif
        // check end of buffer
        ucBufLen -= ucWriteCount;
        if (ucBufLen == 0)
            break;

        wIndex += ucWriteCount; // next word address
        pBuf += ucWriteCount; // next buffer pointer

        Delay1ms(1);
    } // while

}

void NVRam_WriteByte(WORD addr, BYTE value)
{
	rmBurstWriteBytes(addr, &value, 1);
}
void NVRam_ReadByte(WORD addr, BYTE *value)
{
	rmBurstReadBytes(addr, value, 1);
}
void NVRam_WriteTbl(WORD addr, BYTE *buffer, WORD count)
{
 	rmBurstWriteBytes(addr, buffer, count);
}
void NVRam_ReadTbl(WORD addr, BYTE *buffer, WORD count)
{
	rmBurstReadBytes(addr, buffer, count);
}


