#include "wheel.h"
#include "esp8266.h"


volatile TrainingPlatform runningWheel;
extern volatile Wifi wifi;
extern volatile Uart uart;
const uint16_t xdata SPEED_TABLE[6] = { 0x0, 0x0106, 0x020d, 0x03b1, 0x0521, 0x0628};

void wheelProcess(void)
{   
    uint8_t index, index2; /* for loop and array index use */

    if (uart.Tstate == WAIT_KNOCK_DOOR)
        return;

    


    
    
}


















/*************flawless0714 * END OF FILE****/