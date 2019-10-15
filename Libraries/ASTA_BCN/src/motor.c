#include "motor.h"
#include "ALLBCN.h"
/**************************************************************************
  * @brief        : 电机ftm初始化
  * @others       : 18KHz
***************************************************************************/
void MOTOR_Init(void)
{
    ftm_pwm_init(MOTOR_FTM, FORWARD_1, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_1,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, FORWARD_2, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_2,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, FORWARD_3, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_3,MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, FORWARD_4, MOTOR_HZ,0);
    ftm_pwm_init(MOTOR_FTM, BACKWARD_4,MOTOR_HZ,0);
   
}
/**************************************************************************
  * @brief        : 速度控制
  * @param[in]    : 四个电机的占空比
  * @others       : 更新ftm
***************************************************************************/
void MOTOR_Control(int16 duty1, int16 duty2, int16 duty3, int16 duty4)
{
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
}