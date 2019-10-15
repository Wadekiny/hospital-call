#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0  PEin(4)
#define KEY1  PEin(3)
#define KEYUP PAin(0)

#define NONE	0
#define KEY0P 	1
#define KEY1P 	2
#define KEYUPP	3

void KEY_Init(void);
unsigned char KEY_Scan(void);
#endif
