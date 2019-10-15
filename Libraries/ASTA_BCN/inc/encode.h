#ifndef _ENCODE_H_
#define _ENCODE_H_

#include "common.h"

#define WHEEL_C         192             //车轮周长192mm
#define ENCODER_PULSE   512            //编码器转动一周脉冲数
#define WHEEL_PULSE    (512*104/50)     //车轮转动一周脉冲数（104车轮齿轮数，50编码器齿轮数）

#define TPM_DIR1    C11 
#define TPM_DIR2    C14

//逐飞编码器齿轮
//模数0.4 齿数50 外径20.8
//龙邱编码器齿轮
//模数0.4 齿数44 外径18.5
//车轮齿数好像是104
//麦克纳姆轮直径61 宽24
//mm

void ENCODE_Init(void);
void ENCODE_Read(int16* act_speed1,int16* act_speed2, int16* act_speed3, int16* act_speed4);
void ENCODE_SpeedCm(int16* act_speed1, int16* act_speed2, int16* act_speed3, int16* act_speed4);
#endif