/*******************************************************************************
    ExtIO_drv_PCA9505.h
    header function file of ExtIO driver (CHIP:PCA9505)

    Copyright(C) 2012, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.

    IDE:    IAR EWARM V6.10.5 6105-2331  
    ICE:    J-Link V8
    BOARD:  Merak EXTIO DRIVER board V1.0

    History
    2012.12.17  ver.1.00    First release by Roger gu   
    2013.2.22   ver.1.10    2nd release by Roger gu  
                delete  orginal table init method 
                add functions EXTIO_ConfigureByteDirction() and EXTIO_ConfigureBankDirction()
    2013.2.27   ver.1.20    3nd release by Roger gu
                revives EXTIO_ReadBank() bug
*******************************************************************************/


/*****************************************************************************
ʹ��˵��:

1.slave_add ָ ����ExtIO board�ĵ�ַ���е���R22��������һ�飬Ҳ���Թ��档

2.pin��16���Ʊ�ʾPCA9505�����š�
  ���� pin-->0x15 ������� IO 1.5��
  pin-->0x23 �������IO 2.3.

3.bank �У�0��ʾbank 0, 1 ��ʾbank 1���Դ�����

4.direction�����ֱ�ʾ IO_OUTPUT��IO_INPUT��

5.value �����ֱ�ʾ0,1.�����ã�����0��


******************************************************************************/
#include "type.h"

#ifndef     __IO_H__
#define     __IO_H__

//����IO�ṹ�еķ���
#define IO_OUTPUT 0
#define IO_INPUT  1

#define IO_CHANNEL_TOTAL  24

//EXTIO board�Ľṹ����
struct IO_INFO
{ 
    U32 Channel;     //���ź�
    U32 Direction; //����
    U32 Value;     //output����ʱ��ֵ
};


//����bit����
extern BOOL EXTIO_ConfigureBitDirction(U8 TotalChan, U8 BitDirData);

//����byte�ķ���
extern BOOL EXTIO_ConfigureByteDirction(U8 TotalChan, U8 ByteDirData);

//��дһ��byte�ĺ���
extern BOOL EXTIO_WriteByte(U8 TotalChan, U8 ByteData);
extern BOOL EXTIO_ReadByte(U8 TotalChan, U8 * ReadByte);

//��дĳһ��bit�ĺ���
extern BOOL EXTIO_WriteBit(U8 TotalChan, U8 BitData);
extern BOOL EXTIO_ReadBit(U8 TotalChan, U8 * ReadBit);

//
extern U32 EXTIO_Init(struct IO_INFO io_info[]);

#endif



