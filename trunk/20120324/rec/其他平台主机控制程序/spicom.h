//ͷ�ļ�����
//spicom.h
#ifndef	_SPICOM_H_
#define	_SPICOM_H_


#define	CMD_RD_ID	0xF1	//��ȡID
#define	CMD_SET_P54	0xe1  //����P54 ��
#define	CMD_CLR_P54	0xe0  // 
#define	CMD_SET_P00	0xe3
#define	CMD_CLR_P00	0xe2
#define	CMD_RD_P11	0xe4  //��ȡP11�˿ڣ�0
char TEST_SPI(char CMD_a);
#endif