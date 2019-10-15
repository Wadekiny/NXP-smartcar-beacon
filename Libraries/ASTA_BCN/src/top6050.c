#include "top6050.h"

#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "common.h"
#include "headfile.h"
#include "ALLBCN.h"
//q80 r350
//原始数据
int16 gyrox=0,gyroy=0,gyroz=0,accx=0,accy=0,accz=0;
//处理后数据
float acc_x=0,acc_y=0,acc_z=0;
float gyro_x=0,gyro_y=0,gyro_z=0;

//float yuanshi=0;
float pitch=0,roll=0,yaw=0;
uint8 euler_flag=0,acc_flag=0,gyro_flag=0;//获取是否成功 //1成功0失败

int16 gyro_x_offset = -28;//-29 -28
int16 gyro_y_offset = -18; // -8 -18
int16 gyro_z_offset = -2;//  -5  -2
int16 acc_x_offset=0;
int16 acc_y_offset=0;
int16 acc_z_offset=0;

int16 gyro_kalman_q=80;
int16 gyro_kalman_r=320;
void MPU6050_AccAdjust()
{
    accx = accx - acc_x_offset;
    accy = accy - acc_y_offset;
    accz = accz - acc_z_offset;
}
void MPU6050_GyroAdjust()
{
    gyrox = gyrox - gyro_x_offset;
    gyroy = gyroy - gyro_y_offset;
    gyroz = gyroz - gyro_z_offset;
}

void MPU6050_Init()
{
    MPU_Init();
   // while(mpu_dmp_init());
}



//加速度计 2g    32768/2 = 16384   加速度原始数据/4095.875(g)
//陀螺仪2000dps  32767/2000=16.38   陀螺仪原始数据/16.3835 (°/s)
void MPU6050_GetData()
{
//    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)  //欧拉角
//    {
//        euler_flag = 1;
//    }
//    else
//    {
//        euler_flag = 0;
//    }
//    if (MPU_Get_Accelerometer(&accx,&accy,&accz) == 0)//加速度计
//    {
//        acc_flag = 1;
//        MPU6050_AccAdjust();    //消除误差
//        acc_x = accy/16384*9.8;
//        acc_y = accx/16384*9.8;
//        acc_z = accz/16384*9.8;
//    }
//    else
//    {
//        acc_flag = 0;
//    }
    if (MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz) == 0)//陀螺仪
    {
        gyro_flag = 1;
        MPU6050_GyroAdjust();
        if (gyro_z > 1500) gyro_z = 1500;
        else if (gyro_z < -1500) gyro_z = -1500;
        gyro_x = gyroy/16.38;
        gyro_y = gyrox/16.38;
        gyro_z = gyroz/16.38;
        //yuanshi = gyro_z;
        //q  0.8   r 0.32
        gyro_z = KalmanFilter(gyro_z,gyro_kalman_q*1.0/1000,gyro_kalman_r*1.0/1000);
    }
    else
    {
        gyro_flag = 0;
    }
}




#define KALMAN_Q 0.02	//过程噪声0.01  0.01 80
#define KALMAN_R 0.444	//测量噪声0.5   0.3  350

float KalmanFilter(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
{

    float R = MeasureNoise_R;
    float Q = ProcessNiose_Q;

    static float x_last;
    float x_mid = x_last;
    float x_now;

    static float p_last;
    float p_mid ;
    float p_now;

    float kg;

    x_mid=x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

    /*
     *  卡尔曼滤波的五个重要公式
     */
    kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为噪声
    x_now= (1-kg) * x_mid+kg * ResrcData;   //估计出的最优值
    p_now= (1-kg) * p_mid;                 //最优值对应的covariance
    p_last = p_now;                     //更新covariance 值
    x_last = x_now;                     //更新系统状态值

    return x_now;

}