// Ver 0.3.1
/*
    1. Since the complicate step to setup wifi module(esp8266), we temporary deprecated the wifi transfer method and use USB-TTL instead.

*/

#include "main.h"

#define BC_DISCONNECTED 0              // 0 = Board Controller disconnected to EFM8 UART pins     
#define BC_CONNECTED    1              // 1 = Board Controller connected                                       
volatile Mcu mcu;
extern volatile TrainingPlatform runningWheel;
extern volatile Uart uart;
extern const uint16_t xdata SPEED_TABLE[6];
extern volatile Wifi wifi; 
SI_SBIT (BC_EN, SFR_P2, 2);
//uint32_t debugTick = 0;

void main()
{
    Init();
    uartInit();
    wifiInit();
    BC_EN = BC_CONNECTED /*BC_CONNECTED*/; /* since we are now temporary using UART to send training data instead of wifi module, hence we assign UART to Board Controller to send data through usb virtual port directly */
    IE_EA = 1;
        
    while(1) {        
        //wifiProcess(); re-enable once wifi got smooth setup and cleanup
        uartTransmission();
        wheelProcess();
        //taskHandler();
        //successiveDACIncrement();
    }
}
/* auto speed related, since this feature haven't requested by PM, but I believe someday he will
void successiveDACIncrement(void)
{
    uint8_t index, savedpage;
    uint16_t DACval;
    savedpage = SFRPAGE;
    SFRPAGE = PG4_PAGE;

    for (index = 0; index < 3; index++)
    {
        if (!runningWheel.wheel[index].isDACSuccessive)
            continue;
        if (runningWheel.wheel[index].successiveTimestamp >= mcu.sysTick)
            continue;

        switch (index)
        {
            case 0:
                DACval = 0;
                DACval |= DAC0H;                        
                DACval <<= 8;                        
                DACval |= DAC0L;

                if (DACval == runningWheel.wheel[0].successiveDACTarget) 
                {
                    runningWheel.wheel[0].isDACSuccessive = 0;
                    continue;
                }

                DACval++;

                DAC0L = (uint8_t) DACval & 0xff; 
                DAC0H = (uint8_t) ((DACval >> 8) & 0xff);

                runningWheel.wheel[0].successiveTimestamp = mcu.sysTick + DAC_SUCCESSIVE_TIME_INTERVAL;
                break;
            case 1:
                DACval = 0;
                DACval |= DAC1H;
                DACval <<= 8;
                DACval |= DAC1L;

                if (DACval == runningWheel.wheel[1].successiveDACTarget)
                {
                    runningWheel.wheel[1].isDACSuccessive = 0;
                    continue;
                }

                DACval++;

                DAC1L = (uint8_t) DACval & 0xff;
                DAC1H = (uint8_t) ((DACval >> 8) & 0xff);

                runningWheel.wheel[1].successiveTimestamp = mcu.sysTick + DAC_SUCCESSIVE_TIME_INTERVAL;
                break;
            case 2:
                DACval = 0;
                DACval |= DAC2H;
                DACval <<= 8;
                DACval |= DAC2L;

                if (DACval == runningWheel.wheel[2].successiveDACTarget)
                {
                    runningWheel.wheel[2].isDACSuccessive = 0;
                    continue;
                }

                DACval++;

                DAC2L = (uint8_t) DACval & 0xff;
                DAC2H = (uint8_t) ((DACval >> 8) & 0xff);

                runningWheel.wheel[2].successiveTimestamp = mcu.sysTick + DAC_SUCCESSIVE_TIME_INTERVAL;
                break;
        }            
    }
    SFRPAGE = savedpage;
}

void taskUpdate(void)
{
    runningWheel.queueTask >>= 2;
}

void levelupSpeed(uint16_t dac, uint8_t num)
{
    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        if (dac == SPEED_TABLE[i])
        {            
            dac = SPEED_TABLE[i + 1]; 
            runningWheel.wheel[num].isDACSuccessive = 1;
            runningWheel.wheel[num].successiveDACTarget = dac;            
        }
    }
}

void taskProcess(void)
{
    uint8_t savedpage, idx;
    uint16_t DACval;    
    switch (runningWheel.queueTask & 0x3) 
    {
        case 0:            
            break; 
        case 1:
            if (mcu.sysTick >= runningWheel.autoSpeedTick)
            {                
                runningWheel.autoSpeedTick += 10000;
                savedpage = SFRPAGE;
                SFRPAGE = PG4_PAGE;

                for (idx = 0; idx < 3; idx++)
                {
                    DACval = 0; 
                    if (idx == 0)
                    {
                        DACval |= DAC0H;                        
                        DACval <<= 8;                        
                        DACval |= DAC0L;                         
                        levelupSpeed(DACval, idx);
                    }
                    else if (idx == 1)
                    {
                        DACval |= DAC1H;
                        DACval <<= 8;
                        DACval |= DAC1L;
                        levelupSpeed(DACval, idx);
                    }
                    else
                    {
                        DACval |= DAC2H;
                        DACval <<= 8;
                        DACval |= DAC2L;
                        levelupSpeed(DACval, idx);
                    }
                    
                }
                SFRPAGE = savedpage;
            }
            else 
                return;
            break;
        case 2: 
            if (mcu.sysTick >= runningWheel.autoSpeedTick)
            {
                runningWheel.mode = NORMAL;
                runningWheel.queueTask |= 0xC; 
                wifi.isDataChanged = 1;
            }
            else
                return;            
            break;
        case 3:
            if (uart.Tstate != IDLE)
                return;
            runningWheel.mode = AUTO_SPEED;
            break;
        default:
            return; 
    }
    taskUpdate();
}

void taskHandler(void)
{
    if (runningWheel.mode == AUTO_SPEED && uart.Tstate == IDLE)
    {        
        if (!runningWheel.queueTask)
        {             
            runningWheel.queueTask = 0x25;
            runningWheel.autoSpeedTick = mcu.sysTick + 10000;
        }
    }
    taskProcess();
}
*/

