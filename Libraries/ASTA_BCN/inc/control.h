#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "ALLBCN.h"

#define MECANUM_A   9       //9cm
#define MECANUM_B   9

#define LL  1       //左转弯
#define RR  2       //右转弯
#define STANDBY    0//等待
#define FRONT_DIR   0
#define BACK_DIR    2
#define PWM_COMPENSATE_X    0
#define PWM_COMPENSATE_Z    1
#define PWM_COMPENSATE_XZ   2

#define X_BUCHANG_LIMIT 3000
#define BUCHANG_LIMIT 10000
struct sMecanum
{
   int16 v1,v2,v3,v4;
   int16 vx,vy;
   float vz;
   int16 vl,vr;
};
uint8 CTRL_GetTurnDir();
float CTRL_GetZSpeed();
void CTRL_SpeedSet(struct sMecanum* wheel, int16 vx, int16 vy, float vz);
void CTRL_Control();
uint8 CTRL_LightCount(uint8 aim_count);
float CTRL_GetSetX(uint16 bcn_x, uint16 bcn_y,uint8 car_dir);
uint8 CTRL_StartCheck();
void CTRL_SpeedDown(uint16 bcn_y);
float CTRL_GetOffset(uint16 bcn_y,uint8 car_dir,uint8 dir);
void CTRL_PwmCompensateCal(uint8 x_or_z);
void CTRL_PwmCompensateClear(uint8 x_or_z);
#endif
