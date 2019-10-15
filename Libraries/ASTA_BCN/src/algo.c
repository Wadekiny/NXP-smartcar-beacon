#include "algo.h"

#define KALMAN_Q 0.02	//过程噪声0.01  0.01
#define KALMAN_R 0.444	//测量噪声0.5   0.3
//只能被同一个变量调用
float ALGO_KalmanFilter(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
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
       卡尔曼滤波的五个重要公式
    */
//    if (ResrcData == 0 || x_last == 0)  //特殊情况 卡尔曼增益为1 滤坐标 效果不好 坐标不用滤波
//        kg = 1;
//    else 
        kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为噪声
    x_now= (1-kg) * x_mid + kg * ResrcData;   //估计出的最优值
    p_now= (1-kg) * p_mid;                 //最优值对应的covariance
    p_last = p_now;                     //更新covariance 值
    x_last = x_now;                     //更新系统状态值

    return x_now;
}

float ALGO_KalmanSpeed(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R)
{

    float R = MeasureNoise_R;
    float Q = ProcessNiose_Q;

    static float speed_last;
    float speed_mid = speed_last;
    float speed_now;

    static float p_last;
    float p_mid ;
    float p_now;

    float kg;

    speed_mid=speed_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

    /*
       卡尔曼滤波的五个重要公式
    */
//    if (ResrcData == 0 || x_last == 0)  //特殊情况 卡尔曼增益为1 滤坐标 效果不好 坐标不用滤波
//        kg = 1;
//    else 
        kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为噪声
    speed_now= (1-kg) * speed_mid + kg * ResrcData;   //估计出的最优值
    p_now= (1-kg) * p_mid;                 //最优值对应的covariance
    p_last = p_now;                     //更新covariance 值
    speed_last = speed_now;                     //更新系统状态值

    return speed_now;
}
float ALGO_Fabs(float num)
{
    if (num < 0) return -num;
    return num;
}

int ALGO_Abs(int num)
{
    if (num < 0) return -num;
    return num;
}