#include    "isr.h"
#include    "ALLBCN.h"
#include    "headfile.h"

//配置摄像头MT9v032   uart3
                
void UART0_RX_TX_IRQHandler(void)   //openmv2
{
    mv2_flag =1;
    OPENMV_Read();
    protect_cnt2=0;
    //mt9v032_cof_uart_interrupt2(); 
} 
void UART1_RX_TX_IRQHandler(void)   //openmv
{
    mv_flag =1;
    OPENMV_Read();
    protect_cnt1=0;
}

//void UART4_RX_TX_IRQHandler(void)   //蓝牙
//{
//    mt9v032_cof_uart_interrupt(); 
//}    


//速度控制 pit0
uint8 pit_front_dir=0;
uint8 pit_back_dir=0;
void PIT0_IRQHandler()
{
    if (car_dir == FRONT_DIR)//前摄像头
    {
        pit_front_dir=1;
        pit_back_dir=0;
        ENCODE_Read(&act_speed1,&act_speed2,&act_speed3,&act_speed4);
    }
    else//后摄像头
    {
        pit_front_dir=0;
        pit_back_dir=1;
        ENCODE_Read(&act_speed3,&act_speed4,&act_speed1,&act_speed2);
        act_speed1 = -act_speed1;
        act_speed2 = -act_speed2;
        act_speed3 = -act_speed3;
        act_speed4 = -act_speed4;
    }
    //前面得到的分速度一定正确，进行速度转换
    ENCODE_SpeedCm(&act_speed1,&act_speed2,&act_speed3,&act_speed4);
    act_speed_l = (act_speed2+act_speed3)/2;
    act_speed_r = (act_speed1+act_speed4)/2;
    act_speed_y = (act_speed1 + act_speed2 + act_speed3 + act_speed4)*1.0/4;
    filter_speed = act_speed_y;
    act_speed_y = ALGO_KalmanSpeed(act_speed_y,70*1.0/1000,400*1.0/1000);
    minus_l = act_speed3 - act_speed2;
    minus_r = act_speed4 - act_speed1;
    
    PID_MotorControl(wheel.v1,wheel.v2,wheel.v3,wheel.v4,car_dir);
    PIT_FlAG_CLR(pit0);
}

void PIT1_IRQHandler()
{
    pit_finish_flag = 1;
    PIT_FlAG_CLR(pit1);
}

void PIT3_IRQHandler()//100ms
{
    if (is_time_start == 1)//发车
    {
        finish_s += 0.01;
    }
    PIT_FlAG_CLR(pit3);
}
        
/*
//摄像头MT9v032场中断
void PORTD_IRQHandler(void)     //MT9v032
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTD->ISFR);
    flag = PORTD->ISFR;
    PORTD->ISFR  = ~0;          //清中断标志位

    n = 15;                       //场中断
    if(flag & (1 << n))         //PTC18触发中断
    {
        //camera_vsync();
      VSYNC();
    }
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	//PORTC_ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTC_FLAG_CLR(C1);
    //VSYNC();
}

//MT9v032 DMA
void DMA0_IRQHandler(void)      
{
	DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();
    //IMAGE_GetBinary(image,image_binary);    //二值化
}


void PORTA_IRQHandler(void)     //MT9v032 2
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA->ISFR);
    flag = PORTA->ISFR;
    PORTA->ISFR  = ~0;          //清中断标志位

    n = 29;                       //场中断
    if(flag & (1 << n))         //PTC18触发中断
    {
        //camera_vsync();
      VSYNC2();
    }
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	//PORTC_ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTC_FLAG_CLR(C1);
    //VSYNC();
}

//MT9v032 DMA
void DMA2_IRQHandler(void)      
{
    DMA_DIS(DMA_CH2);                                     		//禁用DMA通道
    DMA_IRQ_CLEAN(DMA_CH2);                               		//清除通道传输中断标志位
    DMA_IRQ_EN(DMA_CH2);                                  		//允许DMA通道中断
	DMA_EN(DMA_CH2);  	
	//DMA_IRQ_CLEAN(DMA_CH2);
    row_finished2();
    //IMAGE_GetBinary(image,image_binary);    //二值化
}
*/


/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了DMA0中断，然后就到下面去找哪个是DMA0的中断函数名称，找到后写一个该名称的函数即可
void DMA0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位


DMA0_IRQHandler  
DMA1_IRQHandler  
DMA2_IRQHandler  
DMA3_IRQHandler  
DMA4_IRQHandler  
DMA5_IRQHandler  
DMA6_IRQHandler  
DMA7_IRQHandler  
DMA8_IRQHandler  
DMA9_IRQHandler  
DMA10_IRQHandler 
DMA11_IRQHandler 
DMA12_IRQHandler 
DMA13_IRQHandler 
DMA14_IRQHandler 
DMA15_IRQHandler 
DMA_Error_IRQHandler      
MCM_IRQHandler            
FTFL_IRQHandler           
Read_Collision_IRQHandler 
LVD_LVW_IRQHandler        
LLW_IRQHandler            
Watchdog_IRQHandler       
RNG_IRQHandler            
I2C0_IRQHandler           
I2C1_IRQHandler           
SPI0_IRQHandler           
SPI1_IRQHandler           
SPI2_IRQHandler           
CAN0_ORed_Message_buffer_IRQHandler    
CAN0_Bus_Off_IRQHandler                
CAN0_Error_IRQHandler                  
CAN0_Tx_Warning_IRQHandler             
CAN0_Rx_Warning_IRQHandler             
CAN0_Wake_Up_IRQHandler                
I2S0_Tx_IRQHandler                     
I2S0_Rx_IRQHandler                     
CAN1_ORed_Message_buffer_IRQHandler    
CAN1_Bus_Off_IRQHandler                
CAN1_Error_IRQHandler                  
CAN1_Tx_Warning_IRQHandler             
CAN1_Rx_Warning_IRQHandler             
CAN1_Wake_Up_IRQHandler                
Reserved59_IRQHandler                  
UART0_LON_IRQHandler                   
UART0_RX_TX_IRQHandler                 
UART0_ERR_IRQHandler                   
UART1_RX_TX_IRQHandler                 
UART1_ERR_IRQHandler  
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler  
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler  
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler  
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler  
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler 
RTC_IRQHandler 
RTC_Seconds_IRQHandler  
PIT0_IRQHandler  
PIT1_IRQHandler  
PIT2_IRQHandler  
PIT3_IRQHandler  
PDB0_IRQHandler  
USB0_IRQHandler  
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler  
ENET_Receive_IRQHandler
ENET_Error_IRQHandler  
Reserved95_IRQHandler  
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler 
LPTimer_IRQHandler 
Reserved102_IRQHandler 
PORTA_IRQHandler 
PORTB_IRQHandler 
PORTC_IRQHandler 
PORTD_IRQHandler 
PORTE_IRQHandler 
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler 
*/