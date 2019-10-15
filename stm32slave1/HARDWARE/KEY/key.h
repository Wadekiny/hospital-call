#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0  PEin(2)


#define NONE	0
#define KEY0P 	1

void KEY_Init(void);
unsigned char KEY_Scan(void);
#endif
