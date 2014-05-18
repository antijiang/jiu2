#define _I2C_C_


#include "general.h"
#ifdef	NVRAM_USE_EEP24CXX
#include "	nvram.h"
/////////////////////////////////////////
// Set I2C SCL pin high/low.
//
// Arguments: bSet - high/low bit
/////////////////////////////////////////
void i2cSetSCL_Chk(bit bSet)
{
    BYTE ucDummy; // loop dummy

    i2cSetSCL(bSet); // set SCL pin

    if (bSet == _HIGH) // if set pin high
    {
        ucDummy = I2C_CHECK_PIN_DUMMY; // initialize dummy
        while ((i2cSCL_PIN_STATUS() == _LOW) && (ucDummy--)) ; // check SCL pull high
    }
}

/////////////////////////////////////////
// Set I2C SDA pin high/low
//
// Arguments: bSet - high/low bit
/////////////////////////////////////////
void i2cSetSDA_Chk(bit bSet)
{
    BYTE ucDummy; // loop dummy

    i2cSetSDA(bSet); // set SDA pin

    if (bSet == _HIGH) // if set pin high
    {
        ucDummy = I2C_CHECK_PIN_DUMMY; // initialize dummy
        while ((i2cSDA_PIN_STATUS() == _LOW) && (ucDummy--)) ; // check SDA pull high
    }
}
//////////////////////////////////////////////////////////////
// Delay 4us
//////////////////////////////////////////////////////////////
void i2c_Delay(void)
{
    BYTE i;
    for (i = 0; i < 30; i++)
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}
//////////////////////////////////////////////////////
// I2C start signal.
// <comment>
//  SCL ________
//              \_________
//  SDA _____
//           \____________
//
// Return value: None
//////////////////////////////////////////////////////
BOOL i2c_Start(void)
{
    BOOL bStatus = TRUE; // success status

    i2cSetSDA_Chk(_HIGH);
    i2c_Delay();
    i2cSetSCL_Chk(_HIGH);
    i2c_Delay();

    if ((i2cSCL_PIN_STATUS() == _LOW) || (i2cSDA_PIN_STATUS() == _LOW))
    {
        bStatus = FALSE;
    }
    else
    {
        i2cSetSDA(_LOW);
        i2c_Delay();
        i2cSetSCL(_LOW);
        i2c_Delay();//2013
    }

    return bStatus;
}
/////////////////////////////////////////
// I2C stop signal.
// <comment>
//              ____________
//  SCL _______/
//                 _________
//  SDA __________/
/////////////////////////////////////////
void i2c_Stop(void)
{
    i2cSetSCL(_LOW);
    i2c_Delay();
    i2cSetSDA(_LOW);
    i2c_Delay();
    i2cSetSCL(_HIGH);
    i2c_Delay();
    i2cSetSDA(_HIGH);
    i2c_Delay();
}

//////////////////////////////////////////////////////////////////////////
// I2C send byte to device.
//
// Arguments: ucVal - send byte
// Return value: I2C acknowledge bit
//               I2C_ACKNOWLEDGE/I2C_NON_ACKNOWLEDGE
//////////////////////////////////////////////////////////////////////////
BOOL i2c_SendByte(BYTE ucVal)
{
    BYTE    ucMask = 0x80;
    BOOL     bAck; // acknowledge bit

    while(ucMask)
    {
        if (ucVal & ucMask)
            i2cSetSDA_Chk(_HIGH);
        else
            i2cSetSDA_Chk(_LOW);
        i2c_Delay();
        i2cSetSCL_Chk(_HIGH); // clock
        i2c_Delay();
        i2cSetSCL(_LOW);
        i2c_Delay();

        ucMask >>= 1; // next
    } // while

    // recieve acknowledge
    i2cSetSDA(_HIGH);
    i2c_Delay();
    i2cSetSCL(_HIGH);
    i2c_Delay();
    bAck = i2cSDA_PIN_STATUS(); // recieve acknowlege
    i2cSetSCL(_LOW);
    i2c_Delay();//2103
    return (bAck);
}
//////////////////////////////////////////////////////////////////////////
// I2C receive byte from device.
//
// Return value: receive byte
//////////////////////////////////////////////////////////////////////////
BYTE i2c_ReceiveByte(BOOL bAck)
{
    BYTE    ucReceive = 0;
    BYTE    ucMask = 0x80;

    while(ucMask)
    {
        i2cSetSDA(_HIGH);
        i2cSetSCL(_HIGH);
i2c_Delay();//2013
        if(i2cSDA_PIN_STATUS() == _HIGH)
            ucReceive |= ucMask;

        i2cSetSCL(_LOW);
        i2c_Delay();

        ucMask >>= 1;
    } // while

    if (bAck) // acknowledge
        i2cSetSDA_Chk(I2C_ACKNOWLEDGE);
    else // non-acknowledge
        i2cSetSDA_Chk(I2C_NON_ACKNOWLEDGE);

    i2c_Delay();
    i2cSetSCL(_HIGH);
    i2c_Delay();
    i2cSetSCL(_LOW);
    i2c_Delay();//2013
    return ucReceive;
}

