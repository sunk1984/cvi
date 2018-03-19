

/*******************************************************************************
    Initfile.c
    Read the initializtion file, Process the lines, and get the initializtion setting. and init the usart and AD calibration.

    Copyright(C) 2012, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.

    IDE:    IAR EWARM V6.4
    ICE:    J-Link
    BOARD:  Merak Main board

*******************************************************************************/

#include <formatio.h>
#include "includes.h"

U8 InitArray[CH_INITFILE_MAX] = 

"FCT port: 01\r\n"
"DUT port: 02\r\n"
"AUX port: 00\r\n"
"PRT port: 00\r\n"
;

char deviceName[4][9];
int portNo[4];

void Write_InitFile(void)
{
	int fb;
    
	fb = OpenFile(INIT_FILE, VAL_READ_WRITE, VAL_TRUNCATE, VAL_BINARY);

	if(fb != -1)
	{
	    WriteFile(fb, InitArray, strlen((char * )InitArray));
		CloseFile(fb);
    }
}

void Read_InitFile(void)
{
	int fb;

	fb = OpenFile(INIT_FILE, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);
	  
	if(fb == -1)
	    MessagePopup("Failed", "Can't open the init file");
	else
	{
        memset(InitArray, 0, CH_INITFILE_MAX);  // Clear the file.
        
		ReadFile(fb, InitArray, CH_INITFILE_MAX-1);
		CloseFile(fb);
	}
}

int Proc_InitStr(void)
{
    volatile int i = 0;
    char cpy_str[3] = {0};

    int status;

    for(i = 0; i < 4; i++)
    {
        strncpy(deviceName[i], InitArray+i*14, 8);	        // Get the serial number and name.
        
        cpy_str[0] = InitArray[i*14+10];
        cpy_str[1] = InitArray[i*14+11];
    	portNo[i] = strtod(cpy_str, NULL);    // Get the com number.

        if(portNo[i] > 0 && portNo[i] < 20 )
        {
            status = OpenComConfig( portNo[i], "", 115200, 0, 8, 1, 64, 64 );
            if(status < 0) {
                MessagePopup(deviceName[i], "Com setting error!");
                return FALSE;
            }
        }
    }

    return TRUE;
}


void Setting_toStr(void)
{
    volatile int i;
    
	for(i=0;i<4;i++)
	{
        InitArray[14*i+10] = portNo[i]/10+'0';
        InitArray[14*i+11] = portNo[i]%10+'0';
	}
}

