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

#define ADCFUNC_RD_VOLT 	    0x41    //����ѹ����
#define ADCFUNC_CALIBRATION 	0x42    //�궨��·��ѹ����

#define AUTO_RANGE     0
#define OVERLOAD       2

/******************************************************************************
��������  : ADC_WriteCmd
�������  : func ������ reg�Ĵ��� data_str����
����ֵ    : TRUE/FALSE
��������  : ��������
******************************************************************************/
static BOOL ADC_WriteCmd(U8 func, U8 reg, U8 * data_str)
{
    return(MERAK_WriteCmd("ADC", ADCNUM_ONLY_1, func, reg, data_str));
}
/******************************************************************************
��������  : ADC_ReadCmd
�������  : func ������ reg�Ĵ��� ReadStr
����ֵ    : TRUE/FALSE
��������  : ������
******************************************************************************/
static BOOL ADC_ReadCmd(U8 func, U8 reg, U8 * ReadStr)
{
    return(MERAK_ReadCmd("ADC", ADCNUM_ONLY_1, func, reg, ReadStr));
}

/******************************************************************************
������    : ADC_Calibration
����      : 
����ֵ    : TRUE/FALSE
��������  :
******************************************************************************/
BOOL ADC_Calibration(U32 ref_volt)
{
    char hex_str[6] = {0};

    sprintf((char * )hex_str, "%d", ref_volt);
    return(ADC_WriteCmd(ADCFUNC_CALIBRATION, 0, (U8 * )hex_str));    //�������ñ궨ֵ����
}

/******************************************************************************
������    : ADC_GetVolt
����      : U32 *pVolt 
����ֵ    : TRUE/FALSE
��������  : ��AD���ȡ��ѹ����λmv
******************************************************************************/
BOOL ADC_GetVolt(U32 * pVolt)
{
    U8 voltStr[6] = {0};
    
    if(ADC_ReadCmd(ADCFUNC_RD_VOLT, AUTO_RANGE, voltStr))
    {
        *pVolt = atoi((char *)voltStr);     //ASCIIת����int������
         return(TRUE);
    }
    
    *pVolt = 0;
    return(FALSE);
}
