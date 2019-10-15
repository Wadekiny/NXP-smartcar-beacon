#include "control.h"
#include "ALLBCN.h"

/*******************重要参数************************/
/*******************重要参数************************/
/*******************重要参数************************/
int8 offset_add=0;            //偏移量补偿
uint8 car_dir = 0;             //0正走 2 倒走
int16 act_speed1=0,act_speed2=0,act_speed3=0,act_speed4=0;  //四个轮子实际速度
int16 act_speed_x=0,act_speed_z=0;//x,y,z速度
int16 act_speed_l=0,act_speed_r=0;//l,r速度
float act_speed_y=0;
float filter_speed=0;

int16 v_l =0,v_r=0;
int16 up_speed=500,def_speed=300,down_speed=150;      //三档速度 
int16 curve_vx=500,curve_vy=200,curve_vz=400;         //转弯速度
int16 curve_vy2=200,curve_vz2=400;

uint8 is_fuzzy=10, offset_center=5, force_dir=0,force_turn_dir=0;    
float para_speed_down=0.45;
float cnt_derta_y=1.0;
uint16 is_rush=0;
//是否模糊，第一眼看到中心偏移，固定方向 0自动 1左 2右 3设置数组
uint8 aim_cnt=1;   //err-err_1/gyro_z代入d  

uint16 center_x=160,center_x2=150;   //x中心
uint16 up_y=58,up_y2=62;      //加速减速Y
uint16 pd_y=60,pd_y2=70;      //切到灯的Y值

uint16 angle_y = 50;            //后面判断用
uint16 bcn_x=0, bcn_y=0;
uint16 bcn_x2=0,bcn_y2=0;
uint16 bcn_width=0,bcn_width2=0;//两摄像头信标宽度

uint8 start_sw = 0;             //发车按键
uint8 back_en = 1;              //倒车使能
uint8 crash_en = 1;             //碰撞检测
uint8 send_en = 0;              //上报使能

uint8 light_cnt_flag = 0;      //数灯标志   
uint8 light_index = 0;         //已数灯数
uint8 light_count = 0;         //目标灯数

uint8 cut_dir[30] = {0};        //切灯方向
uint8 force_car_dir[15]={0};   //强制车头方向
uint8 force_speed_down[15]={0};//强制加减速
uint8 turn_dir;                 //转弯方向

uint8 inf_dir=0;    //红外盘方向
uint8 now_dir = LL; //当前切灯方向

uint8 turn_judge=0; //判断转弯方向标志位
uint8 infra_cnt=0;  //红外判断次数
uint8 l_cnt=0;      //左红外
uint8 r_cnt=0;      //右红外
int16 minus_l=0;    //前后轮差速
int16 minus_r=0;
int pwm_buchang=0;  //为满足差速要求补偿占空比
int x_buchang_l=0;
int x_buchang_r=0;
int16 pvfb=15   ;
uint8 must_dir=0;
uint8 must_down=0;//减速

int16 gyro_thd=100;     //gyro减速阈值

uint8 first_light_pit=0;//新灯pit定时设置下线
uint8 pit_finish_flag = 1;//定时器结束标志
uint16 light_pit_ms=0; //设置时间
uint16 light_pit_ms2=0; //设置时间
int16 vy_pit=0;         //pit内速度下限

uint16 down_y = 116,down_y2=130;
uint16 ahead_down_thd = 450;

/*
信标位置
位置pid算出速度vz
增量pid控制速度
*/
struct sMecanum wheel;
void CTRL_SpeedSet(struct sMecanum* wheel, int16 vx, int16 vy, float vz)  //设置三个坐标轴的速度,速度分解
{
    wheel->vx = vx;
    wheel->vy = vy;
    wheel->vz = vz;
    wheel->v1 = (int16)(wheel->vy - wheel->vx + wheel->vz*(MECANUM_A + MECANUM_B)+0.5);
    wheel->v2 = (int16)(wheel->vy + wheel->vx - wheel->vz*(MECANUM_A + MECANUM_B)+0.5);
    wheel->v3 = (int16)(wheel->vy - wheel->vx - wheel->vz*(MECANUM_A + MECANUM_B)+0.5);
    wheel->v4 = (int16)(wheel->vy + wheel->vx + wheel->vz*(MECANUM_A + MECANUM_B)+0.5);
    wheel->vl = (wheel->v2 + wheel->v3)/2;
    wheel->vr = (wheel->v1 + wheel->v4)/2;
}



