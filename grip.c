#include <cvirte.h>		
#include <userint.h>
#include "grip.h"
#include "includes.h"

int mainPanel;
int setupPanel;
int resultPanel;
	
char info_text[200] = {0};
char item_text[4000] = {0};
char sub_item_text[4000] = {0};

int main (int argc, char *argv[])
{
	InitCVIRTE (0, argv, 0);
	
    mainPanel = LoadPanel (0, "grip.uir", MAIN_PANEL);
	setupPanel = LoadPanel(mainPanel, "grip.uir", SET_PANEL);
	resultPanel = LoadPanel(mainPanel, "grip.uir", PAFA_PANEL);

    SetCtrlAttribute(mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, ATTR_TEXT_COLOR, VAL_DK_GREEN);
    DisplayPanel (mainPanel);
    RunUserInterface ();
    
//Write_InitFile(); // zhujianmin
    
    return 0;
}

int CVICALLBACK SetupPortOk (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    int status;
    
	switch (event)
	{
		case EVENT_COMMIT:
            
            GetCtrlVal(setupPanel, SET_PANEL_FIXTURE_PORT_INP, &portNo[0]);
            GetCtrlVal(setupPanel, SET_PANEL_DUT_PORT_INP, &portNo[1]);
            GetCtrlVal(setupPanel, SET_PANEL_AUX_PORT_INP, &portNo[2]);
            GetCtrlVal(setupPanel, SET_PANEL_PRINTER_PORT_INP, &portNo[3]);

            Setting_toStr();

            Write_InitFile();
            Read_InitFile();
            status = Proc_InitStr();
			
            if(status == TRUE)
            {
                HidePanel(setupPanel);   
                SetPanelAttribute(mainPanel, ATTR_DIMMED, FALSE);
                SetActivePanel(mainPanel);
            }
			break;
	}
	return 0;
}

int CVICALLBACK StartTest (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            SetCtrlAttribute (mainPanel, MAIN_PANEL_SUB_ITEM_TEXTBOX, ATTR_TEXT_COLOR, VAL_DK_GREEN);

            Read_InitFile();
            Proc_InitStr();
            CFG_FILE_Proc();

			break;
	}
	return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    volatile int i;
    
	switch (event)
	{
		case EVENT_COMMIT:

            for(i = 0; i < 4; i++)
            {
                if(portNo[i])
                    CloseCom(portNo[i]);
            }
            
			QuitUserInterface (0);

			break;
	}
	return 0;
}

int CVICALLBACK SetupPort (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

            SetPanelAttribute(mainPanel, ATTR_DIMMED, TRUE);
            
            DeleteTextBoxLines(setupPanel, SET_PANEL_PC_PORT_LIST, 0, -1);
            DisplayPanel (setupPanel);
            SetActivePanel(setupPanel);
            
            Read_InitFile();
            Proc_InitStr();
            LoadAllPorts();
            
            PoolingAllPorts();

			break;
	}
	return 0;
}

int CVICALLBACK Return_FromPassFail (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            
            HidePanel(resultPanel);   
            SetActivePanel(mainPanel);
            SetPanelAttribute(mainPanel, ATTR_DIMMED, FALSE);

			break;
	}
	return 0;
}
