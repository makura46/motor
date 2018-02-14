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

char out[100]={0};
void rotate(PS3Controller ps) {
	if (ps.L2) {
		reverse_D_Write(1);
		reverse_B_Write(2);
		reverse_C_Write(1);
	}else {
		reverse_D_Write(2);
		reverse_B_Write(1);
		reverse_C_Write(2);
	}
	motorD_WriteCompare(200);
	motorB_WriteCompare(200);
	motorC_WriteCompare(200);
    
    sprintf(out, "%s\n", "aaa");
    UART_1_PutString(out);
}


int main(void)
{
 	PS3Controller ps3;
    char i;
	double power;
	uint8 m[3] = {0};
    char na[3] = {0};
    double s;
	double x, y;
    char output[100] = {0};
    char flag = 0;

    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    motorD_Start();
    motorB_Start();
    motorC_Start();
	PS3_Start();
    UART_1_Start();
    CyDelay(5000);
    ps3 = PS3_Controller_get();
    ps3.L2 = 0;
    ps3.R2 = 0;
    for(;;)
    {
		ps3 = PS3_Controller_get();

        if (flag) {
            if (ps3.CROSS)
                flag = !flag;
    		if (!ps3.L1 && !ps3.R1) {
    			
                
                //sprintf(output, "%d %d\n", ps3.ANALOG_LX, ps3.ANALOG_LY);
                //UART_1_PutString(output);
        		x = ps3.ANALOG_LX - 64;
        		y = ps3.ANALOG_LY - 64;
                
                s = x*x + y*y;
        		//power = (uint8)(sqrt((double)x * x + y * y));
                power = sqrt(s);
                if (power > 64.0)
                    power *= (64.0 / 90);
                power /= 64;        // 割合
               
        		matrix(&ps3, m, na);
        		if (na[0]) {
        			reverse_D_Write(1);
        		} else {
        			reverse_D_Write(2);
                }
        		if (na[1]) {
        			reverse_B_Write(2);
        		} else {
        			reverse_B_Write(1);
                }
        		if (na[2]) {
        			reverse_C_Write(1);
        		} else {
        			reverse_C_Write(2);
                }
        /*
        		for (i = 0; i < 3; i++) {
        				m[i] *= MAX_POWER / (double)91 * (double)power;
        		}
          */     
                int max = fmax((double)m[0], fmax((double)m[1], m[2]));
                
                
                for (i = 0; i < 3; i++) {
                    m[i] *= MAX_POWER/max;
                    
                    m[i] *= power;
                  
                }
                sprintf(output, "\r\n");
                UART_1_PutString(output);
                
                if (m[0] > MAX_POWER || m[1] > MAX_POWER || m[2] > MAX_POWER) {
                    m[0] = 0;
                    m[1] = 0;
                    m[2] = 0;
                }
                motorD_WriteCompare(m[0]);
        		motorB_WriteCompare(m[1]);
        		motorC_WriteCompare(m[2]);
               
                sprintf(output, "%f %f %f    %d %d %d\n", m[0]/255.0, m[1]/255.0, m[2]/255.0, na[0], na[1], na[2]);
                UART_1_PutString(output);
            } else if(ps3.L1 || ps3.R1) {
                rotate(ps3);
            }
        } else {
            if (ps3.CIRCLE)
                flag = !flag;
        }
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
