
#ifndef _HONEYWELL_FCT_AUDIO_H_
#define _HONEYWELL_FCT_AUDIO_H_

#include "type.h"

#define AUDIO_FREQ_TOLERANCE 5
#define BUZZ_FREQ_TOLERANCE 500

extern BOOL Audio_DecSimpTone(U16 * freq, U16 * amp);
extern BOOL Audio_TestBuzz(U16 buzz_freq, U16 amp_lower, U16 amp_upper);
extern BOOL Audio_TestSimpTone(U16 exp_freq, U16 freq_tol, U16 amp_lower, U16 amp_upper);
extern BOOL Audio_LoopTest(U16 tx_amp, U16 amp_lower, U16 amp_upper);
extern BOOL Audio_GenSimpTone(U16 freq, U16 amp);
extern BOOL Audio_StopSignal(void);

extern BOOL Audio_GenDTMF(U8 * DTMF_numb);
extern BOOL Audio_EnterDTMF(void);
extern BOOL Audio_DecDTMF(U8 * number);

#endif

