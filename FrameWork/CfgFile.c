
/*******************************************************************************
    Cfgfile.c
    Read the testing configuration file, Process the lines, and get the item struct. and test this item.

    Copyright(C) 2012, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.

    IDE:    IAR EWARM V6.4
    ICE:    J-Link
    BOARD:  Merak Main board

*******************************************************************************/


#include <formatio.h>
#include "includes.h"

#define DUT_TYPE_AIO_FCT   0
#define DUT_TYPE_AIO_FT    1
#define DUT_TYPE_HYBRID    2
#define DUT_TYPE_KEYPAD    3

#define CFG_FILE_AIO_FCT	"AIO_FCT_CFG.csv"
#define CFG_FILE_AIO_FT	    "AIO_FT_CFG.csv"
#define CFG_FILE_HYBRID	    "HYBRID_CFG.csv"
#define CFG_FILE_KEYPAD	    "KEYPAD_CFG.csv"

//#define UPDATE_CFGFILE  // zhujianmin

extern int mainPanel;

extern void PowerOff_All(void);

/******************************************************************************
    Routine Name    : Get_A_String
    Form            : static U8 Get_A_String(U8 * line, U8 * str)
    Parameters      : line, str
    Return value    : The length of the string. 
    Description     : Take a string from a line by comma or LF char, return the length of the string.
******************************************************************************/
static U8 Get_A_String(U8 * line, U8 * str)
{
	U8 i;

    memset(str, 0, CH_PERSTR_MAX);  // Clear the string.
    
	for(i=0;i<CH_PERSTR_MAX;i++)
	{
		if(line[i] == ',' || line[i] == '\r')  // Look for a comma or a LF char.
		{
			strncpy((char * )str, (char * )line, i);	//Got a string.
			break;
		}
	}
	return(i + 1);
}

static U8 Get_A_Line(U8 * filestr, U8 * line)
{
	U8 i;

    memset(line, 0, CH_PERLINE_MAX);  // Clear the line.
    
	for(i=0;i<CH_PERLINE_MAX;i++)
	{
		if(filestr[i] == '\n')  // Look for a comma or a LF char.
		{
			strncpy((char * )line, (char * )filestr, i);	//Got a string.
			break;
		}
	}
	return(i + 1);
}

/******************************************************************************
    Routine Name    : ProcItem
    Form            : static BOOL ProcItem(P_ITEM_T pItem)
    Parameters      : pItem
    Return value    : TRUE/FALSE
    Description     : Process the item struct, Compare the testing ID with the ID table, Run the relative function.
******************************************************************************/
static BOOL ProcItem(P_ITEM_T pItem)
{
	U8 i;
	BOOL ret = FALSE;
	U8 id_sum;
	U8 app_id_sum;
	
	id_sum = Get_IdSum();    // Get the sum of testing ID from the TestIdTable. 

	for(i=0; i<id_sum; i++)
	{
		if(strcmp(TestIdTab[i].TestIdStr, (char *)pItem->id) == 0)    // Get the sum of testing ID from the TestIdTable. 
		{
            SetCtrlVal(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, pItem->item);
            SetCtrlVal(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, " ........................... ");

            SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, pItem->item);
            SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, "\r\n");

            LOGFILE_AddItem(pItem->item);
            LOGFILE_AddItem("\r\n");

			(TestIdTab[i].TestFunc)(pItem);     // Testing. 

            SetCtrlVal(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, result_str[pItem->retResult]);
            
            SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, result_str[pItem->retResult]);
            SetCtrlVal(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, "\r\n");

            LOGFILE_AddItem(result_str[pItem->retResult]);
            LOGFILE_AddItem("\r\n");

            ret = pItem->retResult;
