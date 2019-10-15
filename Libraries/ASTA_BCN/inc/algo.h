#ifndef _ALGO_H_
#define _ALGO_H_


float ALGO_KalmanFilter(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);
float ALGO_Fabs(float num);
int ALGO_Abs(int num);
float ALGO_KalmanSpeed(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);


#endif