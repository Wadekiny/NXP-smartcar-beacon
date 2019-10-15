#include "crash.h"
#include "ALLBCN.h"

uint8 crash_judge_y=2;      //碰撞检测y变化阈值
uint16 crash_check_time=70; //碰撞检测用的循环次数
uint8 crash_key_en=0;
//移动方式：
/*
1 纯左横移
2 纯右横移
*/
uint16 crashkey_state=0;
uint8 CRASH_Get()
{
    uint8 left_key0,right_key0,left_key2,right_key2;
    
    static uint16 last_key_state=0;
    static uint8 key_count=0;
    left_key0 = !gpio_get(LEFTKEY0);
    right_key0 = !gpio_get(RIGHTKEY0);
    left_key2 = !gpio_get(LEFTKEY2);
    right_key2 = !gpio_get(RIGHTKEY2);
    crashkey_state = left_key0*1000 + right_key0*100
                   + left_key2*10   + right_key2;
    //0000
    if (crashkey_state == last_key_state)
    {
        key_count++;
        if (key_count > 1)
        {
            key_count=2;
            crashkey_state *= crash_key_en;
            return crashkey_state;
        }
        
    }
    else
    {
        key_count=0;
        crashkey_state *= crash_key_en;
        last_key_state = crashkey_state;
    }
    
    return 0;
}
void CRASH_KeyInit()
{
    gpio_init  (LEFTKEY0,  GPI, 0);
    gpio_init  (RIGHTKEY0, GPI, 0);
    gpio_init  (LEFTKEY2,  GPI, 0);
    gpio_init  (RIGHTKEY2, GPI, 0);
    port_init_NoAlt(LEFTKEY0,  PULLUP);
    port_init_NoAlt(RIGHTKEY0, PULLUP);
    port_init_NoAlt(LEFTKEY2,  PULLUP);
    port_init_NoAlt(RIGHTKEY2, PULLUP);
}
uint16 all_key_state=0;
uint8 CRASH_Check() //碰撞检测 对抗时再用
{
    static uint8 bcn_count = 0;
    static uint8 max_bcn = 0;
    static uint8 min_bcn = 239;
    static uint8 bcn_count2 = 0;
    static uint8 max_bcn2 = 0;
    static uint8 min_bcn2 = 239;
    CRASH_Get();
    all_key_state =crashkey_state;//     //所有行程开关状态
    //从机的车头碰撞检测
    if (back_en == 0) return 0;
    if (all_key_state != 0)//按键检测到，至少有一个触发
    {
        bcn_count = 0;
        max_bcn = 0;
        min_bcn = 239;
        bcn_count2 = 0;
        max_bcn2 = 0;
        min_bcn2 = 239;
        if (all_key_state < 100)//后车头触发 11，10 01
        {
            if (car_dir==2)//后车头触发，后车头跑
            {
                if (bcn_x2 ==0 && bcn_y2 == 0)//没看到信标
                {
                    car_dir = 0;//换车头转弯
                    turn_judge=0;//再次判断红外
                }
                else//看到信标
                {
                    if (all_key_state == 1) //后车头左触发
                    {
                        return 2;
                    }
                    else if (all_key_state == 10)       //后右
                    {
                        return 1;
                    }
                    else//全部
                    {
                        if (bcn_x2 < center_x2)//灯在左
                        {
                            return 1;
                        }
                        else 
                        {
                            return 2;
                        }
                    }
                }
            }
        }
        else if (all_key_state % 100 == 0)//前车头触发
        {
            if (car_dir == 0)
            {
                if (bcn_x ==0 && bcn_y == 0)//没看到信标
                {
                    car_dir = 2;//换车头转弯
                    turn_judge=0;//再次判断红外
                }
                else//看到信标
                {
                    if (all_key_state == 1000) //前车头左触发
                    {
                        return 2;
                    }
                    else if (all_key_state == 100)       //前右
                    {
                        return 1;
                    }
                    else//全部
                    {
                        if (bcn_x < center_x)//灯在左
                        {
                            return 1;
                        }
                        else 
                        {
                            return 2;
                        }
                    }
                }
            }
        }
        else//前后都触发
        {
            if (car_dir == 0)
            {
                if (bcn_x == 0 && bcn_y == 0)//无信标
                {
                    if (inf_dir == 1) return 1;
                    else return 2;
                }
                else
                {
                    if (bcn_x < center_x)//灯在左
                    {
                        return 1;
                    }
                    else 
                    {
                        return 2;
                    }  
                }
            }
            if (car_dir == 2)
            {
                if (bcn_x2 == 0 && bcn_y2 == 0)//无信标
                {
                    if (inf_dir == 1) return 2;
                    else return 1;
                }
                else
                {
                    if (bcn_x2 < center_x2)//灯在左
                    {
                        return 1;
                    }
                    else 
                    {
                        return 2;
                    }  
                }
            }
        }
    }
    
    //信标位置长时间不变
    if (car_dir == 0)
    {
        bcn_count2 = 0;
        max_bcn2 = 0;
        min_bcn2 = 239;
        if (bcn_y >= max_bcn) max_bcn = bcn_y;
        if (bcn_y <= min_bcn) min_bcn = bcn_y;
        if (bcn_count < crash_check_time)	//计次数
        {
            bcn_count++;
        }
        else
        {
            bcn_count = 0;
            if (max_bcn - min_bcn < crash_judge_y)
            {   
                max_bcn = 0;
                min_bcn = 239;
                
                if (bcn_x == 0 && bcn_y ==0)    //长时间看不到
                {
                    car_dir = 2;
                    turn_judge=0;
                }
                    
                else
                {
                    if (bcn_x < center_x)
                        return 1;                   //长时间看到不动
                    else 
                        return 2;
                }
            }
            else
            {
                max_bcn = 0;
                min_bcn = 239;
            }
        }
    }
    
    if (car_dir == 2)
    {
        bcn_count = 0;
        max_bcn = 0;
        min_bcn = 239;
        if (bcn_y2 >= max_bcn2) max_bcn2 = bcn_y2;
        if (bcn_y2 <= min_bcn2) min_bcn2 = bcn_y2;
        if (bcn_count2 < crash_check_time)	//计次数
        {
            bcn_count2++;
        }
        else
        {
            bcn_count2 = 0;
            if (max_bcn2 - min_bcn2 < crash_judge_y)
            {   
                max_bcn2 = 0;
                min_bcn2 = 239;
                
                if (bcn_x2 == 0 && bcn_y2 ==0)    //长时间看不到
                {
                    car_dir = 0;
                    turn_judge=0;
                }
                    
                else
                {
                    if (bcn_x2 < center_x2)
                        return 1;                   //长时间看到不动
                    else 
                        return 2;
                }
            }
            else
            {
                max_bcn2 = 0;
                min_bcn2 = 239;
            }
        }
    }
    return 0;
}

void CRASH_Solve(uint8 crash_mode)
{
    if (crash_mode == 0) 
    {
        return;
    }
    else if (crash_mode == 1)
    {
         CTRL_SpeedSet(&wheel,-300,-150,0);
         systick_delay_ms(500);
    }
    else if (crash_mode == 2)
    {
        CTRL_SpeedSet(&wheel,300,-150,0);
        systick_delay_ms(500);
    }

}