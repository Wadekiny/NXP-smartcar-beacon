#include "barrier.h"
#include "ALLBCN.h"
uint8 master_barrier=0;  //障碍
float barrier_vy;
uint16 barrier_vx;
uint16 barrier_x_minus=100;
uint16 barrier_must_y=100;
uint16 barrier_must_y2=100;
uint16 barrier_cnt=100;
uint16 barrier_delay=100;
uint8 slave_barrier=0;
void BARRIER_Control()
{
    float vz;
    int vy,vx;
    int16 offset_amend_l=0;
    int16 offset_amend_r=0;
    master_barrier = 0;


    if ((bcn_x !=0 || bcn_y !=0) &&  (barrier_x!=0 || barrier_y!=0))//存在信标,障碍 openmv
    {
        if (bcn_y < barrier_y-10)  //灯在障碍后面，壁障动作
        {
            //壁障
            offset_amend_l = (int16)CTRL_GetOffset(barrier_y,FRONT_DIR,LL);
            offset_amend_r = (int16)CTRL_GetOffset(barrier_y,FRONT_DIR,RR);
            
            if (barrier_x < center_x+offset_amend_l && barrier_x > center_x+offset_amend_r)//判断x条件
            {
                if (barrier_y >= barrier_must_y) //距离太近 必须避障
                {
                    if (barrier_x > center_x)
                            master_barrier = 1;
                        else 
                            master_barrier = 2;
                }
                else //距离远 
                {
                    if (ALGO_Fabs(barrier_x - bcn_x) < barrier_x_minus)
                    {
                        if (barrier_x > center_x)
                            master_barrier = 1;
                        else 
                            master_barrier = 2;
                    }
                }
            }
        }   //判断y条件 
    }
    
    //方向2
    else if ((bcn_x2 !=0 || bcn_y2 !=0) &&  (barrier_x2!=0 || barrier_y2!=0))//存在信标,障碍 openmv
    {
        if (bcn_y2 < barrier_y2-10)  //灯在障碍后面，壁障动作
        {
            //壁障
            offset_amend_l = (int16)CTRL_GetOffset(barrier_y2,BACK_DIR,LL);
            offset_amend_r = (int16)CTRL_GetOffset(barrier_y2,BACK_DIR,RR);
            
            if (barrier_x2 < center_x2+offset_amend_l && barrier_x2 > center_x2+offset_amend_r)//判断x条件
            {
                if (ALGO_Fabs(barrier_x2 - bcn_x2) < barrier_x_minus)
                {
                    if (barrier_x2 > center_x2)
                        master_barrier = 1;
                    else 
                        master_barrier = 2;
                }
            }
        }   //判断y条件 
    }
    if (master_barrier != 0)//判断有障碍
    {
        if (slave_barrier == 0)//上一次无障碍
        {
            barrier_cnt=0;//开始计时，障碍方向不改变
        }
        else//上一次有障碍 （连续障碍）
        {
            if (barrier_cnt <= barrier_delay)//在延时时间内
            {
                barrier_cnt++;
                master_barrier = slave_barrier; //等于上次障碍方向
            }
            else//在延时时间之外，可能出现连续障碍，不动master_barrier
            {
                barrier_cnt=0;
            }
        }
    }
    else
    {
        //判断无障碍,下次直接判断方向，cnt加满
        barrier_cnt = barrier_delay;
    }
    //两次方向不等
   
    master_barrier *=send_en;
    slave_barrier = master_barrier;
    if (master_barrier == 0)BEEP_OFF();
    else BEEP_ON();
    
    if (master_barrier != 0)//壁障
    {
        PID_PositionInit(&pid_position);
        if (master_barrier == 1)
        {
            vx = -barrier_vx;
            vy = (int)(ALGO_Fabs(act_speed_y) * (-barrier_vy));
            vz = 0;
            CTRL_SpeedSet(&wheel,vx,vy,vz);
        }
        else if (master_barrier == 2)
        {
            vx = barrier_vx;
            vy = (int)(ALGO_Fabs(act_speed_y) * (-barrier_vy));
            vz = 0;
            CTRL_SpeedSet(&wheel,vx,vy,vz);
        }
        CTRL_PwmCompensateCal(PWM_COMPENSATE_Z);
        CTRL_PwmCompensateClear(PWM_COMPENSATE_X);
    }
}



