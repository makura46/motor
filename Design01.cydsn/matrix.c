#include "matrix.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "project.h"

void matrix(PS3Controller *ps3, uint8 motor[], char na[]) {
	int i, j;
	float L = 176;
    char output[100] = {0};
	//double m[3] = {0}
    
	double mat[3][3] = {{1.0, 0.0, L}, 
					  {-1.0/2, -sqrt(3.0)/2, L}, 
					  {-1.0/2, sqrt(3.0)/2, L}};
    
	float x = ps3->ANALOG_LX - 64;
	float y = ps3->ANALOG_LY - 64;
    float max = fmaxf(fabsf(x), fabsf(y));
    if (max == 0)
        max = 1;
	float col[3] = {x, y, 0};
   
	float re[3] = {0};
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			re[i] += mat[i][j] * col[j];
		}
	}
    
    
	for (i = 0; i < 3; i++) {
        if (re[i] > 0)
            na[i] = 1;
        else if (re[i] < 0)
            na[i] = -1;
        else
            na[i] = 0;
		motor[i] = fabsf(re[i]);
	}
   
    //sprintf(output, "%d %d %d\n", motor[0], motor[1], motor[2]);
   //UART_1_PutString(output);
}
