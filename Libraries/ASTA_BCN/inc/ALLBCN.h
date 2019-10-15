#ifndef _ALLBCN_H_
#define _ALLBCN_H_

#include "common.h"
#include "headfile.h"

//====head files====
#include "interact.h"
#include "key.h"
//#include "MT9V032.h"
#include "K66_tpm.h"
#include "oled.h"
#include "encode.h"
#include "flash.h"
#include "control.h"
#include "motor.h"
#include "PID.h"
#include "bluetooth.h"
//#include "picture.h"
//#include "master.h"
#include "algo.h"
#include "fuzzy.h"
#include "crash.h"
#include "top6050.h"
#include "pan_infrared.h"
#include "beep.h"
#include "openmv.h"
#include "barrier.h"
/***********************PARAMETER***********************/
extern int8 offset_add;                    //偏移量补偿
extern uint8 start_sw;                      //发车按键
extern uint8 light_count,light_index;       //数灯目标，已数个数
extern uint8 send_en,back_en;    //陀螺仪，上报，后退使能
extern uint8 cut_dir[30];                   //切灯方向
extern int16 act_speed1,act_speed2,act_speed3,act_speed4;  //实际速度
extern int16 up_speed,def_speed,down_speed;                //三档速度
extern int16 curve_vx,curve_vy;                   //转弯速度
extern int16 curve_vz;                          //转弯速度
extern int16 motor_kp,motor_ki,motor_kd;
extern uint16 up_y,down_y,angle_y;           //加速y,减速y，打角y


extern uint8 is_fuzzy,offset_center,force_dir;     //三个阈值参数
extern uint8 aim_cnt;     //阈值切换Y值
extern uint16 center_x;

extern uint16 bcn_x, bcn_y;            //bcn坐标
extern uint16 bcn_x2,bcn_y2;
extern struct sPidMotor pid_motor_1;    //四个电机的pid结构体
extern struct sPidMotor pid_motor_2;    //四个电机的pid结构体
extern struct sPidMotor pid_motor_3;    //四个电机的pid结构体
extern struct sPidMotor pid_motor_4;    //四个电机的pid结构体
extern struct sPidPosition pid_position;//位置pid结构体
extern struct sPidPosition pid_z;
extern struct sPidPosition pid_curve;
extern struct sMecanum wheel;           //麦克纳姆轮车体速度，车轮速度结构体


extern uint8 master_barrier;  //障碍

extern float set_x;

extern int16 value1,value2,value3,value4;

extern uint16 pd_y;
extern int16 position_zp0,position_zp1;
extern int16 position_zd0;

extern int16 pvlr0,pvlr1;



extern int16 gyro_thd;
extern float out0_limit,out1_limit;


extern uint8 cut_dir_set;//0自动 1左 2右 3设置数组
extern uint8 x_set;
extern uint32 runtime;
extern uint8 car_dir;

//extern int out_d3[3];
////extern int in_e3[3];
////extern int in_ec3[3];
//extern int d_rule3[3][3];
extern uint8 show_en;
extern uint8 crash_judge_y;      //碰撞检测y变化阈值
extern uint16 crash_check_time; //碰撞检测用的循环次数
extern uint8 crash_en;

extern uint8 curve_dir;    //下次转弯方向 
extern float cal_p;
extern float cal_d;


extern int16 position_zi0;
extern int16 position_cp,position_ci,position_cd;

extern int pwm_buchang;

extern float acc_x,acc_y,acc_z;
extern float gyro_x,gyro_y,gyro_z;
extern float pitch,roll,yaw;
extern int16 gyro_kalman_q,gyro_kalman_r;
extern uint8 beep_en;

extern uint16 mv_xx;
extern uint16 mv_yy;
extern uint16 mv_width;
extern uint8 inf_dir;
extern uint8 red1,red2,red3,red4,red5,red6,red7,red8,red9,red10,red11,red12;
extern uint16 barrier_x,barrier_y;
extern uint16 barrier_x2,barrier_y2;
extern uint16 bcn_width, bcn_width2;

extern uint8 mv2_flag;
extern uint8 mv_flag;
extern uint16 center_x2;   //x中心
extern uint16 up_y2;      //加速减速Y
extern uint16 pd_y2;  //切到灯的Y值
extern uint16 down_y2;//数灯第一眼看到灯必须小于down_y才算新灯
extern int16 minus_l;    //前后轮差速
extern int16 minus_r;
extern int x_buchang_l;
extern int x_buchang_r;
extern int16 curve_vy2,curve_vz2;
extern uint8 force_turn_dir;
extern float para_speed_down;
extern uint16 is_rush;

extern uint8 force_speed_down[15];
extern uint8 force_car_dir[15];
extern float cnt_derta_y;
extern uint8 turn_judge;
extern uint16 crashkey_state;
extern uint8 must_dir;
extern uint8 must_down;
//extern float aim_gyro;

extern int16 act_speed_x,act_speed_z;//x,y,z速度
extern int16 act_speed_l,act_speed_r;//l,r速度
extern float act_speed_y;
extern float filter_speed;

extern uint16 last_bcn_x,last_bcn_y;
extern uint16 last_bcn_x2,last_bcn_y2;

extern uint8 pit_front_dir;
extern uint8 pit_back_dir;

extern uint8 first_light_pit;//新灯pit定时设置下线
extern uint16 light_pit_ms; //设置时间
extern int16 vy_pit;         //pit内速度下限
extern uint8 pit_finish_flag;
extern double finish_s;
extern uint8 is_time_start;
extern double temp_ms;

extern uint8 red_buffer[10];

extern float barrier_vy;
extern uint16 barrier_vx;
extern uint16 barrier_x_minus;
extern uint16 barrier_must_y;
extern uint16 barrier_must_y2;

extern uint16 protect_cnt1,protect_cnt2;

extern uint16 light_pit_ms2;
extern uint16 ahead_down_thd;

extern uint16 barrier_delay;
extern uint8 crash_key_en;
#endif