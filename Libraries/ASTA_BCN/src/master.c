#include "master.h"
#include "ALLBCN.h"
#include "headfile.h"

uint8 master_cut_dir=0;  //切灯
uint8 master_omega_dir=0;//z轴角速度方向
uint8 master_yaw_dir=0;  //航向角方向
int16 master_omega=0;    //z轴角速度
uint8 master_crash=0;    //碰撞s
void MASTER_UartInit()
{
    uart_init(MASTER_UART,115200);
    uart_rx_irq_en(MASTER_UART);
}


//第一个字节
//保留[7:6]               
//障碍[5:4]               00无障碍,01左边,10右边,11中间            
//副板推荐切灯方向[3:2]   00无推荐,01左切,10右切,11保留
//z轴角速度方向[1]        0 正（逆时针）   1 负（顺时针）
//航向角方向[0]           0 正             1 负

//第二个字节
//角速度高八位[7:0] 

//第三个字节
//角速度低八位[7:0]

#define GET_CMD 0xaa
uint8 master_get[3]={0};
uint8 master_message_flag=0;
uint8 master_get_cnt=0;
void MASTER_SendCmd()
{
    master_message_flag = 0;
    uart_putchar(MASTER_UART,GET_CMD);//主机发送信息，从机发送数据 循环最开始使用
   // while(master_message_flag==0);
}
void MASTER_GetMessage()
{
    master_crash   = (master_get[0]>>6)&0x03;
    master_barrier = (master_get[0]>>4)&0x03;//取两位
    master_cut_dir = (master_get[0]>>2)&0x03;
    master_omega_dir=(master_get[0]>>1)&0x01;//取一位
    master_yaw_dir = (master_get[0]>>0)&0x01;
    
    master_omega = 0;
    master_omega |= master_get[1];
    master_omega = master_omega<<8;
    master_omega |= master_get[2];
}