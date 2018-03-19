
#include "includes.h"

//#define DEBUG_CYCLE_TEST  //zjm

U8 MacEmpty = TRUE;
U8 barCode_List[5][20] = {0};

void TEST_BarcodeScan(P_ITEM_T pitem)
{
    int get_butt;
    char code_temp[20] = {0};
	
    get_butt = Scan_Serial(code_temp);
    
    if(get_butt == TRUE)    //press ok button
    {
        strcpy(barCode_List[pitem->Channel], code_temp);
	    pitem->retResult = PASS;
    }
    else
	    pitem->retResult = FAIL;
}

void TEST_WaitDUT(P_ITEM_T pitem)
{
    MessagePopup("ATTENTION", "Mount the DUT in the fixture");
	pitem->retResult = PASS;
}

void TEST_PowerOn(P_ITEM_T pitem)
{
    SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, "Power on DUT\r\n");
    
	pitem->retResult = (U32)PWR_TurnOnDut();

    delay_ms(100);

	RLY_ON(pitem->Channel);

	delay_ms(100);
}

void TEST_PowerOff(P_ITEM_T pitem)
{
    SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, "Power off DUT\r\n");
    
	pitem->retResult = (U32)PWR_TurnOffDut();

    delay_ms(100);
}

void TEST_PowerSet(P_ITEM_T pitem)
{
	PWR_TurnOffDut();
	PWR_SetDutVolt(pitem->upper);
    delay_ms(20);
	pitem->retResult = (U32)PWR_TurnOnDut();
    delay_ms(100);
}

void TEST_PowerOnAux(P_ITEM_T pitem)
{
	pitem->retResult = (U32)PWR_TurnOnAux();
    delay_ms(100);
}

void TEST_PowerOffAux(P_ITEM_T pitem)
{
	pitem->retResult = (U32)PWR_TurnOffAux();
}

void TEST_Delay(P_ITEM_T pitem)
{
    if(pitem->Param > 1)
	    printf((char * )"Delay %d s...\r\n", pitem->Param);
    
    delay_ms(pitem->Param * 1000);
	pitem->retResult = PASS;
}

void TEST_Command(P_ITEM_T pitem)
{
	pitem->retResult = DUT_CMD(pitem);
}

void TEST_BarcodeWr(P_ITEM_T pitem)
{
	U8 wrStr[30];

    if(MacEmpty == TRUE)
    {
        sprintf((char *)wrStr, "%s%s", (char * )pitem->TestCmd, (char * )barCode_List[pitem->Channel]);
        strcpy((char * )pitem->TestCmd, (char *)wrStr);
    	pitem->retResult = DUT_CMD(pitem);
    }
    else
        pitem->retResult = PASS;
}

void TEST_BarcodeRd(P_ITEM_T pitem)
{
	U8 rdStr[30];
	
    if(MacEmpty == TRUE)
    {
        sprintf((char *)rdStr, "%s%s", (char * )pitem->RspCmdPass, (char * )barCode_List[pitem->Channel]);
        strcpy((char * )pitem->RspCmdPass, (char *)rdStr);
    	pitem->retResult = DUT_CMD(pitem);
    }
    else
        pitem->retResult = PASS;
}

void TEST_CtrlExtIO(P_ITEM_T pitem)
{
    EXTIO_ConfigureBitDirction(pitem->Channel, IO_OUTPUT);
    pitem->retResult = EXTIO_WriteBit(pitem->Channel, pitem->Param);
}

void TEST_GetExtIO(P_ITEM_T pitem)
{
    U8 readData;

    EXTIO_ConfigureBitDirction(pitem->Channel, IO_INPUT);

    if(EXTIO_ReadBit(pitem->Channel, &readData))
    {
        if(readData == pitem->Param)    // Bug
        {
            pitem->retResult = PASS;
            return;
        }
    }
    
    pitem->retResult = FAIL;
}


void TEST_CtrlRly(P_ITEM_T pitem)
{
    if(pitem->Param == 1)
	    pitem->retResult = (U32)RLY_ON((U32)pitem->Channel);
    else
	    pitem->retResult = (U32)RLY_OFF((U32)pitem->Channel);
    
//SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, "relay on...");  // zhujianmin
//pitem->retResult = PASS;    // zhujianmin

    delay_ms(100);
}


void TEST_ClearRly(P_ITEM_T pitem)
{
    U8 board_num = pitem->Channel;
    
    pitem->retResult = (U32)RLY_OffAll((U32)board_num);
}

