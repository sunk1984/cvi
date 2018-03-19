/*********************************************************************
 File    : ADC_485.c 
Communication with Merak ADC board throgh RS485 interface

    Copyright(C) 2014, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.

    IDE:    IAR EWARM V6.4
    ICE:    J-Link
    BOARD:  Merak Main board(2014,5,22 rs485 interface)

    History
    2014.8.27  ver.1.00    ZJM first release  

Purpose : PWR for AT91SAM9260-EK and AT91SAM9XE-EK
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "includes.h"

#define ADCNUM_ONLY_1       1

#define ADCFUNC_RD_VOLT 	    0x41    //读电压命令
#define ADCFUNC_CALIBRATION 	0x42    //标定各路电压命令

#define AUTO_RANGE     0
#define OVERLOAD       2

/******************************************************************************
函数名称  : ADC_WriteCmd
输入参数  : func 功能码 reg寄存器 data_str数据
返回值    : TRUE/FALSE
功能描述  : 发送命令
******************************************************************************/
static BOOL ADC_WriteCmd(U8 func, U8 reg, U8 * data_str)
{
    return(MERAK_WriteCmd("ADC", ADCNUM_ONLY_1, func, reg, data_str));
}
/******************************************************************************
函数名称  : ADC_ReadCmd
输入参数  : func 功能码 reg寄存器 ReadStr
返回值    : TRUE/FALSE
功能描述  : 读命令
******************************************************************************/
static BOOL ADC_ReadCmd(U8 func, U8 reg, U8 * ReadStr)
{
    return(MERAK_ReadCmd("ADC", ADCNUM_ONLY_1, func, reg, ReadStr));
}

/******************************************************************************
函数名    : ADC_Calibration
参数      : 
返回值    : TRUE/FALSE
功能描述  :
******************************************************************************/
BOOL ADC_Calibration(U32 ref_volt)
{
    char hex_str[6] = {0};

    sprintf((char * )hex_str, "%d", ref_volt);
    return(ADC_WriteCmd(ADCFUNC_CALIBRATION, 0, (U8 * )hex_str));    //发送设置标定值命令
}

/******************************************************************************
函数名    : ADC_GetVolt
参数      : U32 *pVolt 
返回值    : TRUE/FALSE
功能描述  : 从AD板读取电压，单位mv
******************************************************************************/
BOOL ADC_GetVolt(U32 * pVolt)
{
    U8 voltStr[6] = {0};
    
    if(ADC_ReadCmd(ADCFUNC_RD_VOLT, AUTO_RANGE, voltStr))
    {
        *pVolt = atoi((char *)voltStr);     //ASCII转换成int型数字
         return(TRUE);
    }
    
    *pVolt = 0;
    return(FALSE);
}

