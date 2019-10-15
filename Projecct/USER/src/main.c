#include "headfile.h"
#include "ALLBCN.h"

void INIT_All();
void INIT_Parameter();
uint32 runtime=0;
void MotorTest(int16 duty1,int16 duty2,int16 duty3,int16 duty4);
void ParaInit();
void Protect();

double finish_s=0;
uint8 is_time_start=0;
double temp_ms=0;

uint16 protect_cnt1=0,protect_cnt2=0;
uint8 yan=0;
int main()
{
    get_clk();
    //MotorTest(5000,5000,5000,5000);
    INIT_All();     //初始化 
    
    while (1)
    {   // is_time_start=1;
        pit_time_start(pit2);       //计时开始
        
        MPU6050_GetData();          //获取6050 只取陀螺仪
        inf_dir = INFRARED_Get();
        OPENMV_BcnCoord();
        Protect();
        CTRL_Control();     //控制
        MENU_GetSig();      //得到按键消息
        MENU_SigHandling(); //按键消息处理
        MENU_Display();     //显示菜单 
        yan = !yan;
        BLT_SendData16(/*1*/(int16)(bcn_x),      
                       /*2*/(int16)(inf_dir),
                       /*3*/(int16)(yan),
                       /*4*/(int16)(act_speed_y),                
                       /*5*/(int16)(act_speed1),                
                       /*6*/(int16)(act_speed2),                
                       /*7*/(int16)(value1),   
                       /*8*/(int16)(value2)
                       );       
        runtime = pit_time_get(pit2)/bus_clk_mhz;  //转换为us
        pit_close(pit2);                   //关闭计时
        OLED_Refresh_Gram();               //显示刷新
    }
}
extern uint8 receive_mv;
void Protect()
{
    if (mv_flag == 0)protect_cnt1++;
    else protect_cnt1 = 0;
    if (mv2_flag == 0)protect_cnt2++;
    else protect_cnt2 = 0;
    if (protect_cnt1 > 100)
    {
        bcn_x=0;
        bcn_y=0;
    }
    if (protect_cnt2 > 100)
    {
        bcn_x2=0;
        bcn_y2=0;
    }
}
void INIT_All()
{
              //得到时钟常数
    car_dir = 0;        //运动方向为正向
    ParaInit();
    //IMAGE_CreatPoint(); //128*50创建预设点
    PID_PositionInit(&pid_position);    //方向环pid初始化
    PID_PositionInit(&pid_curve);       //丢灯转弯pid初始化
    PID_PositionInit(&pid_z);       //丢灯转弯pid初始化
    PID_MotorInit(&pid_motor_1);        //电机PID初始化
    PID_MotorInit(&pid_motor_2);        //电机PID初始化
    PID_MotorInit(&pid_motor_3);        //电机PID初始化
    PID_MotorInit(&pid_motor_4);        //电机PID初始化
    OLED_Init();        //OLED初始化
    FLASH_Init();                           //FLASH初始化
    //FLASH_Write();
    FLASH_Read();                           //读FLASH，写入参数
    key_init(KEY_MAX);                      //按键初始化
    MOTOR_Init();                           //电机初始化
    ENCODE_Init();                          //编码器初始化
    BLT_UartInit();                         //蓝牙初始化
    MPU6050_Init();                         //6050
    BEEP_Init();                            //蜂鸣器
    INFRARED_Init();//红外盘
    OPENMV_Init();
    CRASH_KeyInit();
    pit_init_ms(pit3,10);
    enable_irq(PIT3_IRQn);
}

void ParaInit()
{
    is_fuzzy = 18;
    offset_center = 5;
    force_dir = 0;
    force_turn_dir = 0;
    aim_cnt = 13;
    center_x = 150;
    center_x2 = 155;
    para_speed_down = 2;
    up_y =  86;
    up_y2 =  66;
    pd_y = 100;
    pd_y2 = 90;
    offset_add = 0; 
    is_rush = 1;
    cnt_derta_y = 0.3;
    
    barrier_vx = 990;
    barrier_vy = 0;
    barrier_x_minus=100;
    barrier_must_y=105;
    barrier_must_y2=80;
    down_y = 80;            //第一个减速点
    down_y2 =60;
    ahead_down_thd=450;     //减速阈值
    barrier_delay=20;
    crash_key_en = 1;
    
    
    position_zp0 = 95;
    position_zp1 = 105; 
    position_zi0 = 0; 
    position_zd0 = 30; 
    gyro_thd = 100; 
    position_cp = 100; 
    position_ci = 6; 
    position_cd = 0; 
    pvlr0 = 85; 
    pvlr1 = 2; 
    out0_limit = 15;
    out1_limit = 25;
    must_dir = 0;
    must_down = 0;
    
    def_speed = 250; 
    up_speed = 550; 
    down_speed = 140;
    motor_kp = 9500;
    motor_ki = 1000;
    motor_kd = 0;
    curve_vx = 500;
    curve_vy = 170;
    curve_vz = 430;
    curve_vy2 = 170;
    curve_vz2 = 430;
    
    back_en = 1;
    send_en = 1;
    show_en = 0;
    crash_judge_y = 3;
    crash_check_time = 85;
    gyro_kalman_q = 75;
    gyro_kalman_r = 100;
    light_pit_ms2 = 850;
    light_pit_ms = 450;
    vy_pit = 400;
}
void MotorTest(int16 duty1,int16 duty2,int16 duty3,int16 duty4)
{
    ftm_pwm_init(MOTOR_FTM, FORWARD_1, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_1,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, FORWARD_2, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_2,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, FORWARD_3, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_3,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, FORWARD_4, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_4,MOTOR_HZ,0);  
    if (duty1 >= 0)    //一号电机
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_1, duty1);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_1, 0);
    }
    else
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_1, 0);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_1, -duty1);
    }

    if (duty2 >= 0)    //二号电机
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_2, duty2);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_2, 0);
    }
    else               
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_2, 0);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_2, -duty2);
    }

    if (duty3 >= 0)    //三号电机
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_3, duty3);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_3, 0);
    }
    else
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_3, 0);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_3, -duty3);
    }

    if (duty4 >= 0)    //四号电机
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_4, duty4);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_4, 0);
    }
    else
    {
        ftm_pwm_duty(MOTOR_FTM, FORWARD_4, 0);
        ftm_pwm_duty(MOTOR_FTM, BACKWARD_4, -duty4);
    }
    while(1);
}
/*time test*/
//pit_time_start(pit2);

//time = pit_time_get(pit2)/110;//us
//pit_close(pit2);