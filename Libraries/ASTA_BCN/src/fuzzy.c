#include "fuzzy.h"
#include "ALLBCN.h"
#define NB  (-3)
#define NM  (-2)
#define NS  (-1)
#define ZO  (0)
#define PS  (1)
#define PM  (2)
#define PB  (3)
int out_d[5] = {0,1,2,3,4}; //d输出
int out_p[6] = {0,10,20,-15,-35,-55};//p输出
int in_e3[4] = {0,50,100,150};//{0,25,50,75};  //e输入
int in_ec3[4] = {0,25,50,75};  //ec输入
int in_omega[4] = {110,150,250,350};//角速度输入
int d_rule3[3][3]=
{//  EC 0 1 2
 //E   
 /*0*/  0,1,2,
 /*1*/  0,1,1,
 /*2*/  0,0,1,
};

int p_rule[7][7] = 
{
 //  EC -3 -2 -1  0  1  2  3   
/*E-3*/  4, 3, 1, 0, 0, 0, 0,
 /*-2*/  4, 4, 2, 0, 0, 0, 0,
 /*-1*/  4, 4, 2, 0, 0, 0, 0,
 /* 0*/  4, 4, 3, 0, 3, 4, 4,
 /* 1*/  0, 0, 0, 0, 2, 4, 4,
 /* 2*/  0, 0, 0, 0, 2, 4, 4,
 /* 3*/  0, 0, 0, 0, 1, 3, 4,
};
//int p_rule[7][7] = 
//{
// //  EC -3 -2 -1  0  1  2  3   
///*E-3*/  5, 4, 3, 0, 0, 0, 0,
// /*-2*/  5, 5, 3, 0, 0, 0, 0,
// /*-1*/  5, 5, 3, 0, 0, 0, 0,
// /* 0*/  5, 5, 3, 0, 3, 5, 5,
// /* 1*/  0, 0, 0, 0, 3, 5, 5,
// /* 2*/  0, 0, 0, 0, 3, 5, 5,
// /* 3*/  0, 0, 0, 0, 3, 4, 5,
//};
float FUZZY_P(int e, int ec)
{
    int el=ZO,er=ZO,ecl=ZO,ecr=ZO;      //左右
    float efl,efr,ecfl,ecfr; //隶属度
    float out = 0;
    //ec 为陀螺仪输入的话 信标灯右移 ec为正
    //e大于0时 cal_p算出顺时针 如果ec也大于0 需要抑制p的作用 同号抑制 异号加强（不变?）
    if (e >= -in_e3[ZO] && e <= in_e3[ZO])//zo
    {
        el = ZO;
        er = ZO;
        efl = efr = 0.5;
    }
    else if (e >= in_e3[ZO] && e <= in_e3[PS])//ZO PS
    {
        el = ZO;
        er = PS;
        efl = (in_e3[PS] - e) *1.0 / (in_e3[PS] - in_e3[ZO]);
		efr = (e - in_e3[ZO]) *1.0 / (in_e3[PS] - in_e3[ZO]);
    }
    else if (e >= in_e3[PS] && e <= in_e3[PM])//PS PM
    {
        el = PS;
        er = PM;
        efl = (in_e3[PM] - e) *1.0 / (in_e3[PM] - in_e3[PS]);
		efr = (e - in_e3[PS]) *1.0 / (in_e3[PM] - in_e3[PS]);
    }
    else if (e >= in_e3[PM] && e <=in_e3[PB])//PM PB
    {
        el = PM;
        er = PB;
        efl = (in_e3[PB] - e) *1.0 / (in_e3[PB] - in_e3[PM]);
		efr = (e - in_e3[PM]) *1.0 / (in_e3[PB] - in_e3[PM]);
    }
    else if (e >= in_e3[PB]) //PB PB
    {
        el = PB;
        er = PB;
        efl = efr = 0.5;
    }
    else if (e <= -in_e3[ZO] && e >= -in_e3[PS])//NS ZO//负 有点变化
    {
        el = NS;
        er = ZO;
        efr = (in_e3[PS] + e) *1.0 / (in_e3[PS] - in_e3[ZO]);
		efl = (-e - in_e3[ZO]) *1.0 / (in_e3[PS] - in_e3[ZO]);
    }
    else if (e <= -in_e3[PS] && e >= -in_e3[PM])//NM NS
    {
        el = NM;
        er = NS;
        efr = (in_e3[PM] + e) *1.0 / (in_e3[PM] - in_e3[PS]);
		efl = (-e - in_e3[PS]) *1.0 / (in_e3[PM] - in_e3[PS]);
    }
    else if (e <= -in_e3[PM] && e >= -in_e3[PB])//NB NM
    {
        el = NB;
        er = NM;
        efr = (in_e3[PB] + e) *1.0 / (in_e3[PB] - in_e3[PM]);
		efl = (-e - in_e3[PM]) *1.0 / (in_e3[PB] - in_e3[PM]);
    }
    else if (e <= -in_e3[PB])
    {
        el = NB;
        er = NB;
        efr = efl = 0.5;
    }
    
    //ec
    if (ec >= -in_omega[ZO] && ec <= in_omega[ZO])//zo
    {
        ecl = ZO;
        ecr = ZO;
        ecfl = ecfr = 0.5;
    }
    else if (ec >= in_omega[ZO] && ec <= in_omega[PS])//ZO PS
    {
        ecl = ZO;
        ecr = PS;
        ecfl = (in_omega[PS] - ec) *1.0 / (in_omega[PS] - in_omega[ZO]);
		ecfr = (ec - in_omega[ZO]) *1.0 / (in_omega[PS] - in_omega[ZO]);
    }
    else if (ec >= in_omega[PS] && ec <= in_omega[PM])//PS PM
    {
        ecl = PS;
        ecr = PM;
        ecfl = (in_omega[PM] - ec) *1.0 / (in_omega[PM] - in_omega[PS]);
		ecfr = (ec - in_omega[PS]) *1.0 / (in_omega[PM] - in_omega[PS]);
    }
    else if (ec >= in_omega[PM] && ec <=in_omega[PB])//PM PB
    {
        ecl = PM;
        ecr = PB;
        ecfl = (in_omega[PB] - ec) *1.0 / (in_omega[PB] - in_omega[PM]);
		ecfr = (ec - in_omega[PM]) *1.0 / (in_omega[PB] - in_omega[PM]);
    }
    else if (ec >= in_omega[PB]) //PB PB
    {
        ecl = PB;
        ecr = PB;
        ecfl = ecfr = 0.5;
    }
    else if (ec <= -in_omega[ZO] && ec >= -in_omega[PS])//NS ZO//负 有点变化
    {
        ecl = NS;
        ecr = ZO;
        ecfr = (in_omega[PS] + ec) *1.0 / (in_omega[PS] - in_omega[ZO]);
		ecfl = (-ec - in_omega[ZO]) *1.0 / (in_omega[PS] - in_omega[ZO]);
    }
    else if (ec <= -in_omega[PS] && ec >= -in_omega[PM])//NM NS
    {
        ecl = NM;
        ecr = NS;
        ecfr = (in_omega[PM] + ec) *1.0 / (in_omega[PM] - in_omega[PS]);
		ecfl = (-ec - in_omega[PS]) *1.0 / (in_omega[PM] - in_omega[PS]);
    }
    else if (ec <= -in_omega[PM] && ec >= -in_omega[PB])//NB NM
    {
        ecl = NB;
        ecr = NM;
        ecfr = (in_omega[PB] + ec) *1.0 / (in_omega[PB] - in_omega[PM]);
		ecfl = (-ec - in_omega[PM]) *1.0 / (in_omega[PB] - in_omega[PM]);
    }
    else if (ec <= -in_omega[PB])
    {
        ecl = NB;
        ecr = NB;
        ecfr = ecfl = 0.5;
    }
    
    out = out_d[p_rule[el+3][ecl+3]]*is_fuzzy * efl * ecfl
        + out_d[p_rule[el+3][ecr+3]]*is_fuzzy * efl * ecfr
		+ out_d[p_rule[er+3][ecl+3]]*is_fuzzy * efr * ecfl
		+ out_d[p_rule[er+3][ecr+3]]*is_fuzzy * efr * ecfr;
    return out;
}





