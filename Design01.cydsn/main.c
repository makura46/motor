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
#include <math.h>
#include <string.h>
#include "matrix.h"
#include "PS3_Controller.h"


#define MAX_POWER 230

int main(void)
{
 	PS3Controller ps3;
    char i;
	int power;
	uint8 m[3] = {0};
    char na[3] = {0};
    double s;
	double x, y;
    char output[100] = {0};   

    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    motor1_Start();
    motor2_Start();
    motor3_Start();
	PS3_Start();
    UART_1_Start();
    
    for(;;)
    {
		ps3 = PS3_Controller_get();
        //sprintf(output, "%d %d\n", ps3.ANALOG_LX, ps3.ANALOG_LY);
        //UART_1_PutString(output);
		x = ps3.ANALOG_LX - 64;
		y = ps3.ANALOG_LY - 64;
        
        s = x*x + y*y;
		//power = (uint8)(sqrt((double)x * x + y * y));
        power = (int)(sqrt(s));
        
		matrix(&ps3, m, na);
		if (na[0]) {
			reverse_1_Write(1);
		} else {
			reverse_1_Write(2);
        }
		if (na[1]) {
			reverse_2_Write(1);
		} else {
			reverse_2_Write(2);
        }
		if (na[2]) {
			reverse_3_Write(1);
		} else {
			reverse_3_Write(2);
        }
/*
		for (i = 0; i < 3; i++) {
				m[i] *= MAX_POWER / (double)91 * (double)power;
		}
  */     
        for (i = 0; i < 3; i++) {
	        m[i] *= 2;
		}
        
        if (m[0] > MAX_POWER || m[1] > MAX_POWER || m[2] > MAX_POWER) {
            m[0] = 0;
            m[1] = 0;
            m[2] = 0;
        }
        motor1_WriteCompare(m[0]);
		motor2_WriteCompare(m[1]);
		motor3_WriteCompare(m[2]);
       
        sprintf(output, "%f %f %f\n", m[0]/255.0, m[1]/255.0, m[2]/255.0);
        UART_1_PutString(output);
        
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
