#include "mpuiic.h"
#include "headfile.h"
#include "ALLBCN.h"
#define SDA             gpio_get (MPU6050_SDA)
#define SDA0()          gpio_set (MPU6050_SDA, 0)		//IO口输出低电平
#define SDA1()          gpio_set (MPU6050_SDA, 1)		//IO口输出高电平  
#define SCL0()          gpio_set (MPU6050_SCL, 0)		//IO口输出低电平
#define SCL1()          gpio_set (MPU6050_SCL, 1)		//IO口输出高电平
#define DIR_OUT()       gpio_ddr (MPU6050_SDA, GPO)    //输出方向
#define DIR_IN()        gpio_ddr (MPU6050_SDA, GPI)    //输入方向
 
  //MPU IIC 延时函数
void MPU_IIC_Delay(void)
{
	systick_delay_us(2);
}

//初始化IIC
void MPU_IIC_Init(void)
{					     
    gpio_init (MPU6050_SCL, GPO,1);
	gpio_init (MPU6050_SDA, GPO,1);
	
	
	port_init_NoAlt (MPU6050_SCL, ODO | PULLUP);//ODO
	port_init_NoAlt (MPU6050_SDA, ODO | PULLUP);
 
}
//产生IIC起始信号
void MPU_IIC_Start(void)
{
    SDA1();
	SCL1();
	MPU_IIC_Delay();
	SDA0();
	MPU_IIC_Delay();
	SCL0();
}	  
//产生IIC停止信号
void MPU_IIC_Stop(void)
{
	SDA0();
	SCL0();
	MPU_IIC_Delay();
	SCL1();
	MPU_IIC_Delay();
	SDA1();
	MPU_IIC_Delay();	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8 MPU_IIC_Wait_Ack(void)
{
    SCL0();
	DIR_IN();
	MPU_IIC_Delay();
	
	SCL1();
    MPU_IIC_Delay();
	
    if(SDA)           //应答为高电平，异常，通信失败
    {
        DIR_OUT();
        SCL0();
        return 1;
    }
    DIR_OUT();
    SCL0();
	MPU_IIC_Delay();
    return 0;
    
//	u8 ucErrTime=0;
//	MPU_SDA_IN();      //SDA设置为输入  
//	MPU_IIC_SDA=1;MPU_IIC_Delay();	   
//	MPU_IIC_SCL=1;MPU_IIC_Delay();	 
//	while(MPU_READ_SDA)
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			MPU_IIC_Stop();
//			return 1;
//		}
//	}
//	MPU_IIC_SCL=0;//时钟输出0 	   
//	return 0;  
} 
//产生ACK应答
void MPU_IIC_Ack(void)
{
//    
//        SCL0();
//	simiic_delay();
//	if(ack_dat) SDA0();
//    else    	SDA1();
//
//    SCL1();
//    simiic_delay();
//    SCL0();
//    simiic_delay();
    SCL0();
    DIR_OUT();//MPU_IIC_Delay();
    SDA0();
    MPU_IIC_Delay();
    SCL1();
    MPU_IIC_Delay();
    SCL0();
    
//	MPU_IIC_SCL=0;
//	MPU_SDA_OUT();
//	MPU_IIC_SDA=0;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=1;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=0;
}
//不产生ACK应答		    
void MPU_IIC_NAck(void)
{
    SCL0();
    DIR_OUT();
    SDA1();
    MPU_IIC_Delay();
    SCL1();
    MPU_IIC_Delay();
    SCL0();
//	MPU_IIC_SCL=0;
//	MPU_SDA_OUT();
//	MPU_IIC_SDA=1;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=1;
//	MPU_IIC_Delay();
//	MPU_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MPU_IIC_Send_Byte(uint8 txd)
{                        
    uint8 t;   
	DIR_OUT(); 	    
    SCL0();//MPU_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if(txd & 0x80)	SDA1();//SDA 输出数据
        else			SDA0();
        txd <<= 1;
        MPU_IIC_Delay();
        SCL1();                //SCL 拉高，采集信号
        MPU_IIC_Delay();
        SCL0();                //SCL 时钟线拉低
        MPU_IIC_Delay();
//        MPU_IIC_SDA=(txd&0x80)>>7;
//        txd<<=1; 	  
//		    SCL1();//MPU_IIC_SCL=1;
//		    MPU_IIC_Delay(); 
//		    SCL0();//MPU_IIC_SCL=0;	
//		    MPU_IIC_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	DIR_IN();//MPU_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
       SCL0();// MPU_IIC_SCL=0; 
        MPU_IIC_Delay();
		SCL1();//MPU_IIC_SCL=1;
        receive<<=1;
        if(SDA)receive++;   
		MPU_IIC_Delay(); 
    }					 
    if (!ack)
        MPU_IIC_NAck();//发送nACK
    else
        MPU_IIC_Ack(); //发送ACK   
    return receive;
}

























