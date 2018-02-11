#include <project.h>
#include "PS3_Controller.h"

int8   count = -1;
uint8  rx = 0;
uint8  rxdata[6] = {0};
uint16 sum = 0;

PS3Controller PS3_temp;

CY_ISR(ISR_PS3)
{
	PS3Controller ps3rxdata;
    rx = UART_PS3_GetByte();   
	if(count==-1)
    {
		if(rx==0x80)
        {
            sum=0;
			count=0;
		}
	}
	else if(count==6)
    {
		if(rx==(uint8)(sum&0x7F))
        {
			ps3rxdata.SQUARE=rxdata[0]&0x1;
			rxdata[0]>>=1;
			ps3rxdata.L1=rxdata[0]&0x1;
			rxdata[0]>>=1;
			ps3rxdata.L2=rxdata[0]&0x1;
			rxdata[0]>>=1;
			ps3rxdata.R1=rxdata[0]&0x1;
			rxdata[0]>>=1;
			ps3rxdata.R2=rxdata[0]&0x1;
            
			if((rxdata[1]&0x03)==0x3)
            {
				ps3rxdata.START=1;
 				rxdata[1]>>=2;
            }
            else
            {
                ps3rxdata.START=0;
                ps3rxdata.UP=rxdata[1]&0x1;
		    	rxdata[1]>>=1;
			    ps3rxdata.DOWN=rxdata[1]&0x1;
		    	rxdata[1]>>=1;
            }
			if((rxdata[1]&0x03)==0x3)
            {
				ps3rxdata.SELECT=1;
                rxdata[1]>>=2;
            }
			else
            {
				ps3rxdata.RIGHT=rxdata[1]&0x1;
				rxdata[1]>>=1;
				ps3rxdata.LEFT=rxdata[1]&0x1;
				rxdata[1]>>=1;
                ps3rxdata.SELECT=0;
			}
            
			ps3rxdata.TRIANGLE=rxdata[1]&0x1;
			rxdata[1]>>=1;
			ps3rxdata.CROSS=rxdata[1]&0x1;
			rxdata[1]>>=1;
			ps3rxdata.CIRCLE=rxdata[1]&0x1;
            
			ps3rxdata.ANALOG_LX=rxdata[2]&0x7f;
			ps3rxdata.ANALOG_LY=rxdata[3]&0x7f;
			ps3rxdata.ANALOG_RX=rxdata[4]&0x7f;
			ps3rxdata.ANALOG_RY=rxdata[5]&0x7f;
			
			PS3_temp=ps3rxdata;
		
		}
		count=-1;
		sum=0;
	}
	else
    {
		rxdata[count++]=rx;
		sum+=rx;
	}
}

void PS3_Start(void)
{
	UART_PS3_Start();
	UART_PS3_ClearRxBuffer();
	ISR_PS3_StartEx(ISR_PS3);
	ISR_PS3_Enable();
}

void PS3_Stop(void)
{
	UART_PS3_Stop();
	ISR_PS3_Disable();
}

PS3Controller PS3_Controller_get()
{
	return PS3_temp;
}


