#include "wheel.h"
#include "esp8266.h"


volatile TrainingPlatform runningWheel;
extern volatile Wifi wifi;
extern volatile Uart uart;
const uint16_t xdata SPEED_TABLE[6] = { 0x0, 0x0106, 0x020d, 0x03b1, 0x0521, 0x0628};

/* IR receiver pinout */ /* NOTE THAT P0.2 AND P0.3 CAN BE THE IR EMITTER CONTROL PORT */

SI_SBIT (L_1, SFR_P0, 1);
SI_SBIT (L_2, SFR_P0, 7);
SI_SBIT (L_3, SFR_P1, 0);
SI_SBIT (L_4, SFR_P1, 1);
SI_SBIT (L_5, SFR_P1, 2);
SI_SBIT (M_1, SFR_P1, 3);
SI_SBIT (M_2, SFR_P1, 4);
SI_SBIT (M_3, SFR_P1, 5);
SI_SBIT (M_4, SFR_P1, 6);
SI_SBIT (M_5, SFR_P1, 7);
SI_SBIT (R_1, SFR_P2, 0);
SI_SBIT (R_2, SFR_P2, 1);
SI_SBIT (R_3, SFR_P2, 3);
SI_SBIT (R_4, SFR_P2, 4);
SI_SBIT (R_5, SFR_P2, 5);

/*--------------------*/

/* 
   note that the flag we determine if pos data need sent is shared by 3 structures, this will cause miss send
   problem at critical time, but didn't receive any report so far, just make a note that we can make flag
   for each structures. 
*/
void wheelProcess(void)
{   
    uint8_t index, index2; /* for loop and array index use */

    if (uart.Tstate == WAIT_KNOCK_DOOR)
        return;

    if (runningWheel.intervalFlag == 1) {    
        runningWheel.intervalFlag = 0;
        for (index = 0; index < 3; index++) { /* qty of wheel */        
            runningWheel.wheel[index].variability[0] = 0;
        	runningWheel.wheel[index].variability[1] = 0;
            runningWheel.wheel[index].variability[2] = 0;
            runningWheel.wheel[index].variability[3] = 0;
            runningWheel.wheel[index].variability[4] = 0;
        }	
    }

	for (index = 0; index < 3; index++) {
		switch (index) {
			case 0: /* L-wheel */
				if (L_1)      runningWheel.wheel[0].variability[0]++;
				else if (L_2) runningWheel.wheel[0].variability[1]++;
				else if (L_3) runningWheel.wheel[0].variability[2]++;
				else if (L_4) runningWheel.wheel[0].variability[3]++;
				else if (L_5) runningWheel.wheel[0].variability[4]++;

				for (index2 = 4; index2 >= 0; index2--) {
					if (runningWheel.wheel[0].variability[index2] > IR_DETERMINE_THRESHOLD) {
						runningWheel.wheel[0].variability[0] = 0;
						runningWheel.wheel[0].variability[1] = 0;
						runningWheel.wheel[0].variability[2] = 0;
						runningWheel.wheel[0].variability[3] = 0;
						runningWheel.wheel[0].variability[4] = 0;

						runningWheel.wheel[0].currentPos = index2 + 1; /* number of sensored IR */

						if (runningWheel.wheel[0].lastPos != runningWheel.wheel[0].currentPos) {
							runningWheel.wheel[0].lastPos = index2 + 1; /* remove dereference overhead(compare to older version(3_arm_escalator)) */
							wifi.isDataChanged = 1;
						}
					}
				}
				break;
			case 1: /* M-wheel */
				if (M_1)      runningWheel.wheel[1].variability[0]++;
				else if (M_2) runningWheel.wheel[1].variability[1]++;
				else if (M_3) runningWheel.wheel[1].variability[2]++;
				else if (M_4) runningWheel.wheel[1].variability[3]++;
				else if (M_5) runningWheel.wheel[1].variability[4]++;

				for (index2 = 4; index2 >= 0; index2--) {
					if (runningWheel.wheel[1].variability[index2] > IR_DETERMINE_THRESHOLD) {
						runningWheel.wheel[1].variability[0] = 0;
						runningWheel.wheel[1].variability[1] = 0;
						runningWheel.wheel[1].variability[2] = 0;
						runningWheel.wheel[1].variability[3] = 0;
						runningWheel.wheel[1].variability[4] = 0;

						runningWheel.wheel[1].currentPos = index2 + 1; /* number of sensored IR */

						if (runningWheel.wheel[1].lastPos != runningWheel.wheel[1].currentPos) {
							runningWheel.wheel[1].lastPos = index2 + 1; /* remove dereference overhead(compare to older version(3_arm_escalator)) */
							wifi.isDataChanged = 1;
						}
					}
				}
				break;
			case 2: /* R-wheel */
				if (R_1)      runningWheel.wheel[2].variability[0]++;
				else if (R_2) runningWheel.wheel[2].variability[1]++;
				else if (R_3) runningWheel.wheel[2].variability[2]++;
				else if (R_4) runningWheel.wheel[2].variability[3]++;
				else if (R_5) runningWheel.wheel[2].variability[4]++;

				for (index2 = 4; index2 >= 0; index2--) {
					if (runningWheel.wheel[2].variability[index2] > IR_DETERMINE_THRESHOLD) {
						runningWheel.wheel[2].variability[0] = 0;
						runningWheel.wheel[2].variability[1] = 0;
						runningWheel.wheel[2].variability[2] = 0;
						runningWheel.wheel[2].variability[3] = 0;
						runningWheel.wheel[2].variability[4] = 0;

						runningWheel.wheel[2].currentPos = index2 + 1; /* number of sensored IR */

						if (runningWheel.wheel[2].lastPos != runningWheel.wheel[2].currentPos) {
							runningWheel.wheel[2].lastPos = index2 + 1; /* remove dereference overhead(compare to older version(3_arm_escalator)) */
							wifi.isDataChanged = 1;
						}
					}
				}
				break;
		}
				
	}


    
    
}

void portInit(void)
{
	L_1 = 0;
	L_2 = 0;
	L_3 = 0;
	L_4 = 0;
	L_5 = 0;
	M_1 = 0;
	M_2 = 0;
	M_3 = 0;
	M_4 = 0;
	M_5 = 0;
	R_1 = 0;
	R_2 = 0;
	R_3 = 0;
	R_4 = 0;
	R_5 = 0;
}


/*************flawless0714 * END OF FILE****/