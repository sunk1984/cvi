
#ifndef TEST_LIB_ITEM__
#define TEST_LIB_ITEM__

#define ITEM_STR_MAX	24
#define CMD_STR_MAX	    48
#define ID_STR_MAX	    8
#define LCD_LSTR_MAX	ITEM_STR_MAX+ITEM_STR_MAX


typedef struct
{
	U8 item[ITEM_STR_MAX+1];

	U8 TestCmd[CMD_STR_MAX+1];
	U8 RspCmdPass[CMD_STR_MAX+1];
	U8 RspCmdFail[CMD_STR_MAX+1];

	U32 lower;
	U32 upper;

	U8 id[ID_STR_MAX+1];
	U8 lcdPrt[LCD_LSTR_MAX+1];

	U8 Channel;
	U8 Param;

	U32 retResult;

} ITEM_T, * P_ITEM_T;

typedef void (*TEST_FUNC)(P_ITEM_T pitem);

typedef struct
{
	char * TestIdStr;
	TEST_FUNC TestFunc;

} TEST_ID, * P_TEST_ID;

extern U8 Get_IdSum(void);

extern const TEST_ID TestIdTab[];

#endif


