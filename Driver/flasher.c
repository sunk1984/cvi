
#include "includes.h"

#define IMG_SUM_MAX 4

U8 FALSHER_init(U8 IOChan_ProgKey, U8 IOChan_PassFail_Led)
{
    EXTIO_ConfigureBitDirction(IOChan_ProgKey, IO_OUTPUT);
    EXTIO_ConfigureBitDirction(IOChan_PassFail_Led, IO_INPUT);
    
    return(TRUE);
//    return(EXTIO_WriteByte(IOChan_ProgKey, 0xff));
}

static void FALSHER_PressProgKey(U8 IOChan_ProgKey)
{
    EXTIO_WriteBit(IOChan_ProgKey, 0);
    delay_ms(50);
    EXTIO_WriteBit(IOChan_ProgKey, 1);
    delay_ms(100);
}

U8 FALSHER_Prog(U8 IOChan_ProgKey, U8 IOChan_PassFail_Led, U8 prog_time_s)
{
    U8 readData = 0;

    FALSHER_PressProgKey(IOChan_ProgKey);
    delay_ms(prog_time_s * 1000);
    
    EXTIO_ReadBit(IOChan_PassFail_Led, &readData);
    return(readData);
}

static void FALSHER_PressSelImgKey(U8 IOChan_SelImgKey)
{
    EXTIO_WriteBit(IOChan_SelImgKey, 0);
    delay_ms(50);
    EXTIO_WriteBit(IOChan_SelImgKey, 1);
    delay_ms(100);
}

U8 FALSHER_Sel_Img(U8 IOChan_SelImgKey, U8 IOChan_CurrImg_Led)
{
    U8 i;
    U8 readData = 0;

    for(i = 0; i < IMG_SUM_MAX; i++)
    {
        EXTIO_ReadBit(IOChan_CurrImg_Led, &readData);
        
        if(readData == 1)
        {
            break;
        }
        
        FALSHER_PressSelImgKey(IOChan_SelImgKey);
    }

    if(i < IMG_SUM_MAX)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}




