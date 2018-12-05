#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <SI_EFM8LB1_Register_Enums.h>
#include <stdlib.h>
#include "uart.h"
#include "esp8266.h"
#include "wheel.h"

#define DAC_APPLY_TIME 25
#define DAC_SUCCESSIVE_TIME_INTERVAL 15

void Init(void);
//void taskHandler(void);
//void taskUpdate(void);
//void taskProcess(void);
//void levelupSpeed(uint16_t dac, uint8_t num);
//void successiveDACIncrement(void);

typedef struct
{
    uint32_t sysTick; /* tick per second */
}Mcu;

typedef struct{
  uint16_t p1_1;
  uint16_t p1_2;
  uint16_t p1_3;
}scan_t;

#ifdef __cplusplus
}
#endif

#endif

/*************flawless0714 * END OF FILE****/
