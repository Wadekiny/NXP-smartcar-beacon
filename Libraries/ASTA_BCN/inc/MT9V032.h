#ifndef _MT9V032_H_
#define _MT9V032_H_

#include "headfile.h"


#define CAMERA_W    128
#define CAMERA_H    50

#define COL     128//图像宽度   范围1-752     K60采集不允许超过188
#define ROW     50//图像高度	范围1-480

#define CAMERA_W2    80
#define CAMERA_H2    60

#define COL2     80//图像宽度   范围1-752     K60采集不允许超过188
#define ROW2     60//图像高度	范围1-480


//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数
    
    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令
    
    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;



extern uint8 mt9v032_finish_flag;  //一场图像采集完成标志位
extern uint8 image[ROW][COL];      //内部使用

extern uint8 mt9v032_finish_flag2;  //一场图像采集完成标志位
extern uint8 image2[ROW][COL];      //内部使用
//引脚配置

#define MT9V032_COF_UART    uart4  //配置摄像头所使用到的串口     
#define MT9V032_COF_UART2    uart0  //配置摄像头所使用到的串口    

//--------------------------------------------------------------------------------------------------
//        如果修改了场中断引脚那么注意修改PORTC_IRQn、PORTC_ISFR。
//        比如使用的是PTA这组的引脚那么将这两个修改为PORTA_IRQn、PORTA_ISFR即可
//        且务必在对应的中断函数中调用VSYNC函数
//--------------------------------------------------------------------------------------------------
#define MT9V032_PCLK                D14                            	//摄像头像素时钟
        
#define MT9V032_VSYNC               D15                            	//摄像头场同步信号
#define MT9V032_VSYNC_CHANNEL       15                               //MT9V032_VSYNC对应引脚的引脚号 PTC6   -->     6
#define MT9V032_INTERRUPT_NUNBERS   PORTD_IRQn                      //中断编号
#define MT9V032_INTERRUPT_ISFR      PORTD->ISFR                     //中断标志位


/*
摄像头2-------UART0
pclk    A13
VSYNC   A29
RX      A14
TX      A15
DATA    B0-B7   
*/
#define MT9V032_PCLK2                A13                            	//摄像头像素时钟
        
#define MT9V032_VSYNC2               A29                            	//摄像头场同步信号
#define MT9V032_VSYNC_CHANNEL2       29                               //MT9V032_VSYNC对应引脚的引脚号 PTC6   -->     6
#define MT9V032_INTERRUPT_NUNBERS2   PORTA_IRQn                      //中断编号
#define MT9V032_INTERRUPT_ISFR2      PORTA->ISFR                     //中断标志位

//--------------------------------------------------------------------------------------------------
//        摄像头数据接口
//        C_IN_DATA(1)
//        第一个字母C代表着使用C口，可以选择A、B、C、D、E.分别对应A_IN_DATA，B_IN_DATA，C_IN_DATA，D_IN_DATA，E_IN_DATA
//        第二个括号里面的参数1代表着使用的是C8 - C15口,
//		  可以选择0、1、2、3，分别对应的是	该端口的X0-X7，X8-X15，X16-X23，X24-X31(这里x代表的是第一个字母，比如现在的就是C8-C15)
//--------------------------------------------------------------------------------------------------

#define MT9V032_DATAPORT 		E_IN_DATA(0)	                //DMA数据口

void   mt9v032_cof_uart_interrupt(void);
void   VSYNC(void);
void   camera_init(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);

//#define dma_ch_2    

#define MT9V032_DATAPORT2 		B_IN_DATA(0)	                //DMA数据口

void   mt9v032_cof_uart_interrupt2(void);
void   VSYNC2(void);
void   camera_init2(void);
//void   seekfree_sendimg_0322(void);
void   row_finished2(void);



#endif

