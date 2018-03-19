
/*******************************************************************************
    Audio.c
    Communication with Merak Audio board throgh I2C interface

    Copyright(C) 2012, Honeywell Integrated Technology (China) Co.,Ltd.
    Security FCT team
    All rights reserved.

    IDE:    IAR EWARM V6.4
    ICE:    J-Link
    BOARD:  Merak Main board

*******************************************************************************/

#include "includes.h"

#define AUDNUM_ONLY_1 	    1
#define TEST_AUDIO_1KHZ     1000
#define TEST_AUDIO_300MV    300

#define AUDFUNC_ENTER_MODE  0x51 
#define AUDFUNC_AUD_GEN     0x52 
#define AUDFUNC_READ_SIGNAL 0x54

#define AUDREG_ENTER_MODE   0
#define AUDREG_CFG_DTMF     1
#define AUDREG_CFG_TONE     2
#define AUDREG_DECODE_SIGN  0

#define AUDREG_GEN_DTMF     1
#define AUDREG_GEN_TONE     3
#define AUDREG_GEN_IDLE     4


const U8 ModeDTMF[] = "01";
const U8 ModeSimp[] = "04";

const U8 GenIdle[] = "00";

static BOOL Audio_WriteCmd(U8 func, U8 reg, U8 * write_str)
{
    return(MERAK_WriteCmd("AUD", AUDNUM_ONLY_1, func, reg, write_str));
}

static BOOL Audio_ReadCmd(U8 func, U8 reg, U8 * read_str)
{
    return(MERAK_ReadCmd("AUD", AUDNUM_ONLY_1, func, reg, read_str));
}

static BOOL Audio_SetMode(U8 * mode_str)
{
    return(Audio_WriteCmd(AUDFUNC_ENTER_MODE, AUDREG_ENTER_MODE, mode_str));
}

/******************************************************************************
    Routine Name    : Audio_TestSimpTone
    Form            : static BOOL Audio_TestSimpTone(U16 exp_freq, U8 freq_tol, U16 amp_lower, U16 amp_upper)
    Parameters      : exp_freq, amp_lower, amp_upper
    Return value    : PASS/FAIL
    Description     : Test the Audio signal, Compare signal value with limit, Get the PASS/FAIL conclusion.
******************************************************************************/
BOOL Audio_TestSimpTone(U16 exp_freq, U16 freq_tol, U16 amp_lower, U16 amp_upper)
{
    volatile U8 i;
    U16 rx_freq;
    U16 rx_amp;

    delay_ms(200);

    for(i=0; i<3; i++)
    {
    	delay_ms(300);
    	
        rx_freq = 0;
        rx_amp = 0;

        Audio_DecSimpTone(&rx_freq, &rx_amp);
        printf("Audio%dHz,%dmv\r\n", rx_freq, rx_amp);
        
        if(rx_amp < amp_upper && rx_amp > amp_lower)
        {
            if(rx_freq <= (exp_freq+freq_tol) && rx_freq >= (exp_freq-freq_tol) )
                break;
        }
    }

    if(i < 3)
    	return PASS;
    else
    	return FAIL;
}

/******************************************************************************
    Routine Name    : Audio_TestBuzz
    Form            : BOOL Audio_TestBuzz(U16 tx_amp, U16 amp_lower, U16 amp_upper)
    Parameters      : buzz_freq, amp_lower, amp_upper
    Return value    : PASS/FAIL
    Description     : Testing procedure for Buzzer. 
******************************************************************************/
BOOL Audio_TestBuzz(U16 buzz_freq, U16 amp_lower, U16 amp_upper)
{
//    Audio_SetMode((U8 * )ModeSimp);
    return(Audio_TestSimpTone(buzz_freq, BUZZ_FREQ_TOLERANCE, amp_lower, amp_upper));
}

