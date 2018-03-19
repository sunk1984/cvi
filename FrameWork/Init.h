
#ifndef INIT_FILE__
#define INIT_FILE__

#define CH_INITFILE_MAX	(80)

#define INIT_FILE	"Init.csv"

extern char deviceName[4][9];
extern int portNo[4];

extern U8 InitArray[CH_INITFILE_MAX];

extern void Write_InitFile(void);
extern void Read_InitFile(void);
extern int Proc_InitStr(void);
extern void Setting_toStr(void);


#endif
