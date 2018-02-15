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

// 時計周りにD, C, B　の順にモーターを取り付ける
// その場合C, B辺が正面
/* 
Cモーター         Bモーター


        Dモーター
*/

#include "project.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix.h"
#include "PS3_Controller.h"


#define MAX_POWER 230

char out[100]={0};
void rotate(PS3Controller ps, uint8 na[]) {	// 回転する関数
	if (ps.L1) {
		na[0] = 1;
		na[1] = 1;
		na[2] = 2;
		reverse_D_Write(1);
		reverse_B_Write(1);
		reverse_C_Write(2);
	}else {
		na[0] = 2;
		na[1] = 2;
		na[2] = 1;
		reverse_D_Write(2);
		reverse_B_Write(2);
		reverse_C_Write(1);
	}
	motorD_WriteCompare(150);
	motorB_WriteCompare(150);
	motorC_WriteCompare(150);

	sprintf(out, "%s\n", "aaa");
	UART_1_PutString(out);
}


int main(void)
{
	PS3Controller ps3;
	char i;
	double power;
	uint8 m[3] = {0};
	uint8 na[3] = {0};
	uint8 saveRotate[3] = {0};
	double s;
	double x, y;
	char output[100] = {0};
	char flag = 0;	// 操作可能かどうか
    uint8 catch = 0;

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
		for (i = 0; i < 3; i++)
			na[i] = 0;

		if (flag) {	// flagが立っていれば操作可能
			if (ps3.CROSS)	// バツを押すと操作を受け付けない
				flag = !flag;
            if (ps3.TRIANGLE) {
                if (catch) {
                    Air1_Write(0);
                    catch = !catch;
                } else {
                    Air1_Write(1);
                    catch= !catch;
                }
            }
            
            if (ps3.R2) {
                Air2_Write(1);
            } else {
                Air2_Write(0);
            }
            
            if (ps3.L2) {
                Air2_Write(1);
            } else {
                Air2_Write(0);   
            }
			if (!ps3.L1 && !ps3.R1) {	// L1とR1が押されていなかった場合

				//sprintf(output, "%d %d\n", ps3.ANALOG_LX, ps3.ANALOG_LY);
				//UART_1_PutString(output);
				x = ps3.ANALOG_LX - 64;
				y = ps3.ANALOG_LY - 64;

				s = x*x + y*y;
				//power = (uint8)(sqrt((double)x * x + y * y));
				power = sqrt(s);
				if (power > 64.0)
					power *= (64.0 / 90);
				power /= 64;        // スティックの傾き具合の割合 0 <= power <= 1

				matrix(&ps3, m, na);	// 行列計算しmの配列に値を格納  naには回転の向き
				if (na[0] == 1) {
					reverse_D_Write(1);
				} else if (na[0] == 2) {
					reverse_D_Write(2);
				} else {
					if (saveRotate[0] == 1)
						reverse_D_Write(2);
					else if (saveRotate[0] == 2)
						reverse_D_Write(1);
				}
				if (na[1] == 1) {
					reverse_B_Write(1);
				} else if (na[1] == 2) {
					reverse_B_Write(2);
				} else {
					if (saveRotate[1] == 1)
						reverse_B_Write(2);
					else if (saveRotate[1] == 2)
						reverse_B_Write(1);
				}
				if (na[2] == 1) {
					reverse_C_Write(2);
				} else if (na[2] == 2) {
					reverse_C_Write(1);
				} else {
					if (saveRotate[2] == 1)
						reverse_C_Write(1);
					else if (saveRotate[2] == 2)
						reverse_C_Write(2);
				}
				/*
				   for (i = 0; i < 3; i++) {
				   m[i] *= MAX_POWER / (double)91 * (double)power;
				   }
				 */     
				double max = fmax((double)m[0], fmax((double)m[1], m[2])); 	// mの中で最大のものを探す


				for (i = 0; i < 3; i++) {
					double tmp = m[i];
					tmp *= MAX_POWER;
					tmp /= max;
					/*
					m[i] *= MAX_POWER/max;
					*/
					tmp *= power;
					m[i] = (uint8)tmp;

					//m[i] *= power;	// 最大の値に全部の入力を合わす

				}
				sprintf(output, "\r\n");
				UART_1_PutString(output);

				if (m[0] > MAX_POWER || m[1] > MAX_POWER || m[2] > MAX_POWER) {  // 回転の値が最大値を超えると0にする
					m[0] = 0;
					m[1] = 0;
					m[2] = 0;
				}
				motorD_WriteCompare(m[0]);
				motorB_WriteCompare(m[1]);
				motorC_WriteCompare(m[2]);

				sprintf(output, "%f %f %f    %d %d %d\n", m[0]/255.0, m[1]/255.0, m[2]/255.0, na[0], na[1], na[2]);
				UART_1_PutString(output);
			} else if(ps3.L1 || ps3.R1) {	// L1かR1が入力されていると回転する
				rotate(ps3, na);
			}
		} else {
			if (ps3.CIRCLE)	// 丸を押すと操作を受け付ける
				flag = !flag;
		}
		for (i = 0; i < 3; i++)
			saveRotate[i] = na[i];

		/* Place your application code here. */
	}
}

/* [] END OF FILE */
