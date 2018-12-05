#include <SI_EFM8LB1_Register_Enums.h>
#include "esp8266.h"
#include "uart.h"
#include "wheel.h"
#include "main.h"

extern volatile TrainingPlatform runningWheel;
extern volatile uint8_t pdata wifiSendBuffer[SEND_BUFFER_SIZE];
extern volatile uint8_t pdata wifiRecvBuffer[RECV_BUFFER_SIZE];
extern volatile Uart uart;
extern volatile Mcu mcu;
extern volatile Wifi wifi;

SI_SBIT(LED0, SFR_P1, 4); // P1.4 LED0

void UART0_ISR(void) interrupt UART0_IRQn /* WARN: we only turn interrupt at needed */
{
    if (SCON0_TI == 1)
    {
        SCON0_TI = 0;
        if (uart.queuingByte > 0)
        {
            uart.queuingByte--;
            SBUF0 = wifiSendBuffer[uart.currentPos++];
        }
        else
        {
            uart.currentPos = 0;
            uart.state = SEND_DONE;
            //IE_EA = 0;			
			//IE_EA = 1;
            if (uart.byteWaiting > 0)
            {
                wifi.currentTick = mcu.sysTick; /* timeout detection */
                /* uart DAC use */  uart.Tstate = RX_BUSY;
                //SCON0 |= SCON0_REN__RECEIVE_ENABLED;
            }
            else
            {                
                wifi.state = RUNNING_TRAINING;
                uart.state = STANDBY;
                
            }
        }
    }
    if (SCON0_RI == 1)
    {
        SCON0_RI = 0;
        /* this is to capture the end training notification data, since there is no better solution concluded, hence... (TODO) */
        if (SBUF0 == 'E')
        {
            uart.byteWaiting = UART_DAC_SIZE;
        }
        if (uart.byteWaiting > 0)
        {
            wifiRecvBuffer[uart.currentPos] = SBUF0; 
            uart.currentPos++; /* TODO: since we are not sure how compiler implement the ++ suffix at last line so we seperate it */
            uart.byteWaiting--;
        }
        if(uart.byteWaiting == 0)
        {
            uart.currentPos = 0;
            uart.state = RECV_DONE;
            /* uart DAC use */  uart.Tstate = RX_DONE;
            //SCON0 &= ~SCON0_REN__RECEIVE_ENABLED;
        }
    }
    
}

void TIMER2_ISR(void) interrupt TIMER2_IRQn
{
    TMR2CN0_TF2H = 0;
    
    mcu.sysTick++;
    
    if ((mcu.sysTick % 500) == 0) {
        runningWheel.intervalFlag = 1;
        //LED0 = ~LED0;
    }
    
}
/*************flawless0714 * END OF FILE****/

