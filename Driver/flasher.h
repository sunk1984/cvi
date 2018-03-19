
#ifndef     __FLASHER_H__
#define     __FLASHER_H__

extern U8 FALSHER_init(U8 IOChan_ProgKey, U8 IOChan_PassFail_Led);
extern U8 FALSHER_Prog(U8 IOChan_ProgKey, U8 IOChan_PassFail_Led, U8 prog_time_s);
extern U8 FALSHER_Sel_Img(U8 IOChan_SelImgKey, U8 IOChan_CurrImg_Led);

#endif

