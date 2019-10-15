#ifndef _CRASH_H_
#define _CRASH_H_

#include "common.h"
uint8 CRASH_Check();
void CRASH_Solve(uint8 crash_mode);
void CRASH_KeyInit();
#define LEFTKEY0    C16
#define RIGHTKEY0   C17
#define LEFTKEY2    D13
#define RIGHTKEY2   D14
#endif