//////////////////////////////////////////////////////////////////////////
// I2C access start.
//
// Arguments: ucSlaveAdr - slave address
//            trans_t - I2C_TRANS_WRITE/I2C_TRANS_READ
//////////////////////////////////////////////////////////////////////////
BOOL i2c_AccessStart(BYTE ucSlaveAdr, I2C_Direction trans_t)
{
    BYTE ucDummy; // loop dummy

    if (trans_t == I2C_READ) // check i2c read or write
        ucSlaveAdr = I2C_DEVICE_ADR_READ(ucSlaveAdr); // read
    else
        ucSlaveAdr = I2C_DEVICE_ADR_WRITE(ucSlaveAdr); // write

    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while (ucDummy--)
    {
        i2c_Delay();
        if (i2c_Start() == FALSE)
            continue;

        if (i2c_SendByte(ucSlaveAdr) == I2C_ACKNOWLEDGE) // check acknowledge
            return TRUE;
        //printf("ucSlaveAdr====%x", ucSlaveAdr);
        i2c_Stop();
        Delay1ms(2);
    }

    return FALSE;
}
void Delay1ms(WORD wValue)
{
    WORD wCount; // loop counter
    extern 	void DelayXms(WORD td) ;
    DelayXms(wValue);
    return;
    while (wValue--)
    {
        wCount = DELAY_1MS_PERIOD; // reset loop counter
        //wCount = g_ucTestCnt; // reset loop counter

        //  WatchDogClear();
        while (wCount--) ;
    } // while
}
#if	RM_EEPROM_TYPE<RM_TYPE_24C32
/////////////////////////////////////////////////////////////////
// I2C write bytes to device.
//
// Arguments: ucSlaveAdr - slave address
//            ucSubAdr - sub address
//            pBuf - pointer of buffer
//            ucBufLen - length of buffer
/////////////////////////////////////////////////////////////////
void i2cBurstWriteBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy
    BYTE tmp;
    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(ucSlaveAdr, I2C_WRITE) == FALSE)
            continue;
        if (i2c_SendByte(ucSubAdr) == I2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        while(ucBufLen--) // loop of writting data
        {
            tmp = *pBuf;
            i2c_SendByte(tmp); // send byte
            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    i2c_Stop();
 
}

/////////////////////////////////////////////////////////////////
// I2C read bytes from device.
//
// Arguments: ucSlaveAdr - slave address
//            ucSubAdr - sub address
//            pBuf - pointer of buffer
//            ucBufLen - length of buffer
/////////////////////////////////////////////////////////////////

void i2cBurstReadBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen)
{
    BYTE ucDummy; // loop dummy
    BYTE tmp;
    ucDummy = I2C_ACCESS_DUMMY_TIME;
    while(ucDummy--)
    {
        if (i2c_AccessStart(ucSlaveAdr, I2C_WRITE) == FALSE)
            continue;

        if (i2c_SendByte(ucSubAdr) == I2C_NON_ACKNOWLEDGE) // check non-acknowledge
            continue;

        if (i2c_AccessStart(ucSlaveAdr, I2C_READ) == FALSE)
            continue;

        while(ucBufLen--) // loop to burst read
        {
            tmp =   i2c_ReceiveByte(ucBufLen); // receive byte
            *pBuf = tmp;

            pBuf++; // next byte pointer
        } // while

        break;
    } // while

    i2c_Stop();
}





#endif

#if 1
void i2c_WriteTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count)
{
    i2cBurstWriteBytes( deviceID, addr, buffer, count);
}

void i2c_ReadTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count)
{
    i2cBurstReadBytes(deviceID, addr, buffer, count);
}


#endif


#endif
