
#ifndef CFG_FILE__
#define CFG_FILE__

#define CH_PERSTR_MAX	(50)
#define CH_PERLINE_MAX	(250)
#define CH_PERCFG_MAX	(10000)

extern char TestItemArray[];
extern const TEST_ID TestAppIdTab[];

extern U8 Get_App_IdSum(void);
extern void CFG_FILE_Proc(void);

#endif
