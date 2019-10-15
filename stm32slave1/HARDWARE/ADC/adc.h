#ifndef __ADC_H
#define __ADC_H

#include "sys.h"
#include "delay.h"

void AdcInit(void);
u16 GetADC(u8 ch);
u16 GetADCAve(u8 ch, u8 times);

#endif