///fuzzy_d
//float FUZZY_d3(int e,int ec,int d_rule3[3][3])
//{
//	float out_d;                //输出    
//	float e_fl,e_fr,ec_fl,ec_fr;//隶属度
//	int e_l,e_r,ec_l,ec_r;		//位置
//	//E
//    if (e < 0) e = -e;
//    if (ec < 0) ec = -ec;
//    if (e == in_e3[0])
//    {
//        e_l = 0;
//        e_r = 0;
//        e_fl = 0.5;
//        e_fr = 0.5;
//    }
//	else if (e <= in_e3[1])
//	{
//		e_l = 0;
//		e_r = 1;
//		e_fl = (in_e3[1] - e) *1.0 / (in_e3[1] - in_e3[0]);
//		e_fr = (e - in_e3[0]) *1.0 / (in_e3[1] - in_e3[0]);
//	}
//	else if (e <= in_e3[2])
//	{
//		e_l = 1;
//		e_r = 2;
//		e_fl = (in_e3[2] - e) *1.0 / (in_e3[2] - in_e3[1]);
//		e_fr = (e - in_e3[1]) *1.0 / (in_e3[2] - in_e3[1]);
//	}
//	else
//	{
//        e_l = 2;
//		e_r = 2;
//		e_fl = 0.5;
//		e_fr = 0.5;
//	}
//
//	//EC
//	if (ec == in_ec3[0])
//    {
//        ec_l = 0;
//        ec_r = 0;
//        ec_fl = 0.5;
//        ec_fr = 0.5;
//    }
//	else if (ec <= in_ec3[1])
//	{
//		ec_l = 0;
//		ec_r = 1;
//		ec_fl = (in_ec3[1] - ec) *1.0 / (in_ec3[1] - in_ec3[0]);
//		ec_fr = (ec - in_ec3[0]) *1.0 / (in_ec3[1] - in_ec3[0]);
//	}
//	else if (e <= in_e3[2])
//	{
//		ec_l = 1;
//		ec_r = 2;
//		ec_fl = (in_ec3[2] - ec) *1.0 / (in_ec3[2] - in_ec3[1]);
//		ec_fr = (ec - in_ec3[1]) *1.0 / (in_ec3[2] - in_ec3[1]);
//	}
//	else
//	{
//        ec_l = 2;
//        ec_r = 2;
//        ec_fl = 0.5;
//        ec_fr = 0.5;
//	}
//
//	out = out_d[d_rule3[e_l][ec_l]] * e_fl * ec_fl
//     	   +out_d[d_rule3[e_l][ec_r]] * e_fl * ec_fr
//		   +out_d[d_rule3[e_r][ec_l]] * e_fr * ec_fl
//		   +out_d[d_rule3[e_r][ec_r]] * e_fr * ec_fr;
//    return out;
//}