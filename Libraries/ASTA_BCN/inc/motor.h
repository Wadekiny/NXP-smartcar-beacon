#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "common.h"
//----------电机和编码器接口定义-------------

//电机
#define MOTOR_FTM   ftm3
//#define FORWARD_1   ftm_ch6         //PTC1，高电平正转
//#define BACKWARD_1  ftm_ch7         //PTC2，高电平反转
//#define FORWARD_2   ftm_ch0         //PTC3，高电平正转
//#define BACKWARD_2  ftm_ch1         //PTC4，高电平反转
//#define FORWARD_3   ftm_ch3         //PTC1，高电平正转
//#define BACKWARD_3  ftm_ch2         //PTC2，高电平反转
//#define FORWARD_4   ftm_ch5         //PTC3，高电平正转
//#define BACKWARD_4  ftm_ch4         //PTC4，高电平反转

#define FORWARD_1   ftm_ch6         //PTC1，高电平正转
#define BACKWARD_1  ftm_ch7         //PTC2，高电平反转
#define FORWARD_2   ftm_ch0         //PTC3，高电平正转
#define BACKWARD_2  ftm_ch1         //PTC4，高电平反转
#define FORWARD_3   ftm_ch3         //PTC1，高电平正转
#define BACKWARD_3  ftm_ch2         //PTC2，高电平反转
#define FORWARD_4   ftm_ch5         //PTC3，高电平正转
#define BACKWARD_4  ftm_ch4         //PTC4，高电平反转


#define MOTOR_HZ        18000          //PWM波输出频率 18K

void MOTOR_Init(void);
void MOTOR_Control(int16 duty1,int16 duty2,int16 duty3,int16 duty4);


#endif