#ifndef	_SYSTEM_C_
#define	_SYSTEM_C_
/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////


#include "general.h"
#include <string.h>
//#include "cpuconf.c"
#include "cpuconftwo.c"
//#include "cpuconf0528.c"
void Init_patch(void)
{
    TMR3CN = 0x04 | 0x01;
    //1ms
    TMR3RL = 65535 - 1000 * 12 / 8; //T3ѡ�ⲿ12M����

    //TMR3RL=65535- 1000*24.5/12;// �ڲ�
    TH0 = TL0 = 256 - TM0_RELOAD;

    TI0 = 1;                            // Indicate TX0 ready
    IT0 = 1;	// int0 ���ش���
    IT1 = 1; // int1 ���ش���
    EA = 1;

}
#if 0
//ѡ��32768
void SEL_SYSCLK_LFOSC()
{
    OSCLCN    |= 0x82;
    while ((OSCLCN & 0x40) == 0);
    CLKSEL    = 0x02;
    OSCICN &= ~0x80	 ; //�رո�Ƶ��
    TMR3RL = 65535 - 1000 * 0.040 / 12;
}
//ѡ���ڲ���
void SEL_SYSCLK_HFOSC()
{

    OSCICN = 0x83	 ; //�򿪸�Ƶ��
    CLKSEL    = 0x00;
    TMR3RL = 65535 - 1000 * 24.5 / 12;

}
#endif



//�͹��ķ�ʽ ,�л����ڲ���Ƶ����	 ���ⲿ�رգ��ڸ߹ر�
void Oscillator_LowCost()
{
    int i = 0;
    OSCLCN = 0xbc;		//�ڲ���Ƶ����
    for(i = 0; i < 1000; i++);
    CLKSEL = 0x02; //ѡ���ڲ���Ƶ

    OSCICN = 0;					//�ڲ���Ƶ�ر�

    OSCXCN = 0;				 //�ⲿ�ر�

}



//�������У��ڲ���Ƶ�񵴣��ⲿ������
void Oscillator_HiCost()
{
    int i = 0;
    OSCICN    = 0x83;
    for(i = 0; i < 250; i++)
    {
        _nop_();
        _nop_();
    };
    while ((OSCICN & 0x40) == 0);	   //�ȴ���Ƶ�ȶ�
    CLKSEL = 0x00;
    OSCXCN    = 0x67;
    Oscillator_Init()	 ;	 //�ȴ��ⲿ�ȶ�
    OSCLCN = 0;


}
void SwitchADport(BYTE adport)
{
    if(F_ADOK == 0)return;
    //	DBG(printf("sw to Port( %bd )",g_CurrentADport));
    if(adport == 0xff)
    {
        g_CurrentADport++;

        if(g_CurrentADport >= MAXNUMBER_PORT_AD)
            g_CurrentADport = MINNUMBER_PORT_AD;
    }
    else g_CurrentADport = adport;


    adport = g_CurrentADport;
    EIE1 &= ~0x08;
    AD0INT = 0;
    AD0EN = 0;

    if(adport == ADPORT_TempaSensor)
    {
        ADC0MX = 0x10;
    }
    else if(adport == ADPORT_BAT)
    {
        ADC0MX = 0x08;
    }
    else if(adport == ADPORT_POWER)
    {
        ADC0MX = 0x07;
    }
    else if(adport == ADPORT_SENSOR)
    {
        ADC0MX = 0x0C;
    }

    DelayXms(1);
    F_ADInit = 1;
    F_ADOK = 0;
    AD0EN = 1;
    EIE1 |= 0x08;


}

//���ز�����ƽ��ֵ
WORD  Force_MesureADport(BYTE portm )
{
    EIE1 &= ~0x08;
    F_ADOK = 1;
    SwitchADport(portm);
    while(F_ADOK == 0);
    return Current_temperatureV;

}
void Init_VAR(void)
{


}

void INT0_ISR (void) interrupt INTERRUPT_INT0
{

    PCON &= ~0x01	 ;


}
void TIME0_ISR (void) interrupt INTERRUPT_TIMER0
{




}
void INT1_ISR (void) interrupt INTERRUPT_INT1
{
    PCON &= ~0x01	 ;



}
void TIME1_ISR (void) interrupt INTERRUPT_TIMER1
{




}
void TIME2_ISR (void) interrupt INTERRUPT_TIMER2
{






}

//  *********************************************************************
//  Function    :   ISR_Timer0; ET0(SFR, bit 1 of IE)
//                      :   Timer0, mode 1, 16 bits counter
//  Description :   1. receive the internal timer counting(When up counter overflow)
//                      :   2. must re-load the TH1, TL1 value
//                  3. The Timer will scan Key, monitor system and Error-Concealment
//  Arguments   :
//  Return      :   none
//  Side Effect :
//  *********************************************************************


/*
10ms�ж�

*/
void        ISR_Timer3(void) interrupt INTERRUPT_TIMER3 using 3
{
    static BYTE sys_click = 0;
    TMR3CN    &= ~0xC0;	   //����жϱ�־
    F_1ms = 1;						//��ʱ������

    if(sys_click++ >= 5)
    {
        sys_click = 0;
        F_5ms = 1;


        if(sys_time.Time_1_20msec % 10 == 5)
        {
            F_50ms = 1;
#ifdef	SECURE_SPI
            {
                extern BYTE SPITimeOut;
                if(SPITimeOut)SPITimeOut--;
            }
#endif

        }
        if(sys_time.Time_1_20msec % 20 == 10)
        {
            F_100ms = 1;
        }

        //
        if( ++sys_time.Time_1_20msec >= 100)	//50*10=500ms
        {
            sys_time.Time_1_20msec = 0;
            //500msec time
            F_500ms = 1;
            F_half_sec = !F_half_sec;
            if(F_half_sec )
            {
                F_1000ms = 1;

            }
        }

    }
    return;
}