void Init(void)
{
    uint8_t index;
    uint8_t SFRPAGE_save = SFRPAGE;
    /* WDT disable ------------------*/
    SFRPAGE = 0x00;
    WDTCN = 0xDE; //First key (use this write sequence to disable the WDT)
	WDTCN = 0xAD; //Second key
    /*-------------------------------*/
    /* SYSCLK select ----------------*/
    CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1; // SYSCLK = 24 Mhz divided by 1.
    while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY);
    /*-------------------------------*/

    /* GPIO setting -----------------*/
    P0MASK = P0MASK_B0__IGNORED  | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;

    P0MDOUT = P0MDOUT_B0__OPEN_DRAIN | P0MDOUT_B1__OPEN_DRAIN
			| P0MDOUT_B2__PUSH_PULL | P0MDOUT_B3__OPEN_DRAIN
			| P0MDOUT_B4__PUSH_PULL  | P0MDOUT_B5__OPEN_DRAIN
			| P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;

    P0MDIN = P0MDIN_B0__DIGITAL  | P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL
			| P0MDIN_B3__DIGITAL | P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL
			| P0MDIN_B6__DIGITAL | P0MDIN_B7__DIGITAL;

    P0SKIP = P0SKIP_B0__SKIPPED | P0SKIP_B1__SKIPPED
			| P0SKIP_B2__SKIPPED | P0SKIP_B3__SKIPPED
			| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
			| P0SKIP_B6__SKIPPED | P0SKIP_B7__SKIPPED;

    
    P1MASK = P1MASK_B0__IGNORED  | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

    P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN
			| P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN
			| P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;

    P1MDIN = P1MDIN_B0__DIGITAL  | P1MDIN_B1__DIGITAL | P1MDIN_B2__DIGITAL
			| P1MDIN_B3__DIGITAL | P1MDIN_B4__DIGITAL | P1MDIN_B5__DIGITAL
			| P1MDIN_B6__DIGITAL | P1MDIN_B7__DIGITAL;

    P1SKIP = P1SKIP_B0__SKIPPED  | P1SKIP_B1__SKIPPED
			| P1SKIP_B2__SKIPPED | P1SKIP_B3__SKIPPED
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED
			| P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;


    P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN
			| P2MDOUT_B2__OPEN_DRAIN | P2MDOUT_B3__OPEN_DRAIN
			| P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN
			| P2MDOUT_B6__OPEN_DRAIN;

    P2MDIN = P2MDIN_B0__DIGITAL  | P2MDIN_B1__DIGITAL | P2MDIN_B2__DIGITAL
			| P2MDIN_B3__DIGITAL | P2MDIN_B4__DIGITAL | P2MDIN_B5__DIGITAL
			| P2MDIN_B6__DIGITAL;

    SFRPAGE = 0x20;
    P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED
			| P2SKIP_B2__SKIPPED | P2SKIP_B3__SKIPPED;


    SFRPAGE = 0x00;
    XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED
			| XBR2_URT1E__DISABLED | XBR2_URT1RTSE__DISABLED
			| XBR2_URT1CTSE__DISABLED;

    XBR0 = XBR0_URT0E__ENABLED | XBR0_SPI0E__DISABLED | XBR0_SMB0E__DISABLED
			| XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
			| XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;

    /*-------------------------------*/

    /* Timer setting ----------------*/
    TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;    
    TH1 = (/*0xe7*/0x96 << TH1_TH1__SHIFT);

    CKCON0 = CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T0M__PRESCALE
			| CKCON0_T2MH__EXTERNAL_CLOCK | CKCON0_T2ML__EXTERNAL_CLOCK
			| CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
			| CKCON0_T1M__SYSCLK;
            
    TMOD = TMOD_T0M__MODE0 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER
			| TMOD_GATE0__DISABLED | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;

    TCON |= TCON_TR1__RUN;

    /* timer 3 */
    /* timer clock = 2.04 Mhz(24.5Mhz(SYSCLK)/12), specified freq = 38k, which has T = 26us approximately generated by this timer (actually freq is 37.792khz, to prevent this, we should use integer timer clock) */
    TMR3L = 0xca;
    TMR3H = 0xff;
    TMR3RLL = 0xca;
    TMR3RLH = 0xff;
    TMR3CN1 |= 0x60; /* reload only on timer overflow */
    TMR3CN0 = 0x04; /* run control */

    //EIP1 |= 0x80; priority level
    //EIP1H |= 0x80;
    /*---------*/

    /* timer 2 */
    /* with current SYSCLK and timer2 setting, tick of timer is 0.4897us, and with 63493 set to timer2's H and L byte we get a timer which interrupt interval is 1 ms */
    
    TMR2H = 0xf8; /* may be this is not necessary */
    TMR2L = 0x05; /* may be this is not necessary */
    TMR2RLH = 0xf8;
    TMR2RLL = 0x05;
    TMR2CN0 = 0x04; /* TR2 run control */
    //IP |= 0x20; not necessary
    //IPH |= 0x20;
    TMR2CN1 = 0x00;
    /*---------*/
    /*-------------------------------*/

    /* DAC setting ------------------*/
    
    SFRPAGE = 0x20; 
    P3MDOUT = P3MDOUT_B0__OPEN_DRAIN | P3MDOUT_B1__OPEN_DRAIN
			| P3MDOUT_B2__OPEN_DRAIN | P3MDOUT_B3__OPEN_DRAIN
			| P3MDOUT_B4__PUSH_PULL | P3MDOUT_B7__OPEN_DRAIN;
    
    SFRPAGE = 0x30;
    DACGCF0 = 0x88;
    DACGCF1 |= 0x08;
    DACGCF1 &= ~0x07;
    DACGCF2 &= ~0x33;
    DAC0CF0 = 0x80; /* last should have TODO which makes system more efficient  */
    DAC0CF1 = 0x00;
    DAC1CF0 = 0x80;
    DAC1CF1 = 0x00;
    DAC2CF0 = 0x80;
    DAC2CF1 = 0x00;
    
    /*-------------------------------*/

    /* Uart setting -----------------*/
    // inside the timer setting
    /*-------------------------------*/

    /* Struct member ----------------*/
    mcu.sysTick = 0;

    runningWheel.intervalFlag = 0;
    //runningWheel.mode = NORMAL;
    //runningWheel.autoSpeedTick = 0;
    //runningWheel.queueTask = 0;
    for (index = 0; index < 3; index++) {
        runningWheel.wheel[index].variability[0] = 0;
        runningWheel.wheel[index].variability[1] = 0;
        runningWheel.wheel[index].variability[2] = 0;
        runningWheel.wheel[index].variability[3] = 0;
        runningWheel.wheel[index].variability[4] = 0;
        runningWheel.wheel[index].lastPos = SENSOR_INIT;
        runningWheel.wheel[index].currentPos = 0;
        //runningWheel.wheel[index].isDACSuccessive = 0;
        //runningWheel.wheel[index].successiveDACTarget = 0;
        //runningWheel.wheel[index].successiveTimestamp = 0;
    }
    /*-------------------------------*/

    /* DAC init ----------------*/
    SFRPAGE = PG4_PAGE;
    DAC0L = 0x0;
	DAC0H = 0x00;
	DAC1L = 0x0;
	DAC1H = 0x00;
	DAC2L = 0x0;
	DAC2H = 0x00;
    /*-------------------------------*/    

    /* Interrupt setting-------------*/
    
    IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
		    | IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__ENABLED | IE_ES0__ENABLED;
            
    SFRPAGE = PG2_PAGE;    
    EIE1 |= EIE1_ET3__ENABLED;
    /*-------------------------------*/

    SFRPAGE = SFRPAGE_save;
}




/*************flawless0714 * END OF FILE****/
