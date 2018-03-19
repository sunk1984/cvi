/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2017. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAIN_PANEL                       1
#define  MAIN_PANEL_DUT_TYPE              2       /* control type: ring, callback function: (none) */
#define  MAIN_PANEL_START_BUTTON          3       /* control type: command, callback function: StartTest */
#define  MAIN_PANEL_QUITBUTTON            4       /* control type: command, callback function: QuitCallback */
#define  MAIN_PANEL_TEST_ITEM_TEXTBOX     5       /* control type: textBox, callback function: (none) */
#define  MAIN_PANEL_INFO_TEXTBOX          6       /* control type: textBox, callback function: (none) */
#define  MAIN_PANEL_SUB_ITEM_TEXTBOX      7       /* control type: textBox, callback function: (none) */
#define  MAIN_PANEL_SETUP_BUTTON          8       /* control type: command, callback function: SetupPort */
#define  MAIN_PANEL_SPLITTER_5            9       /* control type: splitter, callback function: (none) */
#define  MAIN_PANEL_SPLITTER              10      /* control type: splitter, callback function: (none) */
#define  MAIN_PANEL_SPLITTER_6            11      /* control type: splitter, callback function: (none) */
#define  MAIN_PANEL_SPLITTER_4            12      /* control type: splitter, callback function: (none) */

#define  PAFA_PANEL                       2
#define  PAFA_PANEL_PASSFAIL_BUTTON       2       /* control type: command, callback function: Return_FromPassFail */
#define  PAFA_PANEL_TEXTMSG               3       /* control type: textMsg, callback function: (none) */
#define  PAFA_PANEL_PASS_PICTURE          4       /* control type: picture, callback function: (none) */
#define  PAFA_PANEL_FAIL_PICTURE          5       /* control type: picture, callback function: (none) */

#define  SET_PANEL                        3
#define  SET_PANEL_PORT_OK_BUTTON         2       /* control type: command, callback function: SetupPortOk */
#define  SET_PANEL_FIXTURE_PORT_INP       3       /* control type: numeric, callback function: (none) */
#define  SET_PANEL_PRINTER_PORT_INP       4       /* control type: numeric, callback function: (none) */
#define  SET_PANEL_AUX_PORT_INP           5       /* control type: numeric, callback function: (none) */
#define  SET_PANEL_DUT_PORT_INP           6       /* control type: numeric, callback function: (none) */
#define  SET_PANEL_PC_PORT_LIST           7       /* control type: textBox, callback function: (none) */
#define  SET_PANEL_SPLITTER               8       /* control type: splitter, callback function: (none) */
#define  SET_PANEL_SET_CANEL_BUTTON       9       /* control type: command, callback function: ExitSetting */
#define  SET_PANEL_REFRESH_PORT_BUTTON    10      /* control type: command, callback function: RefreshPorts */
#define  SET_PANEL_TEXTMSG                11      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ExitSetting(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RefreshPorts(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Return_FromPassFail(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetupPort(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetupPortOk(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