/**************************************************************************
  * @brief        : 主控函数
  * @others       : 动态更改阈值，发车检测，碰撞检测，舵机控制，电机控制
***************************************************************************/
int vx=0,vy=0;float vz = 0;       
float set_x = 0;    //设定X坐标
float curve_minus=0;//转弯时的差速

uint8 first_curve_flag=1;//刚刚入弯标志位 1是第一次入弯
uint8 is_first_down = 0;//第一次进入减速区，判断减速速度 1为第一次进入
int16 cal_down_speed=200;
float minus_error=0;
void CTRL_Control()
{
//    float derta_y=0,derta_x;
    if (CTRL_StartCheck() == 1)      //发车
    {
        light_count = aim_cnt;
        if (CTRL_LightCount(light_count) == 1) //数灯达到个数
        {
            is_time_start = 0;
            BEEP_OFF();
            CTRL_SpeedSet(&wheel,0,0,0);  
            CTRL_PwmCompensateClear(PWM_COMPENSATE_XZ);
        }
        else     //未开启数灯功能或未达到预定个数
        {
            is_time_start = 1;
            if (back_en == 1) CRASH_Solve(CRASH_Check());//碰撞检测
            if ((bcn_x!=0 || bcn_y!=0)/* && bcn_y < pd_y*/)  //openmv看到灯且未切灯控制 
            {
                //正向
                PID_PositionInit(&pid_curve);
                car_dir = FRONT_DIR;
                //红外盘判断
                first_curve_flag = 1;
                turn_judge=0;//重置
                infra_cnt=0;
                l_cnt=0;
                r_cnt=0; 
                minus_error = (wheel.vr - wheel.vl) - (act_speed_r - act_speed_l);
                BARRIER_Control();      //障碍判断，壁障
                if (master_barrier == 0)//不壁障
                {
//畸变补偿
//                    derta_x = ALGO_Fabs(bcn_x - 160);
//                    derta_y =  derta_x*derta_x*0.0005-0.0109*derta_x;
//                    if (derta_y > 8) derta_y = 8;
//                    if (derta_y < 0) derta_y = 0;
                    //重置转弯pid，积分清零
                    vx = 0;
                    //vy速度控制
                    if (bcn_y < down_y)//第一个减速点前加速
                    {
                        vy = up_speed;
                    }
                    else
                    {
                        if (bcn_y < up_y)//第二个减速点前
                        {
                            if(act_speed_y >= ahead_down_thd)//远灯，速度已经比较大了
                            {
                                //进入减速区
                                if (act_speed_y > down_speed)//实际速度比设定的中间速度大
                                {
                                    cal_down_speed = (int)(down_speed - (act_speed_y-down_speed)*para_speed_down);
                                }
                                else
                                {
                                    cal_down_speed = down_speed;
                                }
                                if (cal_down_speed < -500) cal_down_speed =-500;
                                vy = cal_down_speed;
                            }
                            else//第二个减速点前速度并不快 继续加速
                            {
                                vy = up_speed;
                            }
                        }
                        else//第二个减速点后 ，强制减速
                        {
                            if (act_speed_y > down_speed)//实际速度比设定的中间速度大
                            {
                                cal_down_speed = (int)(down_speed - (act_speed_y-down_speed)*para_speed_down);
                            }
                            else
                            {
                                cal_down_speed = down_speed;
                            }
                            if (cal_down_speed < -500) cal_down_speed =-500;
                            vy = cal_down_speed;
                        }
                    }
                    if (is_rush == 1 && light_index == aim_cnt-1)//rush
                    {
                        vy = up_speed;
                    }
                    if (ALGO_Fabs(gyro_z) >= gyro_thd) 
                    {
//                        if (vy > curve_vx)
//                        {
                            vy = curve_vx;
//                        }
                    }
                    else//直线
                    {
                        if (first_light_pit != 0) 
                        {
                            if (first_light_pit == 1)
                            {
                                pit_init_ms(pit1,light_pit_ms); 					
                            }
                            else//2
                            {
                                pit_init_ms(pit1,light_pit_ms2);//可调 	
                            }
                            enable_irq(PIT1_IRQn);
                            first_light_pit = 0;
                            pit_finish_flag = 0;
                            
                        }
                        if (pit_finish_flag == 0)
                        {
                            //BEEP_ON();
                            if (vy < vy_pit)
                            {
                                vy = vy_pit;
                            }
                        }
                    }
                    //vz速度
                    
                    set_x = CTRL_GetSetX(bcn_x, bcn_y,car_dir);                
                    vz = PID_PositionCal(&pid_position, set_x, bcn_x);

                    CTRL_SpeedSet(&wheel,vx,vy,vz);
                    CTRL_PwmCompensateCal(PWM_COMPENSATE_XZ);
                }
            }
            else if ((bcn_x2!=0 || bcn_y2!=0) /*&& bcn_y2 < pd_y2*/)
            {
                //红外盘判断
                PID_PositionInit(&pid_curve);
                car_dir = BACK_DIR;
                //红外盘判断
                first_curve_flag = 1;
                turn_judge=0;//重置
                infra_cnt=0;
                l_cnt=0;
                r_cnt=0; 
                minus_error = (wheel.vr - wheel.vl) - (act_speed_r - act_speed_l);
                BARRIER_Control();      //障碍判断，壁障
                if (master_barrier == 0)//不壁障
                {
//畸变补偿
//                    derta_x = ALGO_Fabs(bcn_x - 160);
//                    derta_y =  derta_x*derta_x*0.0005-0.0109*derta_x;
//                    if (derta_y > 8) derta_y = 8;
//                    if (derta_y < 0) derta_y = 0;
                    //重置转弯pid，积分清零
                    vx = 0;
                    //vy速度控制;
                    if (bcn_y2 < down_y2)//第一个减速点前加速
                    {
                        vy = up_speed;
                    }
                    else
                    {
                        if (bcn_y2 < up_y2)//第二个减速点前
                        {
                            if(act_speed_y >= ahead_down_thd)//远灯，速度已经比较大了
                            {
                                //进入减速区
                                if (act_speed_y > down_speed)//实际速度比设定的中间速度大
                                {
                                    cal_down_speed = (int)(down_speed - (act_speed_y-down_speed)*para_speed_down);
                                }
                                else
                                {
                                    cal_down_speed = down_speed;
                                }
                                if (cal_down_speed < -500) cal_down_speed =-500;
                                vy = cal_down_speed;
                            }
                            else//第二个减速点前速度并不快 继续加速
                            {
                                vy = up_speed;
                            }
                        }
                        else//第二个减速点后 ，强制减速
                        {
                            if (act_speed_y > down_speed)//实际速度比设定的速度大
                            {
                                cal_down_speed = (int)(down_speed - (act_speed_y-down_speed)*para_speed_down);
                            }
                            else
                            {
                                cal_down_speed = down_speed;
                            }
                            if (cal_down_speed < -500) cal_down_speed =-500;
                            vy = cal_down_speed;
                        }
                    }
                    if (is_rush == 1 && light_index == aim_cnt-1)//rush
                    {
                        vy = up_speed;
                    }
                    
                    if (ALGO_Fabs(gyro_z) >= gyro_thd) 
                    {
//                        if (vy > curve_vx)
//                        {
                            vy = curve_vx;
//                        }
                    }
                    else//直线
                    {
                        if (first_light_pit != 0) 
                        {
                            if (first_light_pit == 2)
                            {
                                pit_init_ms(pit1,light_pit_ms); 					
                            }
                            else
                            {
                                pit_init_ms(pit1,light_pit_ms2);//可调 	
                            }
                            enable_irq(PIT1_IRQn);
                            first_light_pit = 0;
                            pit_finish_flag = 0;
                        }
                        if (pit_finish_flag == 0)
                        {
                            // BEEP_ON();
                            if (vy < vy_pit)
                            {
                                vy = vy_pit;
                            }
                        }
                    }
                    //vz速度
                    set_x = CTRL_GetSetX(bcn_x2, bcn_y2,car_dir);                
                    vz = PID_PositionCal(&pid_position, set_x, bcn_x2);

                    CTRL_SpeedSet(&wheel,vx,vy,vz);
                    CTRL_PwmCompensateCal(PWM_COMPENSATE_XZ);
                }
            }//
            else            //切灯或看不到灯 
            {
                is_first_down = 1;		//再看到灯重新计算减速 
                CTRL_PwmCompensateClear(PWM_COMPENSATE_Z);
                CTRL_PwmCompensateCal(PWM_COMPENSATE_X);
                PID_PositionInit(&pid_position);    //重置切灯pid 积分清零
			
                turn_dir = CTRL_GetTurnDir();       //转弯方向
                if (now_dir == turn_dir)//左切左转，右切右转 小弯
                {
                    vy = curve_vy;
                    if (turn_dir == LL)
                        curve_minus = PID_Curve(&pid_curve,curve_vz,(int16)gyro_z); 
                    else if (turn_dir == RR)
                        curve_minus = PID_Curve(&pid_curve,-curve_vz,(int16)gyro_z); 
                }
                else if (turn_dir != STANDBY)//左切右转，右切左转，大弯
                {
                    vy = curve_vy2;
                    if (turn_dir == LL)
                        curve_minus = PID_Curve(&pid_curve,curve_vz2,(int16)gyro_z); 
                    else if (turn_dir == RR)
                        curve_minus = PID_Curve(&pid_curve,-curve_vz2,(int16)gyro_z); 
                }
                else//standby
                {
                    vy = curve_vy/2;
                    curve_minus = 0;
                }
                
                vz = curve_minus/(MECANUM_A+MECANUM_B);
                CTRL_SpeedSet(&wheel,0,vy,vz);
                
                if (wheel.v1 < 0) wheel.v1 = 0;
                if (wheel.v2 < 0) wheel.v2 = 0;
                if (wheel.v3 < 0) wheel.v3 = 0;
                if (wheel.v4 < 0) wheel.v4 = 0;             
            }         
        }
    }
	else
    {
        is_time_start = 0;
        BEEP_OFF();
        CTRL_SpeedSet(&wheel,0,0,0);     //未发车
		CTRL_PwmCompensateClear(PWM_COMPENSATE_XZ);
        PID_PositionInit(&pid_position);    //方向环pid初始化
        PID_PositionInit(&pid_z);		//z补偿pd初始化
        PID_PositionInit(&pid_curve);       //丢灯转弯pid初始化
        PID_MotorInit(&pid_motor_1);        //电机PID初始化
        PID_MotorInit(&pid_motor_2);        //电机PID初始化
        PID_MotorInit(&pid_motor_3);        //电机PID初始化
        PID_MotorInit(&pid_motor_4);        //电机PID初始化
    } 
}