//            ret = PASS;
            
			break;
		}
	}

    if(i != id_sum)
    {
	    return(ret);
    }
    
	app_id_sum = Get_App_IdSum();    // Get the sum of testing ID from the TestIdTable. 

	for(i=0; i<app_id_sum; i++)
	{
		if(strcmp(TestAppIdTab[i].TestIdStr, (char *)pItem->id) == 0)    // Get the sum of testing ID from the TestIdTable. 
		{
            SetCtrlVal(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, pItem->item);
            SetCtrlVal(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, " ... ... ");

			(TestAppIdTab[i].TestFunc)(pItem);     // Testing. 

            SetCtrlVal(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, result_str[pItem->retResult]);

            ret = pItem->retResult;
            
			break;
		}
	}
	
	return(ret);
}

/******************************************************************************
    Routine Name    : ProcLine
    Form            : static BOOL ProcLine(U8 * line)
    Parameters      : line
    Return value    : TRUE/FALSE
    Description     : Process the line taken from file, Get a item from the line, then process the item.
******************************************************************************/
static BOOL ProcLine(U8 * line)
{
	BOOL ret = FALSE;
    U8 pos = 0;
    U8 str[CH_PERSTR_MAX];

    ITEM_T testItem;
	P_ITEM_T pItem = &testItem;

    memset(pItem, 0, sizeof(ITEM_T));   //Clear item.

	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX)	    //Get a string. 
	{
	    strcpy((char * )pItem->item, (char * )str);	        // Get the serial number and name.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX)	    //Get a string. 
	{
	    strcpy((char * )pItem->TestCmd, (char * )str);	    // Get the command.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX)	    //Get a string. 
	{
	    strcpy((char * )pItem->RspCmdPass, (char * )str);	// Get the response case pass.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX) 	//Get a string. 
	{
	    strcpy((char * )pItem->RspCmdFail, (char * )str);	// Get the response case fail.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX) 	//Get a string. 
	{
	    pItem->lower = (U32)(atof((char * )str) * 1000);	// Get the lower limit.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX) 	//Get a string. 
	{
	    pItem->upper = (U32)(atof((char * )str) * 1000);	// Get the upper limit.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX)	//Get a string. 
	{
    	strcpy((char * )pItem->id, (char * )str);	    // Get the ID.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX)	//Get a string.  
	{
    	strcpy((char * )pItem->lcdPrt, (char * )str);	// Get the display string.
	    line += pos;
	}
	if((pos = Get_A_String(line, str)) < CH_PERSTR_MAX)	//Get a string.  
	{
    	pItem->Channel = (U8)strtod((char * )str, NULL);	// Get the channel on IO/RLY board.
	    line += pos;
	}
	if(Get_A_String(line, str))	                        //Get a string. 
	{
    	pItem->Param = (U8)strtod((char * )str, NULL);	// Get the parameter.
	}

    ret = ProcItem(pItem);
	
	return(ret);
}

int dut_type;

char * Get_CfgFileName(void)
{
	GetCtrlVal(mainPanel, MAIN_PANEL_DUT_TYPE, &dut_type);
    
    if(dut_type == DUT_TYPE_AIO_FCT)
        return(CFG_FILE_AIO_FCT);

    else if(dut_type == DUT_TYPE_AIO_FT)
        return(CFG_FILE_AIO_FT);

    else if(dut_type == DUT_TYPE_HYBRID)
        return(CFG_FILE_HYBRID);

    else
        return(CFG_FILE_KEYPAD);

}

void DisplayType(void)
{
    char type_str[4][15] = 
    {
        "AIO FCT",
        "AIO FT",
        "Hybrid FCT",
        "Keypad FT"
    }
    ;
    
    int type_index;
    char type_info[30] = {0};

    GetCtrlVal(mainPanel, MAIN_PANEL_DUT_TYPE, &type_index);
    sprintf(type_info, "Device Type:%s\r\n", type_str[type_index]);
    SetCtrlVal(mainPanel, MAIN_PANEL_INFO_TEXTBOX, type_info);

}

//Config RLY boards mode    
void RLY_mode_Init(void)
{
    RLY_SetCommonMode(1);   //Config RLY board 1 to Common mode 
    RLY_SetCommonMode(2);   //Config RLY board 2 to Common mode 
	RLY_SetCommonMode(3);   //Config RLY board 3 to Common mode   
	RLY_SetCommonMode(4);   //Config RLY board 4 to Common mode    
}

void Fixture_Init(void)
{
	RLY_mode_Init();
	RLY_Clear(4);   
}

/******************************************************************************
    Routine Name    : CFG_FILE_Proc
    Form            : void CFG_FILE_Proc(void)
    Parameters      : none
    Return value    : none
    Description     : Read the config file.
******************************************************************************/

void CFG_FILE_Proc(void)
{
	U8 line[CH_PERLINE_MAX];
    int read_sum = 0;

	int fb;
    U8 len;
    U8 test_result;
    static U8 * ptr;
    static int cnt;

//    SetCtrlAttribute (mainPanel, MAIN_PANEL_RESULT, ATTR_TEXT_COLOR, VAL_DK_GREEN);
//    SetCtrlVal(mainPanel, MAIN_PANEL_INFO_TEXTBOX, "");
	Fixture_Init();


#ifdef UPDATE_CFGFILE

	fb = OpenFile(Get_CfgFileName(), VAL_READ_WRITE, VAL_TRUNCATE, VAL_BINARY);

	if(fb != -1)
	{
	    WriteFile(fb, TestItemArray, strlen((char * )TestItemArray));
		CloseFile(fb);
    }
    
#endif

	fb = OpenFile(Get_CfgFileName(), VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);

	if(fb == -1)
	{
	    MessagePopup("Failed", "Can't open the file!");
        return;
	}
	else
	{
        memset(TestItemArray, 0, (CH_PERCFG_MAX-1));  // Clear the CFG file.
		read_sum = ReadFile(fb, TestItemArray, (CH_PERCFG_MAX-1));
		CloseFile(fb);
	}
	
    DeleteTextBoxLines(mainPanel, MAIN_PANEL_INFO_TEXTBOX, 0, -1);
    DeleteTextBoxLines(mainPanel, MAIN_PANEL_TEST_ITEM_TEXTBOX, 0, -1);
    DeleteTextBoxLines(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, 0, -1);

    DisplayType();

    ptr = TestItemArray;
    cnt = 0;

	while(1)
	{
        len = Get_A_Line(ptr, line);
        cnt += len;
        
        if(read_sum < (cnt + strlen(",,,,,,,,,\r")))
        {
            test_result = PASS;
            break;
        }
        
	    ptr += len;

	    //line",,,,,,,,," and line "#" wil be do not care.
    	if(strlen((char * )line) > strlen(",,,,,,,,,\r") && (* line) != '#')
    		if(ProcLine(line) == FALSE)	//Process this line.
    		{
                SetCtrlAttribute (mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, ATTR_TEXT_COLOR, VAL_DK_RED);
                test_result = FAIL;
//    			break;  // zhujianmin
    		}
    }
    
//    PowerOff_All();
    LOGFILE_Write();
    
    SetPanelAttribute(mainPanel, ATTR_DIMMED, TRUE);
    DisplayPanel (resultPanel);
    SetActivePanel(resultPanel);
    
    if(test_result == PASS)    //testing pass.
    {
        SetCtrlAttribute(resultPanel,PAFA_PANEL_PASS_PICTURE,ATTR_VISIBLE,TRUE);
        SetCtrlAttribute(resultPanel,PAFA_PANEL_FAIL_PICTURE,ATTR_VISIBLE,FALSE);
        SetCtrlAttribute (resultPanel, PAFA_PANEL_TEXTMSG, ATTR_TEXT_COLOR, VAL_DK_GREEN);
        SetCtrlVal(resultPanel, PAFA_PANEL_TEXTMSG, "PASS");
    }
    else
    {
        SetCtrlAttribute(resultPanel,PAFA_PANEL_FAIL_PICTURE,ATTR_VISIBLE,TRUE);
        SetCtrlAttribute(resultPanel,PAFA_PANEL_PASS_PICTURE,ATTR_VISIBLE,FALSE);
        SetCtrlAttribute (resultPanel, PAFA_PANEL_TEXTMSG, ATTR_TEXT_COLOR, VAL_DK_RED);
        SetCtrlVal(resultPanel, PAFA_PANEL_TEXTMSG, "FAIL");
    }
}



