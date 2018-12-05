#ifndef __WHEEL_H
#define __WHEEL_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "esp8266.h"

void wheelProcess(void);


typedef enum
{ 
    SENSOR_NO_1 = 1,
    SENSOR_NO_2,
    SENSOR_NO_3,
    SENSOR_NO_4,
    SENSOR_NO_5,
    SENSOR_INIT /* prevent location duplicate */
}RAT_POS;

typedef enum
{
    NORMAL = 1,
    AUTO_SPEED
}MODE;

typedef struct
{
    uint16_t variability[5];
    uint8_t lastPos;
    uint8_t currentPos;
    //uint8_t isDACSuccessive;
    //uint16_t successiveDACTarget;
    //uint32_t successiveTimestamp;
}Wheel;

typedef struct
{
    Wheel wheel[3];
    uint8_t intervalFlag;
    //MODE mode;
    //uint32_t autoSpeedTick;
    //uint8_t queueTask;
}TrainingPlatform;

















#ifdef __cplusplus
}
#endif

#endif

/*************flawless0714 * END OF FILE****/