/**************************************************************************
  * @brief        : 数灯功能
  * @param[in]    : 目标灯数
  * @return       : 0-未达到设定值    1-达到设定值
  * @others       : 目标灯数为0时不数灯，返回0
***************************************************************************/
uint8 CTRL_LightCount(uint8 aim_count)
{
    uint16 temp_index = light_index;
    if (!(last_bcn_y==0 && last_bcn_x==0))
    {
        if (last_bcn_y > bcn_y && last_bcn_y > pd_y)
        {
            if (ALGO_Fabs(last_bcn_y - bcn_y) > (int )(cnt_derta_y*100))
            {
                if (light_index < 30)
                {
                    first_light_pit = 1;//数到新灯
                    light_index++;
                }
            }
        }
    }
  
    if (!(last_bcn_y2==0 && last_bcn_x2==0))
    {
        if (last_bcn_y2 > bcn_y2 && last_bcn_y2 > pd_y2)
        {
            if (ALGO_Fabs(last_bcn_y2 - bcn_y2) > (int )(cnt_derta_y*100))
            {
                if (light_index < 30)
                {
                    first_light_pit = 2;//数到新灯
                    light_index++;
                }
            }
        }
    }
    if (light_index >= temp_index+2)
    {
        light_index = temp_index+1;
    }
    if (aim_count == 0) return 0;    //切灯个数设为0时返回未达到设定值
    if (light_index >= aim_count)   //计数值达到设定值
    {
        light_index = aim_count;
        return 1;
    }
    return 0;
}

