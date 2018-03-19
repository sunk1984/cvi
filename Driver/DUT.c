
#include "includes.h"

#define DEFAULT_TIMEOUT_MS          (100)
#define DEFAULT_CMD_REPEAD_TIMES    (2)

#define BUFF_SIZE   255

U8 rx_cnt = 0;
U8 rx_buf[BUFF_SIZE]; // can not be defined in function?

int SERIAL_GetFrameByEndByte(int port, U8 end_char, U8 * RecvBuff, U8 * FrameLen)
{
    U8 ch;

	if(GetInQLen(port) == FALSE)
	    return(FALSE);
    
    ch = ComRdByte(port);

	if(ch == 0)
    	return FALSE;

	if(ch == end_char)
	{
		memcpy(RecvBuff, rx_buf, rx_cnt);
		memset(rx_buf, 0, BUFF_SIZE);
		* FrameLen = rx_cnt;
		rx_cnt = 0;
		return TRUE;
	}
	else if(ch == '\n' )		// If find a 0x0a and end_char is not 0x0a,discardthis whole line.
	{
		memset(rx_buf, 0, BUFF_SIZE);
		rx_cnt = 0;
		return FALSE;
	}
	else
	{
		rx_buf[rx_cnt++] = ch;

		if(rx_cnt > 200)
		{
			memset(rx_buf, 0, BUFF_SIZE);
			rx_cnt = 0;
		}
		return FALSE;
	}
}

int SERIAL_GetLine(U32 com_dut, U8 * rbuf, U8 * len)
{
	return(SERIAL_GetFrameByEndByte(com_dut, '\r', rbuf, len));
}


static int DUT_PutGet(U8 * test_cmd, U8 * rsp_pass, U8 * rsp_fail)
{   
    volatile int timeout = 0;
    U8 rcnt;
    char tbuf[BUFF_SIZE] = {0};
    char rbuf[BUFF_SIZE] = {0};
    
    FlushInQ(portNo[PORT_DUT]);    // zhujianmin
    
    if(* test_cmd)
    {
        //sprintf((char *)tbuf, "%s\r\n", test_cmd);
        ComWrt(portNo[PORT_DUT], tbuf, strlen(tbuf));
        delay_ms(5);       // zhujianmin  
    }

    if(* rsp_pass == 0)
        return TRUE;

    while(1) 
    {      
        if(timeout++ > DEFAULT_TIMEOUT_MS)           
            break;
        
		if(SERIAL_GetLine(portNo[PORT_DUT], rbuf, &rcnt))  //Received a frame by '\n'.
		{
            if(strncmp(rbuf, (char * )rsp_pass, rcnt) == 0)
                return TRUE;
            
            if(* rsp_fail)
            {
                if(strncmp(rbuf, (char * )rsp_fail, rcnt) == 0)
                    break;
            }
		}
        delay_ms(1);    
    }   
    return FALSE;
}
/*
int buildKeypadPackage(U8 testcmd, U8 *data, U32 len, U8 *des)
{
	
	int packagelen;
	packagelen = len + 3;
	des[0]=0x15;//head
	des[1]=(char)packagelen;
	des[2]=(char)(packagelen>>8);
	des[3]=testcmd;
	if (len != 0) {
		strcpy();
	}
	
}
int Dut_GetRespon(U8 test_cmd, U8 * rsp_pass, U8 * rsp_fail)
{
    volatile int timeout = 0;
    U8 rcnt;
    char tbuf[BUFF_SIZE] = {0};
    char rbuf[BUFF_SIZE] = {0};
    
    FlushInQ(portNo[PORT_DUT]);    // zhujianmin
    
    if(* test_cmd)
    {
        sprintf((char *)tbuf, "%s\r\n", test_cmd);
        ComWrt(portNo[PORT_DUT], tbuf, strlen(tbuf));
        delay_ms(5);       // zhujianmin  
    }

    if(* rsp_pass == 0)
        return TRUE;

    while(1) 
    {      
        if(timeout++ > DEFAULT_TIMEOUT_MS)           
            break;
        
		if(SERIAL_GetLine(portNo[PORT_DUT], rbuf, &rcnt))  //Received a frame by '\n'.
		{
            if(strncmp(rbuf, (char * )rsp_pass, rcnt) == 0)
                return TRUE;
            
            if(* rsp_fail)
            {
                if(strncmp(rbuf, (char * )rsp_fail, rcnt) == 0)
                    break;
            }
		}
        delay_ms(1);    
    }   
    return FALSE;
	  
}*/

