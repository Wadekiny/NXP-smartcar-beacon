#include "openmv.h"
#include "ALLBCN.h"

void OPENMV_Init()
{
    uart_init(uart1,115200);
    uart_init(uart0,115200);
    uart_rx_irq_en(uart1);
    uart_rx_irq_en(uart0);
}

uint8 openmv_buffer[10];//信标x2,y2,障碍x2,y2  8位
uint8 openmv_data_cnt=0;
uint8 openmv_head_cnt=0;
uint8 openmv_tail_cnt=0;
uint16 mv_xx=0,mv_yy=0,mv_width=0;
uint16 barrier_x=0,barrier_y=0;

uint8 openmv_buffer2[10];//信标x2,y2,障碍x2,y2  8位
uint8 openmv_data_cnt2=0;
uint8 openmv_head_cnt2=0;
uint8 openmv_tail_cnt2=0;
uint16 mv_xx2=0,mv_yy2=0,mv_width2=0;
uint16 barrier_x2=0,barrier_y2=0;

#define OPENMV_HEAD1    0xEE
#define OPENMV_HEAD2    0xA2
#define OPENMV_TAIL1    0xFF
uint8 mv2_flag=0;
uint8 mv_flag = 0;
void OPENMV_Read()
{
    uint8 mv_temp;
    uint8 mv_temp2;
    if (mv_flag == 1)
    {
        mv_flag = 0;
        if(UART1->S1 & UART_S1_RDRF_MASK) //接收数据寄存器满
        {
            uart_getchar(uart1,&mv_temp);   //读数据
            if (openmv_head_cnt == 0)//刚开始检测，检测第一个帧头
            {
                openmv_data_cnt = openmv_tail_cnt = 0;
                if (mv_temp == OPENMV_HEAD1)//是第一个帧头
                {
                    openmv_head_cnt++;
                }
                else    //检测错误，从头开始
                {
                    openmv_head_cnt=0;
                }
            }
            else if (openmv_head_cnt == 1)//检测到第一个帧头，检测第二个
            {
                openmv_data_cnt = openmv_tail_cnt = 0;
                if (mv_temp == OPENMV_HEAD2)//是第二个帧头
                {
                    openmv_head_cnt++;
                }
                else if (mv_temp == OPENMV_HEAD1) //又是第一个帧头，
                {
                    openmv_head_cnt=1;//检测到第一个帧头，head——cnt不变
                }
                else    //检测错误，从头开始
                {
                    openmv_head_cnt=0;
                }
            }
            else if (openmv_head_cnt == 2)//两个帧头检测到，检测数据。
            {
                if (openmv_data_cnt != 10)//没检测完十个数据
                {
                    openmv_buffer[openmv_data_cnt] = mv_temp;
                    openmv_data_cnt++;
                }
                else//检测完十个数据，检测帧尾
                {
                    if (mv_temp == OPENMV_TAIL1)//检测成功，坐标更新。
                    {                  
                        mv_xx = openmv_buffer[1];       //合成
                        mv_xx <<= 8;
                        mv_xx |= openmv_buffer[0];  
                        
                        mv_yy = openmv_buffer[3];  
                        mv_yy <<= 8;
                        mv_yy |= openmv_buffer[2];  
                        
                        barrier_x = openmv_buffer[5];
                        barrier_x <<= 8;
                        barrier_x |= openmv_buffer[4];  
                        
                        barrier_y = openmv_buffer[7];
                        barrier_y <<= 8;
                        barrier_y |= openmv_buffer[6];  
                        
                        mv_width = openmv_buffer[9];
                        mv_width <<= 8;
                        mv_width |= openmv_buffer[8];  
                        
                        openmv_head_cnt = 0;         
                    }
                    else if (mv_temp == OPENMV_HEAD1)
                    {
                        openmv_head_cnt = 1;//检测到第一个帧头
                    }
                    else
                    {
                        openmv_head_cnt = 0;//检测第一个帧头
                    }
                }
            }
        }
    }
    //openmv2
    if (mv2_flag == 1)
    {
        mv2_flag = 0;
        if(UART0->S1 & UART_S1_RDRF_MASK) //接收数据寄存器满
        {
            uart_getchar(uart0,&mv_temp2);   //读数据
            if (openmv_head_cnt2 == 0)//刚开始检测，检测第一个帧头
            {
                openmv_data_cnt2 = openmv_tail_cnt2 = 0;
                if (mv_temp2 == OPENMV_HEAD1)//是第一个帧头
                {
                    openmv_head_cnt2++;
                }
                else    //检测错误，从头开始
                {
                    openmv_head_cnt2=0;
                }
            }
            else if (openmv_head_cnt2 == 1)//检测到第一个帧头，检测第二个
            {
                openmv_data_cnt2 = openmv_tail_cnt2 = 0;
                if (mv_temp2 == OPENMV_HEAD2)//是第二个帧头
                {
                    openmv_head_cnt2++;
                }
                else if (mv_temp2 == OPENMV_HEAD1) //又是第一个帧头，
                {
                    openmv_head_cnt2=1;//检测到第一个帧头，head——cnt不变
                }
                else    //检测错误，从头开始
                {
                    openmv_head_cnt2=0;
                }
            }
            else if (openmv_head_cnt2 == 2)//两个帧头检测到，检测数据。
            {
                if (openmv_data_cnt2 != 10)//没检测完十个数据
                {
                    openmv_buffer2[openmv_data_cnt2] = mv_temp2;
                    openmv_data_cnt2++;
                }
                else//检测完十个数据，检测帧尾
                {
                    if (mv_temp2 == OPENMV_TAIL1)//检测成功，坐标更新。
                    {                  
                        mv_xx2 = openmv_buffer2[1];       //合成
                        mv_xx2 <<= 8;
                        mv_xx2 |= openmv_buffer2[0];  
                        
                        mv_yy2 = openmv_buffer2[3];  
                        mv_yy2 <<= 8;
                        mv_yy2 |= openmv_buffer2[2];  
                        
                        barrier_x2 = openmv_buffer2[5];
                        barrier_x2 <<= 8;
                        barrier_x2 |= openmv_buffer2[4];  
                        
                        barrier_y2 = openmv_buffer2[7];
                        barrier_y2 <<= 8;
                        barrier_y2 |= openmv_buffer2[6];  
                        
                        mv_width2 = openmv_buffer2[9];
                        mv_width2 <<= 8;
                        mv_width2 |= openmv_buffer2[8];  
                        
                        openmv_head_cnt2 = 0;         
                    }
                    else if (mv_temp2 == OPENMV_HEAD1)
                    {
                        openmv_head_cnt2 = 1;//检测到第一个帧头
                    }
                    else
                    {
                        openmv_head_cnt2 = 0;//检测第一个帧头
                    }
                }
            }
        }
    }
}
uint16 last_bcn_x=0,last_bcn_y=0;
uint16 last_bcn_x2=0,last_bcn_y2=0;
void OPENMV_BcnCoord()
{
    static uint8 mv_no_index=0;
    static uint8 mv_no_index2=0;
   
    last_bcn_x=bcn_x;
    last_bcn_y=bcn_y;
    last_bcn_x2=bcn_x2;
    last_bcn_y2=bcn_y2;
    if(mv_xx==0&&mv_yy==0)
    {
        mv_no_index++;
        if (mv_no_index >8)
        {
            mv_no_index=8;
            bcn_x = 0;
            bcn_y = 0;
            bcn_width=0;
        }
    }
    else
    {
        mv_no_index=0;
        bcn_x = mv_xx;
        bcn_y = mv_yy;
        bcn_width=mv_width;
    }
    ///////////////////////bcn2/////////////////////////
    if(mv_xx2==0&&mv_yy2==0)
    {
        mv_no_index2++;
        if (mv_no_index2 >8)
        {
            mv_no_index2=8;
            bcn_x2 = 0;
            bcn_y2 = 0;
            bcn_width2=0;
        }
    }
    else
    {
        mv_no_index2=0;
        bcn_x2 = mv_xx2;
        bcn_y2 = mv_yy2;
        bcn_width2=mv_width2;
    }
    if (must_dir == 0)//对抗
    {
        if (car_dir == 0)
        {
            if (bcn_x2 >315 || bcn_x2 < 5)
            {
                bcn_x2=0;
                bcn_y2=0;
                car_dir=0;
            }
            else if (bcn_x2!=0 || bcn_y2 !=0)
            {
                car_dir = 2;
            }
        }
        if (car_dir == 2)
        {
            if (bcn_x >315 || bcn_x < 5)
            {
                bcn_x=0;
                bcn_y=0;
                car_dir=2;
            }
            else if (bcn_x!=0 || bcn_y !=0)
            {
                car_dir = 0;
            }
        }
    }
    else if(must_dir == 1)
    {
        car_dir = 0;
        bcn_x2=0;
        bcn_y2=0;
    }
    else if (must_dir == 2)
    {
        car_dir = 2;
        bcn_x=0;
        bcn_y=0;
    }
    
    if (car_dir == 0)//确定最新坐标为前摄像头
    {
        if(pit_back_dir==1)//上一次读取速度值参考系为!!后!!摄像头
        {
            //速度切换
            act_speed1 = -act_speed3;
            act_speed2 = -act_speed4;
            act_speed3 = -act_speed1;
            act_speed4 = -act_speed2;
        }
    }
    if (car_dir == 2)//确定最新坐标为前摄像头
    {
        if(pit_front_dir==1)//上一次读取速度值参考系为!!前!!摄像头
        {
            //速度切换
            act_speed1 = -act_speed3;
            act_speed2 = -act_speed4;
            act_speed3 = -act_speed1;
            act_speed4 = -act_speed2;
        }
    }
}
