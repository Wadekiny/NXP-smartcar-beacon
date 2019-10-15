#ifndef _MASTER_H_
#define _MASTER_H_
//master e1 e0
//slave  d2 d3


#define MASTER_UART  uart1

void MASTER_UartInit();
void MASTER_SendCmd();
void MASTER_GetMessage();
#endif