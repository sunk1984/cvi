

#include "includes.h"

//Config CFG file

char TestItemArray[CH_PERCFG_MAX] = 
"#ITEM,COMMAND,RESPONSE PASS,RESPONSE FAIL,LOWER(V),UPPER(V),ID,LCD PRINT,IO RLY CHANNEL,PARAM\r\n"
"Voltage Test,,,,0,20,VOLG_T,,3,\r\n"
;


extern void Fixture_Init(void); 

void delay_ms(int ms)
{
    volatile int i;
    
    for(i=0;i<ms;i++)
        Delay(0.001);
}

#define RELAY_BOARD_MAX     4




void PowerOff_All(void)
{
    PWR_TurnOffDut();
    RLY_Clear(RELAY_BOARD_MAX);
}


void TEST_APP_xx(P_ITEM_T pitem)
{
    pitem->retResult = PASS;
}

void TEST_APP_GeyType(P_ITEM_T pitem)
{
	pitem->retResult = PASS;
}


const TEST_ID TestAppIdTab[] = 
{
	{"XXXXXXX", TEST_APP_xx},
//	{"ZONE_T", PowerOff_All},
//	{"GETTYP", TEST_APP_GetType},
//	{"APP_VER", TEST_APP_VerifyAPP},
//	{"COM_CFG", TEST_APP_ComCFG},
};

U8 Get_App_IdSum(void)
{
    return((U8)sizeof(TestAppIdTab)/sizeof(TEST_ID));
}



