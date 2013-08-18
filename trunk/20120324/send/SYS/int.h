#ifndef	_INT_H_
#define		  _INT_H_


#define     CUSTOMER_CODE	0x02
#define     CUSTOMER_CODE1	0x30	//PANEL 
#define	PCA_FOR_IR
#define     IR_NEC
//#define CODE_CONTINUE
//#define CHECK_IR_CUSTOMER_CODE
//if you want to check customer code
// Your customer definition is in "CUSTOMER_CODE"

#define	SYSTEMTM_COUNT_BASE	10		//ϵͳ��׼��ʱ ��λN ms

#if	defined	_INTC_
#define	EXTERN_INTC
#else
#define	EXTERN_INTC	extern
#endif
typedef struct _IR_RECEIVE
{
    BYTE stateIR	 ;
    BYTE User_Code	 ;
    BYTE User_Code1;
    BYTE IR_code	 ;
    BYTE gbCodeKeep;
    BYTE 	REMOTE_CF;
    BYTE RemoteCTM;
} IR_RECEIVE;

EXTERN_INTC BYTE data  stateIR;
EXTERN_INTC BYTE idata User_Code;	/*the custom code*/
EXTERN_INTC BYTE idata User_Code1;	/*the custom code*/
EXTERN_INTC BYTE data IR_code;		/*****IR code ********/
EXTERN_INTC BYTE idata gbCodeKeep;  /* Record the previous ID code  */
EXTERN_INTC BYTE RemoteCTM;	//�����ͷź��ʱ
EXTERN_INTC BYTE RC_Key;		//
EXTERN_INTC BOOL codeIR_Valid;
EXTERN_INTC BOOL REMOTE_CF;	//�������ֱ�־
EXTERN_INTC BYTE gbPressHoldTime; //ң�ذ������ֵ�ʱ��    ������
EXTERN_INTC BYTE gbRepKEY_EN;		//����������
EXTERN_INTC BYTE gbKEYRepTime; //ң�ذ��������Ӽ��ĵ�ʱ����50*x ms    0��ʾû�м��50ms
EXTERN_INTC BYTE gbNEW_KEY;

void Init_IR(void);
#endif
