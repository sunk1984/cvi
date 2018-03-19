#include <userint.h>
#include "grip.h"

#include "includes.h"

char result_str[2][10] =
{
    "FAIL!!!\r\n",
    "PASS!\r\n",
};

void PoolingAllPorts(void)
{
    volatile int i;
    int status;
    char com_list_str[8];
    
    U8 SerialNum;
    U8 PoolingSerialList[20];

    
    SerialNum = 0;
    memset(PoolingSerialList, 0, 20);

    for(i = 1; i < 20; i++) 
    {
		status = OpenCom(i, "");  // zjm
		if(status == 0)    // Opened the COM port !!
		{
            PoolingSerialList[SerialNum] = i;
		    CloseCom(i);

            sprintf(com_list_str, "COM%d\r\n", PoolingSerialList[SerialNum]);
            SetCtrlVal(setupPanel, SET_PANEL_PC_PORT_LIST, com_list_str);
            
            SerialNum++;
		}
        delay_ms(2);
    } 
}

void LoadAllPorts(void)
{
    SetCtrlVal(setupPanel, SET_PANEL_FIXTURE_PORT_INP, portNo[0]);
    SetCtrlVal(setupPanel, SET_PANEL_DUT_PORT_INP, portNo[1]);
    SetCtrlVal(setupPanel, SET_PANEL_AUX_PORT_INP, portNo[2]);
    SetCtrlVal(setupPanel, SET_PANEL_PRINTER_PORT_INP, portNo[3]);
}

int CVICALLBACK ExitSetting (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            
            HidePanel(setupPanel);   
            SetPanelAttribute(mainPanel, ATTR_DIMMED, FALSE);
            SetActivePanel(mainPanel);

			break;
	}
	return 0;
}

int CVICALLBACK RefreshPorts (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            
            DeleteTextBoxLines(setupPanel, SET_PANEL_PC_PORT_LIST, 0, -1);
            PoolingAllPorts();

			break;
	}
	return 0;
}