/**************************************************************************
  * @brief        : 得到位置pid设定值
  * @param[in]    : 信标坐标 bcn_x, bcn_y
  * @return       : 信标位置设定值
***************************************************************************/\
uint8 x_act,x_set;
uint8 last_dirdir=LL;
float CTRL_GetOffset(uint16 bcn_y,uint8 car_dir,uint8 dir)
{
    float offset_amend=0;
    if (car_dir == FRONT_DIR)
    {
        if (dir == LL)   
        {
            if (bcn_y < 64)
            {
                offset_amend=17;
            }
            else if (bcn_y < 68)
            {
                offset_amend=23+(bcn_y-67)*1.5;
            }
            else if (bcn_y < 69)
            {
                offset_amend=31+(bcn_y-68)*8;
            }
            else if (bcn_y < 73)
            {
                offset_amend=37+(bcn_y-72)*1.5;
            }
            else if (bcn_y < 77)
            {
                offset_amend=47+(bcn_y-76)*2.5;
            }
            else if (bcn_y < 78)
            {
                offset_amend=49+(bcn_y-77)*2;
            }
            else if (bcn_y<81)
            {
                offset_amend=55+(bcn_y-80)*2;
            }
            else if (bcn_y<83)
            {
                offset_amend=59+(bcn_y-82)*2;
            }
            else if (bcn_y < 87)
            {
                offset_amend=65+(bcn_y-86)*1.5;
            }
            else if (bcn_y < 89)
            {
                offset_amend=70+(bcn_y-88)*2.5;
            }
            else if (bcn_y<94)
            {
                offset_amend=78+(bcn_y-93)*1.6;
            }
            else if (bcn_y < 99)
            {
                offset_amend = 90+(bcn_y-98)*2.4;
            }
            else if (bcn_y < 106)
            {
                offset_amend=100+(bcn_y-105)*1.43;
            }
            else if (bcn_y < 113)
            {
                offset_amend=117+(bcn_y-112)*2.43;
            }
            else if (bcn_y < 125)
            {
                offset_amend=136+(bcn_y-124)*1.58;
            }
            else if (bcn_y < 141)
            {
                offset_amend = 153+(bcn_y-140)*1.06;
            }
            else if (bcn_y < 156)
            {
                offset_amend = 160+(bcn_y-155)*0.47;
            }
            else
            {
                offset_amend=165;
            }
        }
        else//右切
        {
            if (bcn_y < 65)
            {
                offset_amend=16;
            }
            else if (bcn_y < 67)
            {
                offset_amend=21+(bcn_y-66)*2.5;
            }
            else if (bcn_y < 69)
            {
                offset_amend=27+(bcn_y-68)*3;
            }
            else if (bcn_y < 73)
            {
                offset_amend=34+(bcn_y-72)*1.75;
            }
            else if (bcn_y < 76)
            {
                offset_amend=44+(bcn_y-75)*3.33;
            }
            else if (bcn_y < 83)
            {
                offset_amend=55+(bcn_y-82)*1.57;
            }
            else if (bcn_y<85)
            {
                offset_amend=60+(bcn_y-84)*2.5;
            }
            else if (bcn_y<89)
            {
                offset_amend=70+(bcn_y-88)*2.5;
            }
            else if (bcn_y < 96)
            {
                offset_amend=81+(bcn_y-95)*1.57;
            }
            else if (bcn_y < 101)//
            {
                offset_amend=93+(bcn_y-100)*2.4;
            }
            else if (bcn_y < 104)//
            {
                offset_amend=100+(bcn_y-103)*2.33;
            }
            else if (bcn_y<110)
            {
                offset_amend=108+(bcn_y-109)*1.33;
            }
            else if (bcn_y < 114)
            {
                offset_amend = 118+(bcn_y-113)*2.5;
            }
            else if (bcn_y < 120)
            {
                offset_amend=129+(bcn_y-119)*1.83;
            }
            else if (bcn_y < 128)
            {
                offset_amend=135+(bcn_y-127)*0.75;
            }
            else if (bcn_y < 143)
            {
                offset_amend=144+(bcn_y-142)*0.6;
            }
            else
            {
                offset_amend=145;
            }
            offset_amend = -offset_amend;
        }
    }
    else if (car_dir == BACK_DIR)
    {
        if (dir == LL)   
        {
            if (bcn_y < 46)
            {
                offset_amend=12;
            }
            else if (bcn_y < 47)
            {
                offset_amend=19;
            }
            else if (bcn_y <48)
            {
                offset_amend=22;
            }
            else if (bcn_y < 51)
            {
                offset_amend=30+(bcn_y-50)*2.67;
            }
            else if (bcn_y < 53)
            {
                offset_amend=36+(bcn_y-52)*3;
            }
            else if (bcn_y < 56)
            {
                offset_amend=42+(bcn_y-55)*2;
            }
            else if (bcn_y < 59)
            {
                offset_amend=51+(bcn_y-58)*3;
            }
            else if (bcn_y < 62)
            {
                offset_amend=56+(bcn_y-61)*1.67;
            }
            else if (bcn_y < 64)
            {
                offset_amend=61+(bcn_y-63)*2.5;
            }
            else if (bcn_y < 66)
            {
                offset_amend=66+(bcn_y-64)*2.5;
            }
            else if (bcn_y < 69)
            {
                offset_amend=73+(bcn_y-68)*2.33;
            }
            else if (bcn_y<78)
            {
                offset_amend=96+(bcn_y-77)*2.56;
            }
            else if (bcn_y < 86)
            {
                offset_amend=107+(bcn_y-85)*1.38;
            }
            else if (bcn_y < 95)
            {
                offset_amend=124+(bcn_y-94)*1.89;
            }
            else if (bcn_y < 103)
            {
                offset_amend = 138+(bcn_y-102)*1.75;
            }
            else if (bcn_y < 112)
            {
                offset_amend = 149+(bcn_y-111)*1.22;
            }
            else if (bcn_y < 122)
            {
                offset_amend=157+(bcn_y-121)*0.8;
            }
            else if (bcn_y < 140)
            {
                offset_amend=164+(bcn_y-139)*0.39;
            }
            else
            {
                offset_amend=166;
            }
        }
        else//右切
        {
            if (bcn_y < 45)
            {
                offset_amend=21;
            }
            else if (bcn_y < 48)
            {
                offset_amend=25+(bcn_y-47)*1.33;
            }
            else if (bcn_y < 51)
            {
                offset_amend=37+(bcn_y-50)*4;
            }
            else if (bcn_y < 55)
            {
                offset_amend=47+(bcn_y-54)*2.5;
            }
            else if (bcn_y < 59)
            {
                offset_amend=57+(bcn_y-58)*2.5;
            }
            else if (bcn_y < 66)
            {
                offset_amend=69+(bcn_y-65)*1.71;
            }
            else if (bcn_y < 72)
            {
                offset_amend=78+(bcn_y-71)*1.5;
            }
            else if (bcn_y < 76)
            {
                offset_amend=87+(bcn_y-75)*2.25;
            }
            else if (bcn_y < 80)
            {
                offset_amend=96+(bcn_y-79)*2.25;
            }
            else if (bcn_y < 85)
            {
                offset_amend=105+(bcn_y-84)*1.8;
            }
            else if (bcn_y < 89)
            {
                offset_amend = 113+(bcn_y-88)*2;
            }
            else if (bcn_y < 96)
            {
                offset_amend= 124+(bcn_y-95)*1.57;
            }
            else if (bcn_y < 98)
            {
                offset_amend= 129+(bcn_y-97)*1.67;
            }
            else if (bcn_y < 100)
            {
                offset_amend= 131+(bcn_y-99)*1;
            }
            else if (bcn_y < 104)
            {
                offset_amend= 134+(bcn_y-103)*0.75;
            }
            else if (bcn_y < 111)
            {
                offset_amend= 141+(bcn_y-110)*1;
            }
            else if (bcn_y < 122)
            {
                offset_amend= 146+(bcn_y-121)*0.45;
            }
            else
            {
                offset_amend=147;
            }
            offset_amend = -offset_amend;
        }
    }
    return offset_amend;
}
float CTRL_GetSetX(uint16 bcn_x, uint16 bcn_y,uint8 car_dir)
{
    uint8 dir = RR;
    float offset_amend = 0;
    float set_x = 0;
    uint16 center_coord=0;
    if (car_dir == DIR0) center_coord=center_x;
    else                 center_coord=center_x2;
    if (force_dir == 1)//强左
    {
        dir = LL;
    }
    else if (force_dir == 2)//强右
    {
        dir = RR;
    }
    else if (force_dir == 3 && cut_dir[light_index] == 1)//强指定
    {
        dir = LL;
    }
    else if (force_dir == 3 && cut_dir[light_index] == 2)
    {
        dir = RR;
    }
    else
    {
        if (turn_dir == LL)//上一个左转
        {
            if (bcn_x < center_coord+offset_center)//+10
                dir = RR;
            else 
                dir = LL;
        }
        else if (turn_dir == RR)//上一个右转
        {
            if (bcn_x < center_coord-offset_center)//+10
                dir = RR;
            else 
                dir = LL;
        }
        else//standby
        {
            if (bcn_x < center_coord)//+10
                dir = RR;
            else 
                dir = LL;
        }
    }
    if (last_dirdir != dir) pid_position.inter = 0;
    now_dir = dir;

    if (must_dir == 3)
    {
        if (force_car_dir[light_index] == 1 && force_car_dir[light_index+1] == 2)
            set_x = center_x;
        else if (force_car_dir[light_index] == 2 && force_car_dir[light_index+1] == 1)
            set_x = center_x2;
    }
    else //不撞灯
    {
        offset_amend = CTRL_GetOffset(bcn_y,car_dir,dir);
        if (dir == LL)
            set_x = center_coord + offset_amend + offset_add;
        else
            set_x = center_coord + offset_amend - offset_add;
    }
    last_dirdir = dir;
    return set_x;
}

