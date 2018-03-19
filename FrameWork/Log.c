
#include <formatio.h>
#include "includes.h"

#define LOG_LEN_1_PCS           (15000)

#define LOG_FILE	"TestLog.txt"

static U8 logStr[LOG_LEN_1_PCS] = {0};

void LOGFILE_AddItem(U8 * str)
{
	strcat((char * )logStr, (char * )str);
}

void LOGFILE_Write(void)
{
	int FileHandle;

    static char fileStr[LOG_LEN_1_PCS];

	FileHandle = OpenFile(LOG_FILE, VAL_READ_WRITE, VAL_APPEND, VAL_BINARY);
	if(FileHandle != -1)
	{
	    WriteFile(FileHandle, logStr, strlen((char * )logStr));
		CloseFile(FileHandle);
    }
	
    memset((char * )logStr, 0, sizeof(logStr));
}



