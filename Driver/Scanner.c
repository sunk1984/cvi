

#include "includes.h"

int Scan_Serial(char * barcode)
{
    int sel_button;
    char bar_temp[21] = {0};
    char info_serial[40] = {0};
    
    sel_button = GenericMessagePopup("scan serial number", "Please scan the serial number...",
        "OK", "CANCEL", 0, bar_temp, 20, 0, VAL_GENERIC_POPUP_INPUT_STRING, 
        VAL_GENERIC_POPUP_BTN1, VAL_GENERIC_POPUP_BTN2);

//    printf("sel_button=%d\r\n", sel_button);

    if(sel_button == 1) // OK
    {
        strcpy(barcode, bar_temp);
        sprintf(info_serial, "serial number: %s\r\n", barcode);
        SetCtrlVal(mainPanel, MAIN_PANEL_INFO_TEXTBOX, info_serial);
        LOGFILE_AddItem(info_serial);
        return TRUE;
    }
    else
        return FALSE;
}

int Scan_MAC(char * barcode)
{
    int sel_button;
    char bar_temp[21] = {0};
    char info_mac[40] = {0};
    
    sel_button = GenericMessagePopup("scan mac address", "Please scan the MAC address...",
        "OK", "CANCEL", 0, bar_temp, 20, 0, VAL_GENERIC_POPUP_INPUT_STRING, 
        VAL_GENERIC_POPUP_BTN1, VAL_GENERIC_POPUP_BTN2);

//    printf("sel_button=%d\r\n", sel_button);

    if(sel_button == 1) // OK
    {
        strcpy(barcode, bar_temp);
        sprintf(info_mac, "MAC address: %s\r\n", barcode);
        SetCtrlVal(mainPanel, MAIN_PANEL_INFO_TEXTBOX, info_mac);
        LOGFILE_AddItem(info_mac);
        return TRUE;
    }
    else
        return FALSE;
}

void scanner(void)
{
    char barcode[21];
    
    Scan_Serial(barcode);
    Scan_MAC(barcode);
}