U32 DUT_CMD(P_ITEM_T pitem)
{
    U8 i;
    U8 Cmd_CntMax = DEFAULT_CMD_REPEAD_TIMES;
    
    if(strcmp((char * )pitem->id, "CMD") == 0)  //For "CMD" command, Param indicate maximum repeat time
    {
        if(pitem->Param)
            Cmd_CntMax = pitem->Param;
    }
    
	for(i=0; i<Cmd_CntMax; i++)
	{
    	if(DUT_PutGet(pitem->TestCmd, pitem->RspCmdPass, pitem->RspCmdFail))
    	    break;
	}
	
    if(i < Cmd_CntMax)
    {
        if(i)
            printf("command repeat times is %d\r\n", i);
        return(TRUE);
    }
    else
        return(FALSE);
}

U32 DUT_GetStr(U8 * str, P_ITEM_T pitem)
{
    volatile int timeout = 0;
    U8 rcnt;
    char tbuf[BUFF_SIZE] = {0};
    char rbuf[BUFF_SIZE] = {0};
    U8 len;

    FlushInQ(portNo[PORT_DUT]);    // zhujianmin

    sprintf((char *)tbuf, "%s\r\n", (char * )pitem->TestCmd);
    ComWrt(portNo[PORT_DUT], tbuf, sizeof(tbuf));
    delay_ms(5);       // zhujianmin  
    
    len = (U8)strlen((char * )pitem->RspCmdPass);
    
    while(1) 
    {      
        if(timeout++ > DEFAULT_TIMEOUT_MS)           
            break;
        
		if(SERIAL_GetLine(portNo[PORT_DUT], rbuf, &rcnt))
		{
            if(strncmp((char * )pitem->RspCmdPass, (char * )rbuf, len) == 0)
		    {
                strcpy((char * )str, (char *)(rbuf+len));
//                data = (U32)strtod((char *)str, NULL);
                return TRUE;
            }
		}
        delay_ms(1);    
    }   
    return FALSE;
}

U32 Aux_PutGet(U8 *test_cmd, U8 *rsp_pass, U8 *rsp_fail)   
{
	volatile int timeout = 0;
    U8 rcnt;
    char tbuf[BUFF_SIZE] = {0};
    char rbuf[BUFF_SIZE] = {0};
    
    FlushInQ(portNo[PORT_AUX]);    // zhujianmin
    
    if(* test_cmd)
    {
        sprintf((char *)tbuf, "%s\r\n", test_cmd);
        ComWrt(portNo[PORT_AUX], tbuf, strlen(tbuf));
        delay_ms(5);       // zhujianmin  
    }

    if(* rsp_pass == 0)
        return TRUE;

    while(1) 
    {      
        if(timeout++ > DEFAULT_TIMEOUT_MS)           
            break;
        
		if(SERIAL_GetLine(portNo[PORT_AUX], rbuf, &rcnt))  //Received a frame by '\n'.
		{
            if(strncmp(rbuf, (char * )rsp_pass, rcnt) == 0)
                return TRUE;
            
            if(* rsp_fail)
            {
                if(strncmp(rbuf, (char * )rsp_fail, rcnt) == 0)
                    break;
            }
		}
        delay_ms(1);    
    }   
    return FALSE;
}


