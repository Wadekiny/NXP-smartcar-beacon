#include "beep.h"
#include "headfile.h"

uint8 beep_en=0;
void BEEP_Init()
{
    gpio_init(BEEP,GPO,0);
    //port_init_NoAlt(BEEP,PULLUP);
}

void BEEP_ON()
{
    gpio_set(BEEP,1*beep_en);
}

void BEEP_OFF()
{
    gpio_set(BEEP,0);
}