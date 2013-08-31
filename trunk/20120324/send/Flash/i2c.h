#ifndef _I2C_H_
#define _I2C_H_

#ifdef _I2C_C_
#define _I2CDEC_
#else
#define _I2CDEC_ extern
#endif


typedef enum i2c_direction
{
    I2C_WRITE,
    I2C_READ
} I2C_Direction;

#define I2C_DEVICE_ADR_WRITE(slave_adr)   (slave_adr & ~_BIT0)
#define I2C_DEVICE_ADR_READ(slave_adr)    (slave_adr | _BIT0)
#define I2C_ACCESS_DUMMY_TIME   		  10
#define hwI2C_SCL_Pin				P12
#define hwI2C_SDA_Pin				P06
#define hwI2C_SCL_Pin_STATUS 		(hwI2C_SCL_Pin&0x01)
#define hwI2C_SDA_Pin_STATUS 		(hwI2C_SDA_Pin&0x01)
#define i2cSetSCL(is_hl)  	hwI2C_SCL_Pin=is_hl
#define i2cSetSDA(is_hl)	hwI2C_SDA_Pin=is_hl

#define i2cSCL_PIN_STATUS()	hwI2C_SCL_Pin_STATUS
#define i2cSDA_PIN_STATUS() hwI2C_SDA_Pin_STATUS


#define I2C_CHECK_PIN_TIME   	1000 // unit: 1 us
#define I2C_CHECK_PIN_CYCLE  	8    // cycle of check pin loopp
#define MCU_MICROSECOND_NOP_NUM (1)  //us

//#define MCU_MICROSECOND_NOP_NUM (1/24.5)  //us

#define I2C_CHECK_PIN_DUMMY  	(I2C_CHECK_PIN_TIME / (I2C_CHECK_PIN_CYCLE * MCU_MICROSECOND_NOP_NUM))
#define DELAY_1MS_PERIOD 	(60)
//_I2CDEC_ void i2c_WriteTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count);
//_I2CDEC_ void i2c_ReadTBL(BYTE deviceID, WORD addr, BYTE *buffer, BYTE count);
_I2CDEC_ BOOL i2c_Start(void);
_I2CDEC_ BOOL i2c_SendByte(BYTE ucVal);
_I2CDEC_ void i2c_Stop(void);
_I2CDEC_ BYTE i2c_ReceiveByte(BOOL bAck);
_I2CDEC_ void i2cBurstCurrentBytes(BYTE ucSlaveAdr, BYTE *pBuf, BYTE ucBufLen);
_I2CDEC_ void i2cWriteByte(BYTE deviceID, BYTE addr, BYTE value);
_I2CDEC_ void i2cBurstReadBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen);
_I2CDEC_ void i2cBurstWriteBytes(BYTE ucSlaveAdr, BYTE ucSubAdr, BYTE *pBuf, BYTE ucBufLen);
_I2CDEC_ BOOL i2c_AccessStart(BYTE ucSlaveAdr, I2C_Direction trans_t);
_I2CDEC_  void Delay1ms(WORD wValue)   ;


#endif