/**************************************************************************
  * @brief        : 发车检测
  * @return       : 0-未满足发车条件   1-满足发车条件
  * @others       : start_flag只有0、1两个值
***************************************************************************/
uint8 CTRL_StartCheck()
{
    static uint8 start_flag = 0;   //发车标志
    //car_dir = DIR0;
    if (start_sw == 1)
    {
        if (start_flag == 0)
        {
            if (bcn_x!=0 || bcn_y!=0)      //满足发车条件
            {
                car_dir = DIR0;
                start_flag = 1;
            }
            else if (bcn_x2!=0 || bcn_y2!=0)
            {
                car_dir = DIR2;
                start_flag = 1;
            }
            else if (red_buffer[0] == red_buffer[1] && red_buffer[0] == red_buffer[3] && red_buffer[0]!=0 && red_buffer[0]!=3)
            {
                start_flag = 1;
            }
                
        }
    }
    else
    {
        start_flag = 0;
    }
    return start_flag;
}

uint8 temp_dir=STANDBY;
uint8 CTRL_GetTurnDir()
{
    //  uint8 last_inf_dir=LL;
    if (force_turn_dir == 1)//强左
    {
        return LL;
    }
    else if (force_turn_dir == 2)//强右
    {
        return RR;
    }
    else if (force_turn_dir == 3)//数组
    {
        if (light_index>0)
            return cut_dir[light_index-1+15];
        else
            return inf_dir;
    }
    else    //对抗
    {
       if (turn_judge == 0)
        {
            infra_cnt++;
            if (infra_cnt > 2)
            {
                if (inf_dir == 1) l_cnt++;
                else if (inf_dir == 2)r_cnt++;
                else
                {
                    l_cnt++;
                    r_cnt++;
                }
            }
        }
        if (infra_cnt > 4)  //5- 0,1,2,3,4
        {
            turn_judge=1;
            if (car_dir == 0)
            {
                if (l_cnt>r_cnt) return LL;
                else if (l_cnt < r_cnt) return RR;
                else//==
                {
                    if (now_dir == RR)
                        return RR ;
                    else
                        return LL;
                }
            }
            else 
            {
                if (l_cnt>r_cnt) return RR;
                else if (l_cnt < r_cnt) return LL;
                else//==
                {
                    if (now_dir == RR)
                        return RR ;
                    else
                        return LL;
                }
            }
        }
        else
        {
            return STANDBY;    
        } 
    }
}