#define	AD_COUNT	256
// measured voltage in mV
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR averages 2048 samples then prints the result to the terminal.  The
// ISR is called after each ADC conversion which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
#pragma NOAREGS
void ADC0_ISR (void) interrupt INTERRUPT_ADC0_EOC using 3
{

    static uint32 accumulator ;     // accumulator for averaging
    static uint16 measurements  ;  // measurement counter
    uint16 result;
    AD0INT = 0;                               // clear ADC0 conv. complete flag
    if(F_ADInit)
    {
        accumulator = 0;
        measurements = AD_COUNT;
        F_ADInit = 0;
        return;
    }

    if(F_ADOK) return;

    accumulator += ADC0;
    measurements--;
    if(measurements == 0)
    {
        measurements = AD_COUNT;
        result = accumulator / AD_COUNT;
        accumulator = 0;
        // The 10-bit ADC value is averaged across 2048 measurements.
        // The measured voltage applied to P1.4 is then:
        //
        //                           Vref (mV)
        //   measurement (mV) =   --------------- * result (bits)
        //                       (2^12)-1 (bits)

        Current_temperatureV = result; //( WORD) (result * 2200 / 4095);
        F_ADOK = 1;

        // printf("AD  voltage: %ld mV\n",Current_VP);
    }
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PCA0_ISR
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This is the ISR for the PCA.  It handles the case when a capture occurs on
// channel 0, and updates the variables to reflect the new capture information.
//
//-----------------------------------------------------------------------------


//ң�ؽ���
extern void   IR_core_NEC(WORD width) ;

#define	CEX0_PIN	  P31
//#pragma AREGS      // for other code it is save to use ARx symbols
#pragma NOAREGS
void PCA0_ISR (void) interrupt INTERRUPT_PCA0 using 2
{
#if defined  PCA0_EN||	defined  PCA1_EN||defined  PCA2_EN	    //�������
    static idata PCA0_EN
#endif
#if defined	PCA0_EN
    WORD	CAP_PREV0,
#endif
#if defined	PCA1_EN
    CAP_PREV1
#endif
#if defined  PCA2_EN
    , CAP_PREV2
#endif
#if defined  PCA0_EN||	defined  PCA1_EN||defined  PCA2_EN
    ;
#endif
#if defined  PCA0_EN||	defined  PCA1_EN||defined  PCA2_EN

    WORD   current_capture_value, capture_period;
#endif
    if(CCF0 == 0 && CCF1 == 0 && CCF2 == 0 //&&CCF3==0&&CCF4==0
      )                                // Interrupt was caused by other bits.
    {
        PCA0CN &= ~0x86;                 // Clear other interrupt flags for PCA
        return;
    }



    //ң��
    //p31
    if (CCF0)                           // If Module 0 caused the interrupt
    {
        PCA0CN &= ~0x81;                        // Clear module 0 interrupt flag.
#ifdef	PCA0_EN
        // Store most recent capture value
        current_capture_value = PCA0CP0;
        // Calculate capture period from last two values.
        capture_period = current_capture_value - CAP_PREV0;
        // Update previous capture value with most recent info.
        CAP_PREV0 = current_capture_value;
        //  IR_core_NEC(capture_period);
#endif

    }
    //p32
    //��������
    if (CCF1)                           // If Module 1 caused the interrupt
    {
        //P23=0;
        //  CCF1 = 0;
        PCA0CN &= ~0x82;
#ifdef	PCA1_EN
        // Store most recent capture value
        current_capture_value = PCA0CP1;
        // Calculate capture period from last two values.
        capture_period = current_capture_value - CAP_PREV1;
        // Update previous capture value with most recent info.
        CAP_PREV1 = current_capture_value;
        // 	  DALI_Receive(capture_period);
#endif
    }

    //p33
    //DALI���ݷ��� manchs
    if (CCF2)                           // If Module 2 caused the interrupt
    {

        //CCF2 = 0;
        PCA0CN &= ~0x84;
#if 0
        // Store most recent capture value
        current_capture_value = PCA0CP2;
        // Calculate capture period from last two values.
        capture_period = current_capture_value - CAP_PREV2;
        // Update previous capture value with most recent info.
        CAP_PREV2 = current_capture_value;
#endif


        DALI_SendData();



    }


    return;
}




/*******************************************/
void DelayXms(WORD td)
{

    while(td)
    {
        while(F_1ms == 0);
        F_1ms = 0;
        td--;
    }
}

/*******************************************/
void Delay10us(WORD td)
{
    WORD i;
    while(td)
    {
        for (i = 0; i < 20; i++)		 //14* n
        {
            _nop_();
        }
        td--;
    }
}


/********************************************
˫�ֽ�16����תΪ 5 λ10����BCD ��
tran_data:
BCD_buf:�ռ䲻С��5
		��λ��BCD_buf[0]
		��λ��BCD_buf[4]

*********************************************/
#define	BCD_NUMS_P	4  //ת����λ��
void HexToBCD(WORD  *ptran_data, BYTE  *BCD_buf)
{
    BYTE i;
    BYTE  *p_buf;
    WORD tran_data = *ptran_data;
    p_buf = BCD_buf;
    for(i = 0; i < BCD_NUMS_P; i++)
    {
        *(p_buf + BCD_NUMS_P - 1 - i) = tran_data % 10;
        tran_data = tran_data / 10;
    }

}

#endif
