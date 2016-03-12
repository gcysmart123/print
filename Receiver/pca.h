#ifndef __PCA_H_
#define __PCA_H_

#include "STC15F2K60S2.H"
#include "global.h"
#include "tpyedef.h"



#define FOSC    12000000L
#define T100Hz  (FOSC / 12 / 1923)

#define T20Hz  (FOSC / 12 / 20)

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


extern WORD value;
extern WORD value_send_delay;
extern WORD value_waite;

extern void pca_timer_init();
extern void SendIRdata(unsigned char id_data_3, unsigned char id_data_2, unsigned char id_data_1);

#endif
