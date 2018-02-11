#ifndef MATRIX_H
#define MATRIX_H
    
#include "PS3_Controller.h"

void matrix(PS3Controller *ps3, uint8 motor[], int power);
	/*
	double L = 30;
	double mat[3][3] = {{1.0, 0.0, L}, 
					  {-1/2, sqrt(3/2), L}, 
					  {-1/2, -sqrt(3/2), L}};
	//double col[] = {vx, vy, va};
	double col[3] = {0};

	col[0] = 0;
	col[1] = 100;
	col[2] = 0;
	//col[2] = M_PI / 180 * 90;

	double re[3] = {0};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			re[i] += mat[i][j] * col[j];
		}
	}
	*m1 = re[0];
	*m2 = re[1];
	*m3 = re[2];
}
*/

#endif