void TEST_ClearAllRly(P_ITEM_T pitem)
{
    U8 boards_sum = pitem->Param;

    pitem->retResult = RLY_Clear(boards_sum);
}

void TEST_VoltageTest(P_ITEM_T pitem)
{
	U32 volt = 0;
    U8 str[10];

    SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, "Start measure voltage...\r\n");
    
	RLY_ON((U32)pitem->Channel);
    delay_ms(70);
//    ADC_GetVolt(&volt);
	RLY_OFF((U32)pitem->Channel);

    sprintf((char *)str, "%2d.%03dV\r\n", volt/1000, volt%1000);
    SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, str);
	
    delay_ms(70);

	if(volt >= pitem->lower && volt <= pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
}

void TEST_CalcCurrTest(P_ITEM_T pitem)
{
	U32 volt_former;
	U32 volt_latter;
	U32 volt_diff;
	U32 resis;  //resis range: 0.1 OHm to 99 OHm
	U32 curr;
    U8 str[10];

	RLY_ON((U32)pitem->Channel);
    delay_ms(70);
    ADC_GetVolt(&volt_former);
	RLY_OFF((U32)pitem->Channel);
    sprintf((char * )str, "former=%2d.%03dV\r\n", volt_former/1000, volt_former%1000);
	printf((char * )str);
    delay_ms(50);

	RLY_ON((U32)(pitem->Channel+1));
    delay_ms(50);
    ADC_GetVolt(&volt_latter);
	RLY_OFF((U32)pitem->Channel+1);
    sprintf((char * )str, "latter=%2d.%03dV\r\n", volt_latter/1000, volt_latter%1000);
	printf((char * )str);

    resis = pitem->Param; // resis = Param, For example : Param = 8, resis is 8 Ohm

    volt_diff = volt_former - volt_latter;
    curr = volt_diff/resis;
    sprintf((char * )str, "curr=%2d.%03dA", curr/1000, curr%1000);
	printf(str);

	if(curr >= pitem->lower && curr <= pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
}

void TEST_ReadCurrTest(P_ITEM_T pitem)
{
    U32 DutCur;
    U8 str[10];
    
    if(PWR_GetDUTCur((U32 * )&DutCur) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }
    
    sprintf((char * )str, "DutCurr=%2d.%03dA", DutCur/1000, DutCur%1000);
	printf(str);

	if(DutCur >= pitem->lower && DutCur <= pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
}


void TEST_ManualTest(P_ITEM_T pitem)
{
    int confirm_res;
    
    if(DUT_CMD(pitem) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }
	
    confirm_res = ConfirmPopup("JUDGE", (U8 *)pitem->lcdPrt);
    if(confirm_res == TRUE)
	    pitem->retResult = PASS;
    else
	    pitem->retResult = FAIL;

}

void TEST_GpioInTest(P_ITEM_T pitem)
{
	U8 pos;
	U8 lev;

    pos = (U8)(strlen((char *)pitem->RspCmdPass) - 1);    //PCA9555INT:X
    lev = pitem->RspCmdPass[pos] - '0';

    EXTIO_ConfigureBitDirction(pitem->Channel, IO_OUTPUT);
    EXTIO_WriteBit(pitem->Channel, lev);

	pitem->retResult = DUT_CMD(pitem);
}

void TEST_GpioInRlyTest(P_ITEM_T pitem)
{
	U8 pos;
	U8 lev;

    pos = (U8)(strlen((char *)pitem->RspCmdPass) - 1);    //PCA9555INT:X
    lev = pitem->RspCmdPass[pos] - '0';

    if(lev == 0)
	    RLY_ON((U32)pitem->Channel);
    else
	    RLY_OFF((U32)pitem->Channel);

	pitem->retResult = DUT_CMD(pitem);
}

void TEST_GpioOutTest(P_ITEM_T pitem)
{
	U8 readData;
	U8 pos;
	U8 UseInvert;
	U8 cmdLev;

    pos = (U8)(strlen((char *)pitem->TestCmd) - 1);
    cmdLev = pitem->TestCmd[pos] - '0';

    UseInvert = pitem->Param;

    if(UseInvert == TRUE)
        cmdLev = 1 - cmdLev;

    EXTIO_ConfigureBitDirction(pitem->Channel, IO_INPUT);
    
    if(DUT_CMD(pitem) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }

    delay_ms(20);
    EXTIO_ReadBit(pitem->Channel, &readData); 

	if(readData == cmdLev)
		pitem->retResult = PASS;
    else
	    pitem->retResult = FAIL;
}

void TEST_GpioIn16Test(P_ITEM_T pitem)
{
	U8 * p;
    U8 start_pin;
	U16 out_data;

    start_pin = pitem->Channel;
    p = pitem->RspCmdPass + (strlen((char *)pitem->RspCmdPass) - 4);    //ALARMIN:XXXX
    out_data = (U32)strtol((char *)p, NULL, 16);

    EXTIO_ConfigureByteDirction(start_pin, IO_OUTPUT);
    EXTIO_ConfigureByteDirction(start_pin+8, IO_OUTPUT);

    EXTIO_WriteByte(start_pin, (U32)out_data);
    EXTIO_WriteByte(start_pin+8, (U32)(out_data>>8));

	pitem->retResult = DUT_CMD(pitem);
}

void TEST_AudioLoopTest(P_ITEM_T pitem)
{
    U16 tx_amp;

    tx_amp = pitem->Param * 10; // Param is only assigned from 0 to 255, so amp is Param *10 , 80 means 800mv
    
    if(pitem->Channel)
        RLY_ON(pitem->Channel);
    
	delay_ms(500);
    
    if(DUT_CMD(pitem) == TRUE)
        pitem->retResult = (U32)Audio_LoopTest(tx_amp, pitem->lower, pitem->upper);
    else
        pitem->retResult = FAIL;
    
    if(pitem->Channel)
        RLY_OFF(pitem->Channel);
}

void TEST_AudioDecTest(P_ITEM_T pitem)
{
    U16 exp_freq;

    exp_freq = pitem->Param * 100; // Param is only assigned from 0 to 255, so amp is Param *10 , 80 means 8000Hz
    
    if(pitem->Channel)
        RLY_ON(pitem->Channel);
    
	delay_ms(500);
    
    if(DUT_CMD(pitem) == TRUE)
        pitem->retResult = Audio_TestSimpTone(exp_freq, AUDIO_FREQ_TOLERANCE, pitem->lower, pitem->upper);
    else
        pitem->retResult = FAIL;
    
    if(pitem->Channel)
        RLY_OFF(pitem->Channel);
}

void TEST_GenAudio(P_ITEM_T pitem)
{
    U16 freq;
    U16 amp;

    freq = pitem->lower;
    amp = pitem->upper;
    
    if(pitem->Channel)
        RLY_ON(pitem->Channel);
    
	delay_ms(300);
    
    if(DUT_CMD(pitem) == TRUE)
        pitem->retResult = (U32)Audio_GenSimpTone(freq, amp);
    else
        pitem->retResult = FAIL;
    
    if(pitem->Channel)
        RLY_OFF(pitem->Channel);
}

void TEST_StopAudio(P_ITEM_T pitem)
{
    pitem->retResult = Audio_StopSignal();
}

void TEST_DecAudioFreq(P_ITEM_T pitem)
{
//    pitem->retResult = Audio_DecSimpTone(pitem->lower, pitem->upper);
}

void TEST_AudInitDTMF(P_ITEM_T pitem)
{
    pitem->retResult = Audio_EnterDTMF();
}

void TEST_AudDecDTMF(P_ITEM_T pitem)
{
    U8 DecNumber[16];
    U8 ExpNumber[16];

    strcpy((char * )ExpNumber, (char * )pitem->RspCmdPass);
    
    if(Audio_DecDTMF(DecNumber))
    {
        if(strcmp((char * )ExpNumber, (char * )DecNumber) == 0)
        {
            pitem->retResult = PASS;
            return;
        }
    }
    
    pitem->retResult = FAIL;
}

void TEST_CompAudioAmp(P_ITEM_T pitem)
{
//    pitem->retResult = Audio_CompToneAmp(pitem->lower, pitem->upper);
}

void TEST_CommTest(P_ITEM_T pitem)
{
	pitem->retResult = DUT_CMD(pitem);
}


static U32 Get_IP_Address(U8 * ip_str)
{
	U32 DestIpAddr = 0xC0A8010A;  //Default IP address is 192.168.1.10;   
	char * C_addr = "192.168.";
    U32 Ip12 = 0xC0A80000;    //192.168.0.0
    U8 Ip3, Ip4;
    U8 * sta4;

    if(strncmp((char * )ip_str, C_addr, strlen(C_addr)) == 0)
    {
        sta4 = (U8 *)(strrchr((char *)(ip_str + 9), '.') + 1);
        Ip3 = (U8)strtod((char *)(ip_str + 8), NULL);
        Ip4 = (U8)strtod((char *)sta4, NULL);
        
	    DestIpAddr = Ip12 + (U16)(Ip3<<8) + Ip4;
    }
    return(DestIpAddr);
}

void TEST_EthernetTest(P_ITEM_T pitem)
{
	U32 DestIpAddr;
	
    DestIpAddr = Get_IP_Address(pitem->RspCmdFail);
//	pitem->retResult = IP_Ping_Test(DestIpAddr);
}

void TEST_BuzzTest(P_ITEM_T pitem)
{
    if(DUT_CMD(pitem))
        pitem->retResult = Audio_TestBuzz(pitem->Param*100, pitem->lower, pitem->upper);
    else
        pitem->retResult = FAIL;
}

void TEST_LedTest(P_ITEM_T pitem)
{
	U32 volt;
    U8 str[10];
	
    if(DUT_CMD(pitem) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }

	RLY_ON((U32)pitem->Channel);
    delay_ms(300);

    ADC_GetVolt(&volt);

	RLY_OFF((U32)pitem->Channel);
    delay_ms(20);

    sprintf((char *)str, "%2d.%03dV\r\n", volt/1000, volt%1000);
    printf(str);

	if(volt >= pitem->lower && volt < pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
}


void TEST_PortTxTest(P_ITEM_T pitem)
{
//	pitem->retResult = Cmd_Ack(pitem->Channel, "", pitem->RspCmdPass, "");
}

void TEST_PortRxTest(P_ITEM_T pitem)
{
    U8 txCmd[40];

    sprintf((char *)txCmd, "%s\r\n", (char * )pitem->TestCmd);
//    UsartPutStr(pitem->Channel, txCmd);

	pitem->retResult = PASS;
}


void TEST_USB_DevTest(P_ITEM_T pitem)
{
    pitem->retResult = PASS;
}

void TEST_ADinTest(P_ITEM_T pitem)
{
    U32 adc;
    U8 str[10];
    
    if(DUT_GetStr(str, pitem) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }

    adc = (U32)strtod((char *)str, NULL);
    sprintf((char *)str, "DATA: %d\r\n", adc);
    printf(str);
    
	if(adc >= pitem->lower && adc <= pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
}

void TEST_KeyTest(P_ITEM_T pitem)
{
    /*
    U8 i;
    
	LCD_Display2Line(LCD_LINE3, (U8 *)pitem->lcdPrt);
	HMI_LongWarnBuzz();

    pitem->retResult = FAIL;

	for(i = 0; i < 255; i++)
	{
        if(DUT_CMD(pitem) == TRUE)
        {
            pitem->retResult = PASS;
            break;
        }
		if(HMI_PressNoKey())
		{
			pitem->retResult = FAIL;
			break;
		}
    	
    	delay_ms(200);
    }
    
	HMI_LongWarnBuzzOff();
	HMI_PassBuzz();
	LCD_Clear(LCD_LINE3);
	LCD_Clear(LCD_LINE4);
	*/
}

void TEST_FlasherReady(P_ITEM_T pitem)
{
    U8 IOChan_ProgKey;
    U8 IOChan_PassFail_Led;

    IOChan_ProgKey = pitem->Channel;
    IOChan_PassFail_Led = (U8)(pitem->upper/1000);
    
    pitem->retResult = FALSHER_init(IOChan_ProgKey, IOChan_PassFail_Led);
}

void TEST_FlasherProgram(P_ITEM_T pitem)
{
    U8 IOChan_ProgKey;
    U8 IOChan_PassFail_Led;
    U8 prog_time_s;

    IOChan_ProgKey = pitem->Channel;
    IOChan_PassFail_Led = (U8)(pitem->upper/1000);
    prog_time_s = pitem->Param;
    
    pitem->retResult = (U32)FALSHER_Prog(IOChan_ProgKey, IOChan_PassFail_Led, prog_time_s);
}

void TEST_FlasherSelImg(P_ITEM_T pitem)
{
    U8 IOChan_SelImgKey;
    U8 IOChan_CurrImg_Led;

    IOChan_SelImgKey = pitem->Channel;
    IOChan_CurrImg_Led = (U8)(pitem->upper/1000);
    
    pitem->retResult = (U32)FALSHER_Sel_Img(IOChan_SelImgKey, IOChan_CurrImg_Led);
}

void TEST_Zone(P_ITEM_T pitem)
{
	 U8 Buff[16]={0};
	 U32 zoneVolt=0;
	 
	 RLY_ON((U32)pitem->Channel);
     delay_ms(300);
	 
	 DUT_GetStr(Buff, pitem);
	 
	 RLY_OFF((U32)pitem->Channel);
     delay_ms(20);
	 
	 zoneVolt= atoi(Buff);
	if(zoneVolt >= pitem->lower && zoneVolt <= pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
}

void TEST_TriggerOut(P_ITEM_T pitem)
{
	U32 volt=0;
    U8 str[10]={0};
	if(DUT_CMD(pitem) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }
	RLY_ON((U32)pitem->Channel);
    delay_ms(300);

    ADC_GetVolt(&volt);

	RLY_OFF((U32)pitem->Channel);
    delay_ms(20);
    //printf(str);

	sprintf((char *)str, "%2d.%03dV\r\n", volt/1000, volt%1000);
    SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, str);
	
	if(volt >= pitem->lower && volt<= pitem->upper)
		pitem->retResult = PASS;
	else
		pitem->retResult = FAIL;
	
}

void  TEST_IB2(P_ITEM_T pitem)
{
	//U8 recvStr[0];
	if(DUT_CMD(pitem) == FALSE)
    {
        pitem->retResult = FAIL;
        return;
    }
	
	//strcpy((char *)pitem->RspCmdPass,(char *)pitem->lcdPrt);
    strcpy((char *)pitem->TestCmd,"");
	pitem->retResult = Aux_PutGet("","IB2test","ERROR");
}

const TEST_ID TestIdTab[] = 
{
	{"BAR_SCA", TEST_BarcodeScan},
	{"BAR_WR", TEST_BarcodeWr},
	{"BAR_RD", TEST_BarcodeRd},
	{"WAITDUT", TEST_WaitDUT},
	{"DELAY", TEST_Delay},
	{"CMD", TEST_Command},
	{"EIO_CTL", TEST_CtrlExtIO},
	{"EIO_GET", TEST_GetExtIO},
	{"RLY_CTL", TEST_CtrlRly},
	{"RLY_CLR", TEST_ClearRly},
	{"RLYCLRA", TEST_ClearAllRly},
	{"PWR_ON", TEST_PowerOn},
	{"PWR_OFF", TEST_PowerOff},
	{"PWR_SET", TEST_PowerSet},
	{"PWB_ON", TEST_PowerOnAux},
	{"PWB_OFF", TEST_PowerOffAux},
	{"COMM_T", TEST_CommTest},
	{"GPINR_T", TEST_GpioInRlyTest},
	{"GPIN_T", TEST_GpioInTest},
	{"IN16_T", TEST_GpioIn16Test},
	{"GPOUT_T", TEST_GpioOutTest},
	{"VOLG_T", TEST_VoltageTest},
	{"RCURR_T", TEST_ReadCurrTest},
	{"CCURR_T", TEST_CalcCurrTest},
	{"AUDLP_T", TEST_AudioLoopTest},
	{"AUDDEC", TEST_AudioDecTest},
	{"AUDGEN", TEST_GenAudio},
	{"AUDEND", TEST_StopAudio},
	{"AUDFREQ", TEST_DecAudioFreq},
	{"AUDAMP", TEST_CompAudioAmp},
	{"NET_T", TEST_EthernetTest},
	{"BUZZ_T", TEST_BuzzTest},
	{"MAN_T", TEST_ManualTest},
	{"LED_T", TEST_LedTest},
	{"PORTT_T", TEST_PortTxTest},
	{"PORTR_T", TEST_PortRxTest},
	{"USBDV_T", TEST_USB_DevTest},
	{"KEY_T", TEST_KeyTest},
	{"ADC_T", TEST_ADinTest},
	{"DTMFRDY", TEST_AudInitDTMF},
	{"DTMFDEC", TEST_AudDecDTMF},
	{"FLS_RDY", TEST_FlasherReady},
	{"FLS_PRG", TEST_FlasherProgram},
	{"FLS_SEL", TEST_FlasherSelImg},
	{"ZONE_T",  TEST_Zone},
	{"TRIGGER_T",TEST_TriggerOut},
	{"IB2_T",TEST_IB2},
};

U8 Get_IdSum(void)
{
    return(sizeof(TestIdTab)/sizeof(TEST_ID));
}




