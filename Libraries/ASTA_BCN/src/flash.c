#include "flash.h"
#include "ALLBCN.h"
#include "headfile.h"

/**************************************************************************
  * @brief        : 参数写入flash
  * @others       : 偏移量为8
***************************************************************************/

int16 parameter_float[23] = {0};

void FLASH_Write()
{
    int i;
    
    FLASH_EraseSector(FLASH_SECTOR_STORE);

    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&back_en,1 ,FLASH_OFFSET*0);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&show_en,1 ,FLASH_OFFSET*1);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&send_en,1 ,FLASH_OFFSET*2);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&angle_y,1 ,FLASH_OFFSET*3);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&is_fuzzy,1 ,FLASH_OFFSET*4);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&offset_center,1 ,FLASH_OFFSET*5);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&force_dir,1 ,FLASH_OFFSET*6);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&up_y,1 ,FLASH_OFFSET*7);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&down_y,1 ,FLASH_OFFSET*8);
//    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&y_thd_lm,1 ,FLASH_OFFSET*9);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&aim_cnt,1 ,FLASH_OFFSET*10);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&center_x,1 ,FLASH_OFFSET*11);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&pd_y,1 ,FLASH_OFFSET*12);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&gyro_thd,1 ,FLASH_OFFSET*13);
    

    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&up_speed,  2 ,FLASH_OFFSET*14);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&down_speed,2 ,FLASH_OFFSET*15);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&def_speed, 2 ,FLASH_OFFSET*16);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&curve_vx,  2 ,FLASH_OFFSET*17);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&curve_vy,  2 ,FLASH_OFFSET*18);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&light_pit_ms,2 ,FLASH_OFFSET*19);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&vy_pit,2 ,FLASH_OFFSET*20);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&barrier_vx,2 ,FLASH_OFFSET*21);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&barrier_x_minus,2 ,FLASH_OFFSET*22);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&barrier_must_y,2 ,FLASH_OFFSET*23);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&barrier_must_y2,2 ,FLASH_OFFSET*24);
    
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&light_pit_ms2,2 ,FLASH_OFFSET*25);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&barrier_delay,2 ,FLASH_OFFSET*26);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&crash_key_en,1 ,FLASH_OFFSET*27);
    

//    parameter_float[0] = (int16)(barrier_vy*100);
//    parameter_float[1] = (int16)(position_ki*100);
//    parameter_float[2] = (int16)(position_kd*100);
//    parameter_float[3] = (int16)(motor_kp*100);
//    parameter_float[4] = (int16)(motor_ki*100);
//    parameter_float[5] = (int16)(motor_kd*100);
//    parameter_float[6] = (int16)(curve_kp*100);
//    parameter_float[7] = (int16)(curve_ki*100);
//    parameter_float[8] = (int16)(curve_kd*100);
   // parameter_float[9] = (int16)(curve_vz*100);
//    parameter_float[10]= (int16)(position_zp0*100);
//    parameter_float[11]= (int16)(position_zd0*100);
//    parameter_float[12]= (int16)(position_xp0*100);
//    parameter_float[13]= (int16)(position_xd0*100);
//    parameter_float[14]= (int16)(position_zp1*100);
//    parameter_float[15]= (int16)(position_zd1*100);
    parameter_float[16]= (int16)(barrier_vy*100);
    parameter_float[17]= (int16)(cnt_derta_y*100);
    parameter_float[18]= (int16)(out0_limit*100);
    parameter_float[19]= (int16)(out1_limit*100);
    parameter_float[20]= (int16)(para_speed_down*100);
