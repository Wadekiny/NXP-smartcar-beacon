#include "PID.h"
#include "ALLBCN.h"
#include "math.h"
/**************************************************************************
  * @brief        : 位置pid初始化
  * @param[out]   : 初始化kp,ki,kd,err,err_1,inter,value
  * @others       : 未使用积分项
***************************************************************************/
struct sPidPosition pid_position;
struct sPidPosition pid_curve;
struct sPidPosition pid_z;

void PID_PositionInit(struct sPidPosition* pid) //位置pid初始化
{
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->err = 0;
    pid->err_1 = 0;
    pid->ec = 0;
    pid->inter = 0;
    pid->value = 0;
}

int16 position_cp=50,position_ci=6,position_cd=0;
float PID_Curve(struct sPidPosition* pid, float set, int act)
{
    float pid_temp;
    float curve_p=0,curve_d=0;
    float curve_i=0;
    
    pid->err_1 = pid->err;
    pid->err = set-act;	
    pid->ec = pid->err - pid->err_1;
    if (ALGO_Fabs(pid->err) < 400) pid->inter += pid->err; 
    if (pid->inter > 1000)  pid->inter = 1000;
    if (pid->inter < -1000) pid->inter = -1000;
    pid->kp = (float)position_cp;
    pid->ki = (float)position_ci;
    pid->kd = (float)position_cd;

    pid->kp /= 100;
    pid->ki /= 100;
    pid->kd /= 100;
    curve_p = pid->kp * pid->err;
    curve_i = pid->ki * pid->inter;
    curve_d = pid->kd *pid->ec;
    
    
    pid_temp = curve_p + curve_i + curve_d;
 
    if (pid_temp > 200) pid_temp = 200;
    if (pid_temp < -200) pid_temp = -200;
    
    pid->value = pid_temp;
	return pid->value;
}   

float PID_ZCal(struct sPidPosition* pid, float set, float act)
{
    float pid_temp;
    
    pid->err_1 = pid->err;
    pid->err = set-act;	
    pid->ec = pid->err - pid->err_1;

    pid->kp = (float)pvlr0;
    pid->ki = (float)0;
    pid->kd = (float)pvlr1;

//    pid->kp /= 100;
//    pid->ki /= 100;
//    pid->kd /= 100;
//    curve_p = pid->kp * pid->err;
//    curve_i = pid->ki * pid->inter;
//    curve_d = pid->kd *pid->ec;
    
    
    pid_temp = pid->kp*pid->err + pid->kd*pid->ec;
 
    if (pid_temp > BUCHANG_LIMIT) pid_temp = BUCHANG_LIMIT;
    if (pid_temp < -BUCHANG_LIMIT) pid_temp = -BUCHANG_LIMIT;
    
    pid->value = pid_temp;
	return pid->value;
}


/**************************************************************************
  * @brief        : 位置pid计算
  * @param[in]    : 舵机pid结构体，信标x坐标设定值，信标x坐标实际值
  * @param[out]   : 舵机Pid结构体err,err_1,inter,value更新
  * @return       : pid->value
  * @others       : set设定位置，act实际位置
 kp =9.8 ;i=d=0  error = 40  value = 9.8*40 = 392 392/100 = 3.92
***************************************************************************/

int16 position_zp0=70;//低速p
int16 position_zp1=80;//高速p
int16 position_zi0 = 1;//i
int16 position_zd0=14;// d后两个没用

int16 pvlr1=24,pvlr0=14;//z差速补偿d，z差速补偿p


float out0_limit=15,out1_limit=15;//p out 


float cal_p=0;
float cal_i=0;
float cal_d=0;
float PID_PositionCal(struct sPidPosition* pid, float set, int act)
{
  	float pid_temp;
    //err更新
    pid->err_1 = pid->err;
    pid->err = set-act;	
    pid->ec = pid->err - pid->err_1;

    /*积分项*/
    if (ALGO_Fabs(pid->err) < 30 && gyro_z < 150)//积分限制
        pid->inter += pid->err;
    if (pid->inter > 200) pid->inter = 200;
    if (pid->inter < -200)pid->inter = -200;
    /*积分项*/
    
    //得到pid参数值
    if (wheel.vy >= def_speed)
    {
        pid->kp = (float)position_zp1;//+ FUZZY_P((int)pid->err,(int16)gyro_z);
    }
    else
    {
        pid->kp = (float)position_zp0;//+ FUZZY_P((int)pid->err,(int16)gyro_z);
    }
    pid->ki = (float)position_zi0;
    if (is_fuzzy == 0)
        pid->kd = (float)position_zd0;
    else
        pid->kd = (float)position_zd0+ FUZZY_P((int)pid->err,(int16)gyro_z);
    

    pid->kp /= 1000;
    pid->ki /= 1000;
    pid->kd /= 1000;
 
    cal_p = pid->kp * pid->err ;
    cal_i = pid->ki * pid->inter;
    cal_d = pid->kd * (-gyro_z);//用陀螺仪的值带入D
    
    //p限幅
    if (cal_p > out0_limit) cal_p = out0_limit;
    if (cal_p < -out0_limit) cal_p = -out0_limit;
    
    pid_temp = cal_p + cal_i + cal_d;
    //pid输出先复
    if (pid_temp > out1_limit) pid_temp = out1_limit;
    if (pid_temp < -out1_limit) pid_temp = -out1_limit;

    pid->value = pid_temp;

	return pid->value;
}   
//float PID_XCal(struct sPidPosition* pid, float set, int act)
//{
//  	float pid_temp;
//    float cal_p=0;
//    float cal_d=0;
//
//    pid->err_1 = pid->err;
//    pid->err = set-act;		
//    
//    pid->kp = position_xp0;
//    pid->kd = position_xd0;
//    cal_p = pid->kp * pid->err*10 ;
//    cal_d = pid->kd *(pid->err - pid->err_1)*10;
//    
//    pid_temp = cal_p + cal_d;
//    
//    pid->value = pid_temp;
//	return pid->value;
//}  

