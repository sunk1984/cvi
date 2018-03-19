/******************************************************************************
    Relay_LPC1114.c
    API functions

    Copyright(C) 2011, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.

    History
    2011.12.16  ver.0.1.00   by Roger
******************************************************************************/
/******************************************************************************
    Note : �̵�����Ŵ�1�ſ�ʼ��channel��Ҳ �� 1�ſ�ʼ 
******************************************************************************/

/*
#include <utility.h>
#include "type.h"
#include "relay.h"
#include "comm_485.h"
*/
#include "includes.h"

//ÿ�����ӵ����channel ��
#define CHANNEL_NUMBER_PER_BOARD    24
#define RLY_BOARD_MAX               8
#define CHANNEL_NUMBER_ALL_BOARD    (RLY_BOARD_MAX * CHANNEL_NUMBER_PER_BOARD)

#define RLYFUNC_ON_OFF_CHAN  0x21
#define RLYFUNC_OFF_ALLCHAN  0x22
#define RLYFUNC_SCAN_CHAN    0x23
#define RLYFUNC_SET_MODE     0x24

#define RLYREG_SET_BOARD     0

const U8 RlyOnData[] = "01";
const U8 RlyOffData[] = "00";

const U8 ModeCommonlData[] = "01";
const U8 ModeAdData[] = "00";

static BOOL RLY_Chan_Total2Board(U8 * board_num, U8 * board_chan, U8 TotalChan)
{
	if(TotalChan > CHANNEL_NUMBER_ALL_BOARD)
	{
        return(FALSE);
    }
    if(TotalChan == 0)
    {
        return(FALSE);
	}
	
    * board_num = ((TotalChan-1) / CHANNEL_NUMBER_PER_BOARD) + 1;
    * board_chan = TotalChan % CHANNEL_NUMBER_PER_BOARD;
    
    if(* board_chan == 0)
    {
        * board_chan = CHANNEL_NUMBER_PER_BOARD;
    }
    
    return(TRUE);
}


static BOOL RLY_WriteCmd(U8 board_num, U8 func, U8 reg, U8 * WriteStr)
{
    return(MERAK_WriteCmd("RLY", board_num, func, reg, WriteStr));
}


BOOL RLY_ON(U32 TotalChan)
{
    U8 board_num;
    U8 board_chan;

	if(RLY_Chan_Total2Board(&board_num, &board_chan, TotalChan) == FALSE)
    {
        return(FALSE);
	}
    return(RLY_WriteCmd(board_num, RLYFUNC_ON_OFF_CHAN, board_chan, (U8 * )RlyOnData));
}
/*********************************************************************************
function:    RLY_OFF

description: �˺������ڹرն�Ӧ��relayͨ��

parameters:  Channelnum��ͨ����       
           
return: TRUE/FALSE
*********************************************************************************/
BOOL RLY_OFF(U32 TotalChan)
{
    U8 board_num;
    U8 board_chan;

	if(RLY_Chan_Total2Board(&board_num, &board_chan, TotalChan) == FALSE)
    {
        return(FALSE);
	}
    return(RLY_WriteCmd(board_num, RLYFUNC_ON_OFF_CHAN, board_chan, (U8 * )RlyOffData));
}

/*********************************************************************************
function:    RLY_OffAll

description: �˺������ڹر�ĳһ��relay������ͨ��

parameters:  board_num�����       
           
return: TRUE/FALSE
*********************************************************************************/
BOOL RLY_OffAll(U8 board_num)
{
    return(RLY_WriteCmd(board_num, RLYFUNC_OFF_ALLCHAN, RLYREG_SET_BOARD, ""));
}

/*********************************************************************************
function:    RLY_Clear

description: �˺������ڹر�ȫ��relay������ͨ��

parameters:  boards_sum��������   
           
return: TRUE/FALSE
*********************************************************************************/
BOOL RLY_Clear(U8 boards_sum)
{
    U8 i;

    for(i = 1; i <= boards_sum; i++)
    {
        if(RLY_OffAll(i) == FALSE)
        {
            break;
        }
        delay_ms(2);
    }
    
    delay_ms(50);

    if(i == (boards_sum+1))
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}


/*********************************************************************************
function:    RLY_Scan

description: �˺�������ɨ��ĳһ��relay�������ͨ��

parameters:  board_num�����       
           
return: TRUE/FALSE
*********************************************************************************/
BOOL RLY_Scan(U8 board_num)
{
    return(RLY_WriteCmd(board_num, RLYFUNC_SCAN_CHAN, RLYREG_SET_BOARD, ""));
}

/*********************************************************************************
function:    RLY_SetAdMode

description: �˺�����������relay���ģʽΪADģʽ

parameters:  board_num�����       
           
return: TRUE/FALSE
*********************************************************************************/
BOOL RLY_SetAdMode(U8 board_num)
{
    return(RLY_WriteCmd(board_num, RLYFUNC_SET_MODE, RLYREG_SET_BOARD, (U8 * )ModeAdData));
}

/*********************************************************************************
function:    RLY_SetCommonMode

description: �˺�����������relay���ģʽΪ��ͨģʽ

parameters:  board_num�����       
           
return: TRUE/FALSE
*********************************************************************************/
BOOL RLY_SetCommonMode(U8 board_num)
{
    return(RLY_WriteCmd(board_num, RLYFUNC_SET_MODE, RLYREG_SET_BOARD, (U8 * )ModeCommonlData));
}




