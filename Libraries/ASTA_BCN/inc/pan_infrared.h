#ifndef _PAN_INFRARED_H_
#define _PAN_INFRARED_H_

#include "common.h"

#define RED1    A27
#define RED2    A26
#define RED3    A28
#define RED4    A29
#define RED5    B0
#define RED6    B1
#define RED7    B2
#define RED8    B3
#define RED9    B4
#define RED10   B5
#define RED11   B6
#define RED12   B7

void INFRARED_Init();
uint8 INFRARED_Get();
#endif