/**************************************************************************
  * @brief        : 电机增量pid初始化
  * @param[out]   : 初始化kp,ki,kd,err,err_1,err_2,value
  * @others       : value为int型
***************************************************************************/
struct sPidMotor pid_motor_1;   //四个电机的pid结构体
struct sPidMotor pid_motor_2;
struct sPidMotor pid_motor_3;
struct sPidMotor pid_motor_4;
int16 motor_kp=5000;
int16 motor_ki=400;
int16 motor_kd=50;
void PID_MotorInit(struct sPidMotor *pid)
{
    pid->s_kp = 0;
    pid->s_ki = 0;
    pid->s_kd = 0;
    pid->c_kp = 0;
    pid->c_ki = 0;
    pid->c_kd = 0;
    pid->err = 0;
    pid->err_1 = 0;
    pid->err_2 = 0;
    pid->value = 0;
}

/**************************************************************************
  * @brief        : 增量Pid计算
  * @param[in]    : 电机Pid结构体，设定速度，实际速度
  * @param[out]   : 电机Pid结构体三次err更新，value更新
  * @return       : pid->value
  * @others       : 输出限幅调到最大，最大输出满占空比
                    mode-0   直线
                    mode-1   弯道
                    mode决定使用s_kp还是c_kp
//因为是10000上限，所以不/100 设高速调阈值，高于阈值kp*2
***************************************************************************/
float PID_MotorCal(struct sPidMotor* pid, int set, int act, uint8 mode)
{
    float pid_temp = 0;
    pid->err_2 = pid->err_1;
    pid->err_1 = pid->err;
    pid->err = set-act;
    
    pid->s_kp = (float)motor_kp*1.0/100;
    pid->s_ki = (float)motor_ki*1.0/100;
    pid->s_kd = (float)motor_kd*1.0/100;


    pid_temp = pid->s_kp * (pid->err - pid->err_1)
             + pid->s_ki *  pid->err
             + pid->s_kd * (pid->err - 2*pid->err_1 + pid->err_2);

    pid->value += pid_temp;
    if (pid->value > (float)MOTOR_MAX) pid->value = MOTOR_MAX;
    if (pid->value < (float)MOTOR_MIN) pid->value = MOTOR_MIN;

   
    return pid->value;
}

int16 value1,value2,value3,value4;
void PID_MotorControl(int set1,int set2, int set3, int set4, uint8 mode)
{
    //前后切换电机输出要变
    if (mode == DIR0)
    {
        PID_MotorCal(&pid_motor_1, set1, act_speed1, mode);//pid计算
        PID_MotorCal(&pid_motor_2, set2, act_speed2, mode);
        PID_MotorCal(&pid_motor_3, set3, act_speed3, mode);
        PID_MotorCal(&pid_motor_4, set4, act_speed4, mode);
    }
    else
    {
        PID_MotorCal(&pid_motor_3, -set1, -act_speed1, mode);//pid计算
        PID_MotorCal(&pid_motor_4, -set2, -act_speed2, mode);
        PID_MotorCal(&pid_motor_1, -set3, -act_speed3, mode);
        PID_MotorCal(&pid_motor_2, -set4, -act_speed4, mode);
    }
    
    if (car_dir == FRONT_DIR)
    {
        value1 = (int16)(pid_motor_1.value+0.5+pwm_buchang+x_buchang_r);
        value2 = (int16)(pid_motor_2.value+0.5-pwm_buchang+x_buchang_l);
        value3 = (int16)(pid_motor_3.value+0.5-pwm_buchang-x_buchang_l);
        value4 = (int16)(pid_motor_4.value+0.5+pwm_buchang-x_buchang_r);
    }
    else
    {
        value1 = (int16)(pid_motor_1.value+0.5+pwm_buchang+x_buchang_l);
        value2 = (int16)(pid_motor_2.value+0.5-pwm_buchang+x_buchang_r);
        value3 = (int16)(pid_motor_3.value+0.5-pwm_buchang-x_buchang_r);
        value4 = (int16)(pid_motor_4.value+0.5+pwm_buchang-x_buchang_l);
    }
    if (value1 > MOTOR_MAX) value1 = MOTOR_MAX;
    if (value1 < MOTOR_MIN) value1 = MOTOR_MIN;
    if  (value2 > MOTOR_MAX) value2 = MOTOR_MAX;
    if (value2 < MOTOR_MIN) value2 = MOTOR_MIN;
    if (value3 > MOTOR_MAX) value3 = MOTOR_MAX;
    if (value3 < MOTOR_MIN) value3 = MOTOR_MIN;
    if (value4 > MOTOR_MAX) value4 = MOTOR_MAX;
    if (value4 < MOTOR_MIN) value4 = MOTOR_MIN;
    
    MOTOR_Control(value1, value2, value3, value4);
}