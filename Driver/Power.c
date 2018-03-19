/*********************************************************************
 File    : PWR.c 
Communication with Merak Power board throgh RS485 interface

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

#define PWRFUNC_SET_VOLT	    0x11//���ÿɵ��ڵ�Դ��ѹ����
#define PWRFUNC_ON_OFF_DUT 	    0x12//��Դ�������������
#define PWRFUNC_ON_OFF_AUX 	    0x13//�󱸵�Դ��������
#define PWRFUNC_RD_CURRENT 	    0x14//����������
#define PWRFUNC_VOL_ADD 	    0x15//�ɵ��ڵ�Դ��ѹ��������
#define PWRFUNC_VOL_SUB 	    0x16//�ɵ��ڵ�Դ��ѹ�½�����

#define PWRCHAN_DUT	            0x01//DUT��Դͨ��
#define PWRCHAN_MEARK	        0x02//meark��Դͨ��
#define PWRCHAN_AUX 	        0x01

#define PWRNUM_ONLY_1       1
#define MAX_VOL_DUT        24000    //DUT����ѹֵ

#define MCU_POWERIO_P7_3		AT91C_PIO_PC24

const U8 TurnOnData[]  = "01";
const U8 TurnOffData[] = "00";

/**************************************************************************** 
��������: Hex2Str32 
��������: ʮ������ת�ַ��� 
�������: str �ַ��� hex_data ʮ������ 
�������: �� 
*****************************************************************************/   
static void Hex2Str32(U8 * str, U32 hex_data)
{
    sprintf((char * )str, "%d", hex_data);
} 
/******************************************************************************
��������  : PWR_WriteCmd
�������  : func ������ reg�Ĵ��� data_str����
����ֵ    : TRUE/FALSE
��������  : ��������
******************************************************************************/
static BOOL PWR_WriteCmd(U8 func, U8 reg, U8 * data_str)
{
    return(MERAK_WriteCmd("PWR", PWRNUM_ONLY_1, func, reg, data_str));
}
/******************************************************************************
��������  : PWR_ReadCmd
�������  : func ������ reg�Ĵ��� ReadStr
����ֵ    : TRUE/FALSE
��������  : ������
******************************************************************************/
static BOOL PWR_ReadCmd(U8 func, U8 reg, U8 * ReadStr)
{
    return(MERAK_ReadCmd("PWR", PWRNUM_ONLY_1, func, reg, ReadStr));
}
/******************************************************************************
������    : PWR_TurnOnDut
����      : void
����ֵ    : TRUE/FALSE
��������  : ʹ�����DUT��Դ
******************************************************************************/
BOOL PWR_TurnOnDut(void)
{
    return(PWR_WriteCmd(PWRFUNC_ON_OFF_DUT, PWRCHAN_DUT, (U8 * )TurnOnData));
}

/******************************************************************************
������    : PWR_TurnOffDut
����      : void
����ֵ    : TRUE/FALSE
��������  : �ر����DUT��Դ
******************************************************************************/
BOOL PWR_TurnOffDut(void)
{
    return(PWR_WriteCmd(PWRFUNC_ON_OFF_DUT, PWRCHAN_DUT, (U8 * )TurnOffData));
}


/******************************************************************************
������    : PWR_TurnOnAux
����      : void
����ֵ    : TRUE/FALSE
��������  : ʹ�ܱ��ݵ�Դ
******************************************************************************/
BOOL PWR_TurnOnAux(void)
{
    return(PWR_WriteCmd(PWRFUNC_ON_OFF_AUX, PWRCHAN_AUX, (U8 * )TurnOnData));
}
/******************************************************************************
������    : PWR_TurnOnAux
����      : void
����ֵ    : TRUE/FALSE
��������  : �رձ��ݵ�Դ
******************************************************************************/
BOOL PWR_TurnOffAux(void)
{
    return(PWR_WriteCmd(PWRFUNC_ON_OFF_AUX, PWRCHAN_AUX, (U8 * )TurnOffData));
}
/******************************************************************************
������    : PWR_SetDutVolt
����      : volt ���õĵ�ѹֵ ��λmv ����Ҫ����12v volt=12000 
����ֵ    : TRUE/FALSE
��������  :����DUT���ѹֵ ���ֻ�����24V
******************************************************************************/
BOOL PWR_SetDutVolt(U32 volt)
{
    U8 VoltStr[6];
    //������õ�ѹ�����������ֵ,�����ֵ����
    if(volt > MAX_VOL_DUT) 
    {
        volt = MAX_VOL_DUT;
    }
    Hex2Str32(VoltStr, volt);//����ת����ASCII
 
    return(PWR_WriteCmd(PWRFUNC_SET_VOLT, PWRCHAN_DUT, (U8 * )VoltStr));//�������õ�ѹ����
}

BOOL PWR_SetAuxVolt(U32 volt)
{
    U8 VoltStr[6];

    Hex2Str32(VoltStr, volt);
    
    return(PWR_WriteCmd(PWRFUNC_SET_VOLT, PWRCHAN_AUX, (U8 * )VoltStr));
}
/******************************************************************************
������    : PWR_RdDUTCurrent
����      : pCur 
����ֵ    : TRUE/FALSE
��������  : ��ȡDUT���ص��� ��λma
******************************************************************************/
BOOL PWR_GetDUTCur(U32 *pCur)
{
    U8 VoltStr[6];
    if(PWR_ReadCmd(PWRFUNC_RD_CURRENT, PWRCHAN_DUT, (U8 * )VoltStr)==TRUE)//���Ͷ���������
    {
        *pCur=atoi((char *)VoltStr);//ASCIIת����int������
         return TRUE;
    }
    else
    {
        *pCur=0;
        return FALSE;
    }
}
/******************************************************************************
������    : PWR_AddDutVolt
����      : stepval ����ֵ   1��������ѹֵ0.09884v
����ֵ    : TRUE/FALSE
��������  : �ڵ�ǰ��ѹ�Ļ���������(0.09884*stepval)V��ѹ  
���統ǰ��ѹ5v  ִ��PWR_AddDutVolt(10)��DUT��ѹֵ=5+10*0.098=5.98v
******************************************************************************/
BOOL PWR_AddDutVolt(U32 Stepval)
{
    U8 VoltStr[6];

    Hex2Str32(VoltStr, Stepval);//����ת����ASCII
    return(PWR_WriteCmd(PWRFUNC_VOL_ADD, PWRCHAN_DUT, (U8 * )VoltStr));//���͵��ڵ�Դ��ѹ��������
}
/******************************************************************************
������    : PWR_SubDutVolt
����      : stepval ����ֵ
����ֵ    : TRUE/FALSE 1��������ѹֵ0.09884v
��������  : �ڵ�ǰ��ѹ�Ļ����ϼ�ȥ(0.09884*stepval)V��ѹ
���統ǰ��ѹ5v  ִ��PWR_AddDutVolt(10)��DUT��ѹֵ=5-10*0.098=4.02v
******************************************************************************/
BOOL PWR_SubDutVolt(U32 Stepval)
{
    U8 VoltStr[6];

    Hex2Str32(VoltStr, Stepval);//����ת����ASCII
    return(PWR_WriteCmd(PWRFUNC_VOL_SUB, PWRCHAN_DUT, (U8 * )VoltStr));//���͵��ڵ�Դ��ѹ�½�����
}