//    parameter_float[21]= (int16)(z_para*100);
//    parameter_float[22]= (int16)(p_para*100);

    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+16),2 ,FLASH_OFFSET*35);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+17),2 ,FLASH_OFFSET*36);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+18),2 ,FLASH_OFFSET*37);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+19),2 ,FLASH_OFFSET*38);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+20),2 ,FLASH_OFFSET*39);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+21),2 ,FLASH_OFFSET*40);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(parameter_float+22),2 ,FLASH_OFFSET*41);
    
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_zp0,  2 ,FLASH_OFFSET*42);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_zd0,  2 ,FLASH_OFFSET*43);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_zi0,  2 ,FLASH_OFFSET*44);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&pvlr0,  2 ,FLASH_OFFSET*45);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_cp,  2 ,FLASH_OFFSET*46);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_ci,  2 ,FLASH_OFFSET*47);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_cd,  2 ,FLASH_OFFSET*48);
   // FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_zd2,  2 ,FLASH_OFFSET*49);
    
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&motor_kp,  2 ,FLASH_OFFSET*50);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&motor_ki,  2 ,FLASH_OFFSET*51);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&motor_kd,  2 ,FLASH_OFFSET*52);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&crash_judge_y,  1 ,FLASH_OFFSET*53);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&crash_check_time,2 ,FLASH_OFFSET*54);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&curve_vz,2 ,FLASH_OFFSET*55);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&gyro_kalman_q,2 ,FLASH_OFFSET*56);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&gyro_kalman_r,2 ,FLASH_OFFSET*57);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&beep_en,1 ,FLASH_OFFSET*58);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&position_zp1,2 ,FLASH_OFFSET*59);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&pvlr1,2 ,FLASH_OFFSET*60);
    for (i=0;i<30;i++)
    {
        FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(cut_dir+i),1 ,FLASH_OFFSET*61+i);
    }
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&center_x,2,FLASH_OFFSET*91);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&pd_y2,   2,FLASH_OFFSET*92);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&up_y2,   2,FLASH_OFFSET*93);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&down_y2, 2,FLASH_OFFSET*94);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&curve_vy2, 2,FLASH_OFFSET*95);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&curve_vz2, 2,FLASH_OFFSET*96);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&force_turn_dir, 1,FLASH_OFFSET*97);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&offset_add, 1,FLASH_OFFSET*98);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&is_rush, 2,FLASH_OFFSET*99);
    for (i=0;i<15;i++)
    {
        FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(force_car_dir+i),1 ,FLASH_OFFSET*100+i);
    }
    for (i=0;i<15;i++)
    {
        FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)(force_speed_down+i),1 ,FLASH_OFFSET*115+i);
    }
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&must_dir, 2,FLASH_OFFSET*130);
    FLASH_WriteSector(FLASH_SECTOR_STORE,(uint8 *)&must_down, 2,FLASH_OFFSET*131);
}
void FLASH_Read()
{
    uint8 i;
    back_en = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*0,uint8);
    show_en = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*1,uint8);
    send_en = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*2,uint8);
    angle_y = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*3,uint8);
    is_fuzzy = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*4,uint8);
    offset_center = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*5,uint8);
    force_dir = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*6,uint8);
    up_y = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*7,uint8);
    down_y = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*8,uint8);
//    y_thd_lm = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*9,uint8);
    aim_cnt = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*10,uint8);
    center_x = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*11,uint8);
    pd_y = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*12,uint8);
    gyro_thd = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*13,uint8);
    
    up_speed = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*14,int16);
    down_speed = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*15,int16);
    def_speed = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*16,int16);
    curve_vx = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*17,int16);
    curve_vy = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*18,int16);
    light_pit_ms = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*19,uint16);
    vy_pit = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*20,int16);
    barrier_vx = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*21,uint16);
    barrier_x_minus = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*22,uint16);
    barrier_must_y = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*23,uint16);
    barrier_must_y2 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*24,uint16);
   
    light_pit_ms2 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*25,uint16);
    barrier_delay = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*26,uint16);
    crash_key_en = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*27,uint8);
    barrier_vy = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*35,int16))/100;
    cnt_derta_y = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*36,int16))/100;
    out0_limit = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*37,int16))/100;
    out1_limit = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*38,int16))/100;
    para_speed_down = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*39,int16))/100;
//    z_para = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*40,int16))/100;
//    p_para = ((float)flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*41,int16))/100;
    position_zp0 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*42,int16);
    position_zd0 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*43,int16);
    position_zi0 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*44,int16);
    pvlr0 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*45,int16);
    position_cp = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*46,int16);
    position_ci = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*47,int16);
    position_cd = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*48,int16);
 //   position_zd2 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*49,int16);
    
    motor_kp = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*50,int16);
    motor_ki = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*51,int16);
    motor_kd = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*52,int16);
    
    crash_judge_y    = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*53,uint8);
    crash_check_time = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*54,uint16);
    curve_vz = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*55,int16);
    gyro_kalman_q = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*56,int16);
    gyro_kalman_r = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*57,int16);
    beep_en = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*58,uint8);
    position_zp1 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*59,int16);
    pvlr1 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*60,int16);
    for (i=0;i<30;i++)
    {
        cut_dir[i] = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*61+i,uint8);
    }
    center_x = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*91,uint16);
    pd_y2 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*92,uint16);
    up_y2 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*93,uint16);
    down_y2 = flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*94,uint16);
    curve_vy2=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*95,int16);
    curve_vz2=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*96,int16);
    force_turn_dir=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*97,uint8);
    offset_add=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*98,int8);
    is_rush=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*99,uint16);
    for (i=0;i<15;i++)
    {
        force_car_dir[i]= flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*100+i,uint8);
    }
    for (i=0;i<15;i++)
    {
        force_speed_down[i]= flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*115+i,uint8);
    }
    must_dir=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*130,uint8);
    must_down=flash_read(FLASH_SECTOR_STORE,FLASH_OFFSET*131,uint8);
}
