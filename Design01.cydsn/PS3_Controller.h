//ahya

#ifndef PS3_CONTROLLER_H
#define PS3_CONTROLLER_H

#include <project.h>

typedef struct
{
    uint8 SELECT : 1;
	uint8 START : 1;
	uint8 UP : 1;
	uint8 RIGHT : 1;
	uint8 DOWN : 1;
	uint8 LEFT : 1;
	uint8 L2 : 1;
	uint8 R2 : 1;
	uint8 L1 : 1;
	uint8 R1 : 1;
	uint8 TRIANGLE : 1;
	uint8 CIRCLE : 1;
	uint8 CROSS : 1;
	uint8 SQUARE : 1;
	uint8 ANALOG_RX;
	uint8 ANALOG_RY;
	uint8 ANALOG_LX;
	uint8 ANALOG_LY;
}PS3Controller;

void PS3_Start(void);
void PS3_Stop(void);
PS3Controller PS3_Controller_get(void);
//void PS3_controller_rxdata(uint8 rx);

#endif
