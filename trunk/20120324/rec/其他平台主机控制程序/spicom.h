//头文件如下
//spicom.h
#ifndef	_SPICOM_H_
#define	_SPICOM_H_


#define	CMD_RD_ID	0xF1	//读取ID
#define	CMD_SET_P54	0xe1  //设置P54 高
#define	CMD_CLR_P54	0xe0  // 
#define	CMD_SET_P00	0xe3
#define	CMD_CLR_P00	0xe2
#define	CMD_RD_P11	0xe4  //读取P11端口：0
char TEST_SPI(char CMD_a);
#endif