/******************************************************************************
    Routine Name    : Audio_LoopTest
    Form            : BOOL Audio_LoopTest(U16 tx_amp, U16 amp_lower, U16 amp_upper)
    Parameters      : tx_amp, amp_lower, amp_upper
    Return value    : PASS/FAIL
    Description     : Testing procedure for Audio. 
******************************************************************************/
BOOL Audio_LoopTest(U16 tx_amp, U16 amp_lower, U16 amp_upper)
{
    BOOL ret;

    Audio_SetMode((U8 * )ModeSimp);
    delay_ms(100);
    Audio_GenSimpTone(TEST_AUDIO_1KHZ, tx_amp);
    printf("Test%dHz, %dmv\r\n", TEST_AUDIO_1KHZ, tx_amp);
    ret = Audio_TestSimpTone(TEST_AUDIO_1KHZ, AUDIO_FREQ_TOLERANCE, amp_lower, amp_upper);
    delay_ms(100);
    Audio_StopSignal();
    delay_ms(100);
   
    return(ret);
}

/******************************************************************************
    Routine Name    : Audio_GenSimpTone
    Form            : BOOL Audio_GenerateSinTone(U16 freq, U16 amp)
    Parameters      : freq, amp
    Return value    : TRUE/FALSE
    Description     : Generate the Audio Tone signal .
******************************************************************************/
BOOL Audio_GenSimpTone(U16 freq, U16 amp)
{
    U8 tone_str[9];
    
    sprintf((char * )tone_str, "%d,%d", freq, amp);

    return(Audio_WriteCmd(AUDFUNC_AUD_GEN, AUDREG_GEN_TONE, tone_str));
}

/******************************************************************************
    Routine Name    : Audio_StopSignal
    Form            : void Audio_StopSignal()
    Parameters      : void
    Return value    : TRUE/FALSE
    Description     : End the signal transmitting .
******************************************************************************/
BOOL Audio_StopSignal(void)
{
    return(Audio_WriteCmd(AUDFUNC_AUD_GEN, AUDREG_GEN_IDLE, (U8 * )GenIdle));
}

/******************************************************************************
    Routine Name    : Audio_DecSimpTone
    Form            : U8 Audio_DecSimpTone(U16 * freq, U16 * amp)
    Parameters      : freq, amp
    Return value    : TRUE/FALSE
    Description     : Decode the Audio signal, Get the value of the audio signal.
******************************************************************************/
BOOL Audio_DecSimpTone(U16 * freq, U16 * amp)
{
    U8 i;
    U8 DecStr[10];
    
    if(Audio_ReadCmd(AUDFUNC_READ_SIGNAL, AUDREG_DECODE_SIGN, DecStr))
    {
        for(i = 0; i < 10; i++)
        {
            if(DecStr[i] == ',')
                break;
        }
        if(i < 10)
        {
            * freq = (U16)strtod((char * )DecStr, NULL);
            * amp = (U16)strtod((char * )(DecStr+i+1), NULL);
            return(TRUE);
        }
    }
    return(FALSE);
}

BOOL Audio_GenDTMF(U8 * DTMF_numb)
{
    return(Audio_WriteCmd(AUDFUNC_AUD_GEN, AUDREG_GEN_DTMF, DTMF_numb));
}

/******************************************************************************
    Routine Name    : Audio_EnterDTMF
    Form            : BOOL Audio_EnterDTMF(void)
    Parameters      : void
    Return value    : PASS/FAIL
    Description     : Enter DTMF Decode mode. 
******************************************************************************/
BOOL Audio_EnterDTMF(void)
{
    return(Audio_SetMode((U8 * )ModeDTMF));
}

/******************************************************************************
    Routine Name    : Audio_DecDTMF
    Form            : BOOL Audio_DecDTMF(U8 * number)
    Parameters      : number (ASCII string)
    Return value    : PASS/FAIL
    Description     : Get DTMF number from audio board, The number that received within last 4 seconds and being not  read. 
******************************************************************************/
BOOL Audio_DecDTMF(U8 * number)
{
    U8 DecStr[17] = {0};
    
    if(Audio_ReadCmd(AUDFUNC_READ_SIGNAL, AUDREG_DECODE_SIGN, DecStr))
    {
        strcpy((char * )number, (char * )DecStr);
        return(TRUE);
    }
    return(FALSE);
}


