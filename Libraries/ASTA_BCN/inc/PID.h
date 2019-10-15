#ifndef _PID_H_
#define _PID_H_

#include "common.h"
#include "ALLBCN.h"
#define MOTOR_MAX       (9950)
#define MOTOR_MIN       (-9950)
#define POSITION_MAX    (5)
#define POSITION_MIN    (-5)

#define DIR0   0
#define DIR2   2

struct sPidPosition //位置pid 位置pid
{
   float kp;
   float ki;
   float kd;
   float inter; //积分
   float err;   
   float err_1;
   float ec;
   float value;
};

struct sPidMotor    //电机pid  增量pid
{
   float s_kp;  //直线pid
   float s_ki;  
   float s_kd;
   float c_kp;  //特殊pid
   float c_ki;
   float c_kd;
   float err;   
   float err_1;
   float err_2;
   float value;
};

float PID_Curve(struct sPidPosition* pid, float set, int act);
float PID_XCal(struct sPidPosition* pid, float set, int act);
void PID_PositionInit(struct sPidPosition* pid);
float PID_PositionCal(struct sPidPosition* pid, float set, int act);
void PID_MotorInit(struct sPidMotor *pid);
float PID_MotorCal(struct sPidMotor* pid, int set, int act, uint8 mode);
void PID_MotorControl(int set1,int set2, int set3, int set4, uint8 mode);
float PID_ZCal(struct sPidPosition* pid, float set, float act);
#endif