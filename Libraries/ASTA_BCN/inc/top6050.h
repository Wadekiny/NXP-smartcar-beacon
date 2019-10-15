#ifndef _TOP6050_H_
#define _TOP6050_H_


void MPU6050_Init();
void MPU6050_GetData();

float KalmanFilter(const float ResrcData,float ProcessNiose_Q,float MeasureNoise_R);
#endif 