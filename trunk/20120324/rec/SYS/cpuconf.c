/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////

//#include "C8051F330.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void PCA_Init()
{
    PCA0CN    = 0x40;
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x04;
    PCA0CPM0  = 0x31;
}

void Timer_Init()
{
    TCON      = 0x50;
    TMOD      = 0x22;
    CKCON     = 0x03;
    TH1       = 0x96;
    TMR2CN    = 0x0C;
    TMR3CN    = 0x04;
    TMR3RLL   = 0x9E;
    TMR3RLH   = 0x38;
}

void ADC_Init()
{
    AMX0P     = 0x0C;
    AMX0N     = 0x11;
    ADC0CF    = 0xC0;
    ADC0CN    = 0x02;
}

void Voltage_Reference_Init()
{
    REF0CN    = 0x07;
}

void Port_IO_Init()
{
    // P0.0  -  Skipped,     Open-Drain, Analog
    // P0.1  -  Skipped,     Open-Drain, Digital
    // P0.2  -  Skipped,     Open-Drain, Analog
    // P0.3  -  Skipped,     Open-Drain, Analog
    // P0.4  -  Skipped,     Open-Drain, Digital
    // P0.5  -  Skipped,     Open-Drain, Digital
    // P0.6  -  Skipped,     Open-Drain, Digital
    // P0.7  -  Skipped,     Open-Drain, Digital

    // P1.0  -  Skipped,     Open-Drain, Digital
    // P1.1  -  Skipped,     Push-Pull,  Digital
    // P1.2  -  Skipped,     Push-Pull,  Digital
    // P1.3  -  Skipped,     Push-Pull,  Digital
    // P1.4  -  Skipped,     Push-Pull,  Digital
    // P1.5  -  Skipped,     Push-Pull,  Digital
    // P1.6  -  CEX0 (PCA),  Open-Drain, Digital
    // P1.7  -  Unassigned,  Open-Drain, Digital

    P0MDIN    = 0xF2;
    P1MDOUT   = 0x3E;
    P0SKIP    = 0xFF;
    P1SKIP    = 0x3F;
    XBR1      = 0x41;
}

void Oscillator_Init()
{
    int i = 0;
    OSCXCN    = 0x67;
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);
    OSCICN    = 0x83;
}

void Interrupts_Init()
{
    EIE1      = 0x90;
    IT01CF    = 0x21;
}

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
    Timer_Init();
    ADC_Init();
    Voltage_Reference_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();
}
