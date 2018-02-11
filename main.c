/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include "matrix.h"
#include "PS3_Controller.h"
#include <math.h>

#define MAX_POWER 230

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    motor1_Start();
    motor2_Start();
    motor3_Start();
	PS3_Start();
	PS3Controller *ps3;
    char i;
	uint8 power;
	uint8 m[3] = {0};
	uint8 x, y;
    for(;;)
    {
		ps3 = PS3_Contorller_get();
		x = ps3->ANALOG_LX - 64;
		y = ps3->ANALOG_LY - 64;
		power = sqrt(x * x + y * y);
		matrix(ps3, m, power);
		if (m[0] >= 0)
			reverse_1_Write(0);
		else
			reverse_1_Write(1);
		if (m[1] >= 0)
			reverse_2_Write(0);
		else
			reverse_2_Write(1);
		if (m[2] >= 0)
			reverse_3_Write(0);
		else
			reverse_3_Write(1);

		for (i = 0; i < 3; i++) {
				m[i] *= power;
		}
		motor1_WriteCompare(m[0]);
		motor2_WriteCompare(m[1]);
		motor3_WriteCompare(m[2]);
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