//0 x compensate
//1 z compensate
//2 x and z compensate
void CTRL_PwmCompensateCal(uint8 x_or_z)
{
   if (x_or_z == PWM_COMPENSATE_X)
   {
        x_buchang_l = minus_l * pvfb;
        x_buchang_r = minus_r * pvfb;
        if (x_buchang_l > X_BUCHANG_LIMIT) x_buchang_l = X_BUCHANG_LIMIT;
        if (x_buchang_l < -X_BUCHANG_LIMIT) x_buchang_l = -X_BUCHANG_LIMIT;
        if (x_buchang_r > X_BUCHANG_LIMIT) x_buchang_r = X_BUCHANG_LIMIT;
        if (x_buchang_r < -X_BUCHANG_LIMIT) x_buchang_r = -X_BUCHANG_LIMIT;
   }
   else if (x_or_z == PWM_COMPENSATE_Z)
   {
        pwm_buchang = (int)PID_ZCal(&pid_z, wheel.vr - wheel.vl, act_speed_r - act_speed_l);
        if (pwm_buchang > BUCHANG_LIMIT)pwm_buchang = BUCHANG_LIMIT;
        if (pwm_buchang < -BUCHANG_LIMIT)pwm_buchang = -BUCHANG_LIMIT;
   }
   else
   {
        x_buchang_l = minus_l * pvfb;
        x_buchang_r = minus_r * pvfb;
        if (x_buchang_l > X_BUCHANG_LIMIT) x_buchang_l = X_BUCHANG_LIMIT;
        if (x_buchang_l < -X_BUCHANG_LIMIT) x_buchang_l = -X_BUCHANG_LIMIT;
        if (x_buchang_r > X_BUCHANG_LIMIT) x_buchang_r = X_BUCHANG_LIMIT;
        if (x_buchang_r < -X_BUCHANG_LIMIT) x_buchang_r = -X_BUCHANG_LIMIT;
        pwm_buchang = (int)PID_ZCal(&pid_z, wheel.vr - wheel.vl, act_speed_r - act_speed_l);
        if (pwm_buchang > BUCHANG_LIMIT)pwm_buchang = BUCHANG_LIMIT;
        if (pwm_buchang < -BUCHANG_LIMIT)pwm_buchang = -BUCHANG_LIMIT;
   }
}
void CTRL_PwmCompensateClear(uint8 x_or_z)
{
    if (x_or_z == PWM_COMPENSATE_X)
   {
		x_buchang_l=0;
		x_buchang_r=0;
   }
   else if (x_or_z == PWM_COMPENSATE_Z)
   {
		pwm_buchang=0;
   }
   else
   {
		x_buchang_l=0;
		x_buchang_r=0;
		pwm_buchang=0;
   }
}