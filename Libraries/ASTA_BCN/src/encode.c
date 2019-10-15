#include "encode.h"
#include "ALLBCN.h"

/**************************************************************************
  * @brief        : 编码器ftm初始化
  * @others       : 20ms测速一次，电机pid控制
***************************************************************************/
void ENCODE_Init(void)
{
    ftm_quad_init(ftm1);      //FTM1 正交解码初始化    
    ftm_quad_init(ftm2);      //FTM2 正交解码初始化  
    TPM_PulseInit();
    gpio_init(TPM_DIR1, GPI, 0);
    gpio_init(TPM_DIR2, GPI, 0);
    pit_init_ms(pit0,10); 
    //set_irq_priority(PIT0_IRQn,2);						//设置优先级,根据自己的需求设
	enable_irq(PIT0_IRQn);								//打开pit0的中断开关
	//EnableInterrupts;		
    //set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);
   
}
   
/**************************************************************************
  * @brief        : 读取速度
  * @param[out]   : 指针传值得到两轮速度（脉冲数）
  * @others       : 一轮直接读取，另一轮取负
***************************************************************************/

void ENCODE_Read(int16* act_speed1,int16* act_speed2, int16* act_speed3, int16* act_speed4)
{
    *act_speed3 = ftm_quad_get(ftm1);
    ftm_quad_clean(ftm1);
    *act_speed4 = -ftm_quad_get(ftm2); 
    ftm_quad_clean(ftm2);

    if (gpio_get(TPM_DIR1) == 0)   *act_speed1 = TPM_Read(tpm1);
    else                *act_speed1 = -TPM_Read(tpm1);
    TPM_Clear(tpm1);
    if (gpio_get(TPM_DIR2) == 1)   *act_speed2 = TPM_Read(tpm2);
    else                *act_speed2 = -TPM_Read(tpm2);
    TPM_Clear(tpm2);
}

/**************************************************************************
  * @brief        : 脉冲速度转换成cm/s
  * @param[out]   : 指针传值，速度转换
  * @others       : int型
***************************************************************************/
void ENCODE_SpeedCm(int16* act_speed1, int16* act_speed2, int16* act_speed3, int16* act_speed4)
{               
    *act_speed1 = (int16)((*act_speed1)*100*1.0/WHEEL_PULSE*WHEEL_C/10);    //cm/s
    *act_speed2 = (int16)((*act_speed2)*100*1.0/WHEEL_PULSE*WHEEL_C/10);    //cm/s
    *act_speed3 = (int16)((*act_speed3)*100*1.0/WHEEL_PULSE*WHEEL_C/10);    //cm/s
    *act_speed4 = (int16)((*act_speed4)*100*1.0/WHEEL_PULSE*WHEEL_C/10);    //cm/s
}