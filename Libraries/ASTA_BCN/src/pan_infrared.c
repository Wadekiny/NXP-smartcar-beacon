#include "pan_infrared.h"
#include "ALLBCN.h"

uint8 red_buffer[10]={0};
void INFRARED_Init()
{
    gpio_init(RED1,GPI,0);
    gpio_init(RED2,GPI,0);
    gpio_init(RED3,GPI,0);
    gpio_init(RED4,GPI,0);
    gpio_init(RED5,GPI,0);
    gpio_init(RED6,GPI,0);
    gpio_init(RED7,GPI,0);
    gpio_init(RED8,GPI,0); 
    gpio_init(RED9,GPI,0);
    gpio_init(RED10,GPI,0);
    gpio_init(RED11,GPI,0);
    gpio_init(RED12,GPI,0); 
//    port_init_NoAlt(RED1, PULLUP);
//    port_init_NoAlt(RED2, PULLUP);
//    port_init_NoAlt(RED3, PULLUP);
//    port_init_NoAlt(RED4, PULLUP);
//    port_init_NoAlt(RED5, PULLUP);
//    port_init_NoAlt(RED6, PULLUP);
//    port_init_NoAlt(RED7, PULLUP);
//    port_init_NoAlt(RED8, PULLUP);
//    port_init_NoAlt(RED9, PULLUP);
//    port_init_NoAlt(RED10, PULLUP);
//    port_init_NoAlt(RED11, PULLUP);
//    port_init_NoAlt(RED12, PULLUP);
}

uint8 red1,red2,red3,red4,red5,red6,red7,red8,red9,red10,red11,red12;
float red_angle;
uint8 INFRARED_Get()
{
    uint8 red_state=0;
    uint8 red_l=0;
    uint8 red_r=0;
    uint8 i;
    red1 = gpio_get(RED1);
    red2 = gpio_get(RED2);
    red3 = gpio_get(RED3);
    red4 = gpio_get(RED4);
    red5 = gpio_get(RED5);
    red6 = gpio_get(RED6);
    red7 = gpio_get(RED7);
    red8 = gpio_get(RED8);
    red9 = gpio_get(RED9);
    red10 = gpio_get(RED10);
    red11 = gpio_get(RED11);
    red12 = gpio_get(RED12);
    red_l = red1 + red2 + red3 + red4 + red5 + red6;
    red_r = red11 + red12 + red7 + red8 + red9 + red10;
    
    if (red_l < red_r) red_state = 1;
    else if (red_r < red_l) red_state = 2;
    else if (red_l ==red_r && red_r == 0) red_state = 0; //全亮
    else red_state = 3;   //非全亮 相等
    
    for (i=9; i>0; i--)
    {
        red_buffer[i] = red_buffer[i-1];
    }
    red_buffer[0] = red_state;
    
    return red_state;
}