#include "bluetooth.h"
#include "headfile.h"
#include "ALLBCN.h"

void BLT_UartInit()
{
    uart_init(BLT_UART,115200);
}

//山外上位机
void BLT_SendData16(int16 pa1, int16 pa2, int16 pa3, int16 pa4,
                   int16 pa5, int16 pa6, int16 pa7, int16 pa8)
{
    uart_putchar(BLT_UART,0x03);
    uart_putchar(BLT_UART,0xfc);
    
    uart_putchar(BLT_UART,pa1&0xff);
    uart_putchar(BLT_UART,(pa1>>8)&0xff);
    
    uart_putchar(BLT_UART,pa2&0xff);
    uart_putchar(BLT_UART,(pa2>>8)&0xff);
    
    uart_putchar(BLT_UART,pa3&0xff);
    uart_putchar(BLT_UART,(pa3>>8)&0xff);
    
    uart_putchar(BLT_UART,pa4&0xff);
    uart_putchar(BLT_UART,(pa4>>8)&0xff);
    
    uart_putchar(BLT_UART,pa5&0xff);
    uart_putchar(BLT_UART,(pa5>>8)&0xff);
    
    uart_putchar(BLT_UART,pa6&0xff);
    uart_putchar(BLT_UART,(pa6>>8)&0xff);
    
    uart_putchar(BLT_UART,pa7&0xff);
    uart_putchar(BLT_UART,(pa7>>8)&0xff);
    
    uart_putchar(BLT_UART,pa8&0xff);
    uart_putchar(BLT_UART,(pa8>>8)&0xff);
    
    uart_putchar(BLT_UART,0xfc);
    uart_putchar(BLT_UART,0x03);
}
