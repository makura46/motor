#include "matrix.h"
#include <math.h>

void matrix(PS3Controller *ps3, uint8 motor[], int power) {
	int i;
	double L = 176;
	//double m[3] = {0}
	double mat[3][3] = {{1.0, 0.0, L}, 
					  {-1/2, sqrt(3/2), L}, 
					  {-1/2, -sqrt(3/2), L}};
	//double col[] = {vx, vy, va};
	int x = ps3->ANALOG_LX;
	int y = ps3->ANALOG_LY;
	double col[3] = {x/power, y/power, 0};

	//col[2] = M_PI / 180 * 90;

	double re[3] = {0};
	for (i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			re[i] += mat[i][j] * col[j];
		}
	}
	for (i = 0; i < 3; i++) {
		motor[i] = re[i];
	}
}
