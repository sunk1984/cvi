/*******************************************************************************
    Copyright(C) 2012, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.
    File name:  Test_comm_dut.h
    Function: FCT communication test with DUT head file
    IDE:    IAR EWARM V6.21 
    ICE:    J-Link 
    BOARD:  Merak board V1.0
    History
    2012.02.28  ver.1.00    First released by Roger                                 
*******************************************************************************/
#ifndef _HONEYWELL_FCT_COMM_DUT_H_
#define _HONEYWELL_FCT_COMM_DUT_H_

#include "includes.h"

extern int SERIAL_GetLine(U32 com_dut, U8 * rbuf, U8 * len);

extern U32 DUT_CMD(P_ITEM_T pitem);
extern U32 DUT_GetStr(U8 * str, P_ITEM_T pitem);
extern U32 Aux_PutGet(U8 *test_cmd, U8 *rsp_pass, U8 *rsp_fail);  

#endif
