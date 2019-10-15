#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "common.h"

#define BLT_UART    uart4

void BLT_UartInit();
void BLT_SendData16(int16 pa1, int16 pa2, int16 pa3, int16 pa4,
                int16 pa5, int16 pa6, int16 pa7, int16 pa8);
#endif