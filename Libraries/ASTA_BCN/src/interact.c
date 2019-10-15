#include "interact.h"
#include "ALLBCN.h"

enum SIGx interact_signal = NONESIG;
enum PAGEx interact_page = MAIN_PAGE;

uint8 parameter_en = 0;
uint8 show_en = 0;
enum SIGx MENU_GetSig()  //返回按键消息，更改interact_signal
{
    enum SIGx signal = NONESIG;
    KEY_MSG_t keymsg;
    uint8 key_flag = 0;
    key_IRQHandler();
    key_flag=get_key_msg(&keymsg);
    if (key_flag)
    {   
        if (keymsg.key == KEY_L && keymsg.status == KEY_DOWN)
        {
           // signal = OUTSIG;
            signal = LEFTSIG;
        }
        else if (keymsg.key == KEY_R && (keymsg.status == KEY_DOWN))
        {
            signal = RIGHTSIG;
        }
        else if (keymsg.key == KEY_A && keymsg.status == KEY_DOWN)
        {
            signal = ENTERSIG;
        }
        else if (keymsg.key == KEY_U && (keymsg.status == KEY_DOWN || keymsg.status == KEY_HOLD))
        {
            signal = UPSIG;
        }
        else if (keymsg.key == KEY_D && (keymsg.status == KEY_DOWN || keymsg.status == KEY_HOLD))
        {
            signal = DOWNSIG;
        }
        else if (keymsg.key == KEY_B && keymsg.status == KEY_DOWN)
        {
            signal = OUTSIG;
        }
    }
    interact_signal = signal;
    return signal;
}

void MENU_SigHandling()
{
    if (interact_signal == ENTERSIG)
    {
        parameter_en = !parameter_en;
    }
    else if (interact_signal == LEFTSIG)
    {
        OLED_Clear();
        if (interact_page == MAIN_PAGE) 
            interact_page = (enum PAGEx)(MAX_PAGE-1);
        else 
            interact_page--;
        parameter_en = 0;
    }
    else if (interact_signal == RIGHTSIG)
    {   
        OLED_Clear();
        if (interact_page == (enum PAGEx)(MAX_PAGE-1)) 
            interact_page = MAIN_PAGE;
        else 
            interact_page++;
        parameter_en = 0;
    }
}

extern uint32 time;
void MENU_Display()
{
    switch (interact_page)
    {
        case MAIN_PAGE:  MENU_Main();   break;
        case CAMERA_PAGE: MENU_Camera(); break;
        case CAMERA2_PAGE: MENU_Camera2(); break;
        //case ARRAY_PAGE: MENU_Array(); break;
        case MOTOR_PAGE: MENU_Motor(); break;
        case POSITION_PAGE: MENU_Position(); break;
        //case CUT_PAGE:   MENU_Cut();   break;
        case GARISH_PAGE:MENU_Garish();break;
        default : MENU_Main();  break;
    }
   // OLED_Refresh_Gram();
};


void MENU_Main()
{
    if (interact_signal == OUTSIG)
    {
        start_sw = !start_sw;
    }
    if (start_sw == 1)
        OLED_ShowString_Inverse(35, 52, "STARTING", 12);
    else if (start_sw == 0)
        OLED_ShowString(35, 52, "SHUTDOWN ", 12);
        
    if (start_sw==1 && show_en == 0)
    {
        OLED_ShowNum(0,52,runtime, 5,12,0); 
        OLED_ShowNumFloat(34,18,finish_s,5,12,0);
    }
    else
    {
        OLED_ShowNum(44+5,0,red1,1,12,0);
        OLED_ShowNum(32+5,0,red2,1,12,0);
        OLED_ShowNum(20+5,12,red3,1,12,0);
        OLED_ShowNum(20+5,24,red4,1,12,0);
        OLED_ShowNum(32+5,36,red5,1,12,0);
        OLED_ShowNum(44+5,36,red6,1,12,0);
        
        OLED_ShowNum(56+5,36,red7,1,12,0);
        OLED_ShowNum(68+5,36,red8,1,12,0);
        OLED_ShowNum(80+5,24,red9,1,12,0);
        OLED_ShowNum(80+5,12,red10,1,12,0);
        OLED_ShowNum(68+5,0,red11,1,12,0);
        OLED_ShowNum(56+5,0,red12,1,12,0);
          
        OLED_ShowNum(0,52,runtime, 5,12,0); 
        OLED_ShowNumFloat(34,18,finish_s,5,12,0);
       // MENU_ShowBinary(image_binary);
       // OLED_ShowNum(0,0,master_barrier,1,12,0);
        MENU_CoordCross(bcn_x,bcn_y);
        OLED_ShowNum_Inverse(0,24,bcn_x2,3,12,0);
        OLED_ShowNum_Inverse(0,36,bcn_y2,3,12,0);
        OLED_ShowNum(110,0,barrier_x,3,12,0);
        OLED_ShowNum(110,12,barrier_y,3,12,0);
        OLED_ShowNum_Inverse(110,24,barrier_x2,3,12,0);
        OLED_ShowNum_Inverse(110,36,barrier_y2,3,12,0);
        
        
        if (car_dir == FRONT_DIR)
            OLED_ShowChar(90,FIFTH_Y,'F',12,0);
        else if (car_dir == BACK_DIR)
            OLED_ShowChar(90,FIFTH_Y,'B',12,0);
        else
            OLED_ShowChar(90,FIFTH_Y,'E',12,0);
        OLED_ShowNum(116,FIFTH_Y,light_index,2,12,0);
    }  
     
    
}

void MENU_Garish() //功能设置界面
{
    static uint8 arrow = L0;//箭头方向
    uint8 page = 0;		    //页数0-5-10
    static uint8 save_flag = 0;
    uint8 *name[10] = 	//字符串
    {
    	"BACK", "SEND", "SHOW", "CJY ", "CCT ",
        "GY-Q", "GY-R", "MS-2", "MS-1", "vLim",
    };
    
	if (parameter_en == 1) //开启参数更改
	{
		if (interact_signal == UPSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
				case L0:  back_en = 1; break;
				case L1:  send_en = 1; break;
				case L2:  show_en = 1; break;
                case L3:  crash_judge_y +=1;   break;
				case L4:  crash_check_time+=5; break;
                case L5:  gyro_kalman_q+=5;break;
                case L6:  gyro_kalman_r+=5;break;
                case L7:  light_pit_ms2 += 50; break;
                case L8:  light_pit_ms+=50;break;
                case L9:  vy_pit += 10; break;
				default:  break;
			} 
            if (crash_judge_y > 50) crash_judge_y = 0;
            if (crash_check_time>500)crash_check_time = 0;
		}
		else if (interact_signal == DOWNSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
				case L0:  back_en = 0; break;
				case L1:  send_en = 0; break;
				case L2:  show_en = 0; break;
                case L3:  if (crash_judge_y >= 1)   crash_judge_y -=1;   break;
				case L4:  if (crash_check_time >= 5)crash_check_time-=5; break;
                case L5:  if (gyro_kalman_q>=5)gyro_kalman_q-=5;break;
                case L6:  if (gyro_kalman_r>=5)gyro_kalman_r-=5;break;
                case L7:  if (light_pit_ms2 >= 50)light_pit_ms2 -= 50; break;
                case L8:  if (light_pit_ms >= 50) light_pit_ms-=50;break;
                case L9:  if (vy_pit >= 10)vy_pit-=10;break;
				default:  break;
			}
            
		}
	}
	else	//未开启参数更改
	{	
		if (interact_signal == UPSIG)
			arrow--;
		else if (interact_signal == DOWNSIG)
			arrow++;

        if (arrow== L10) arrow = L0;
		if (arrow > L10) arrow = L9;
	}
    
    OLED_ShowString(65,0,"Garish",16);
    OLED_Showint(65,40,(int16)gyro_z,4,12);
	//显示字符串
    page = arrow/5*5;
	OLED_ShowString(0, FIRST_Y,  name[page+0], 12);
	OLED_ShowString(0, SECOND_Y, name[page+1], 12);
	OLED_ShowString(0, THIRD_Y,  name[page+2], 12);
	OLED_ShowString(0, FORTH_Y,  name[page+3], 12);
	OLED_ShowString(0, FIFTH_Y,  name[page+4], 12);
//显示参数
	switch (page)
	{
		case 0: OLED_ShowNum(35, FIRST_Y,  back_en,  3, 12, 0);
				OLED_ShowNum(35, SECOND_Y, send_en,  3, 12, 0);
				OLED_ShowNum(35, THIRD_Y, show_en,   3, 12, 0);
                OLED_ShowNum(35, FORTH_Y,  crash_judge_y,  3, 12, 0);
				OLED_ShowNum(35, FIFTH_Y, crash_check_time,3, 12, 0);
				break;
        case 5: OLED_ShowNum(35, FIRST_Y,  gyro_kalman_q,  3, 12, 0);
				OLED_ShowNum(35, SECOND_Y, gyro_kalman_r,  3, 12, 0);
				OLED_ShowNum(35, THIRD_Y, light_pit_ms2/10,3, 12, 0);
                OLED_ShowNum(35, FORTH_Y, light_pit_ms/10,3, 12, 0);
				OLED_ShowNum(35, FIFTH_Y, vy_pit,3, 12, 0);
				break;
		default:break;
	}
	//反色显示字符串
	switch (arrow%5)
	{
		case L0:  OLED_ShowString_Inverse(0, FIRST_Y,  name[page+0], 12); break;
		case L1:  OLED_ShowString_Inverse(0, SECOND_Y, name[page+1], 12); break;
		case L2:  OLED_ShowString_Inverse(0, THIRD_Y,  name[page+2], 12); break;
		case L3:  OLED_ShowString_Inverse(0, FORTH_Y,  name[page+3], 12); break;
		case L4:  OLED_ShowString_Inverse(0, FIFTH_Y,  name[page+4], 12); break;
		default:  break;
	}	
	//反色参数
	if (parameter_en == 1)
	{
		switch (arrow)
		{
			case L0:  OLED_ShowNum_Inverse(35, FIRST_Y,  back_en,  3, 12, 0); break;
			case L1:  OLED_ShowNum_Inverse(35, SECOND_Y, send_en,  3, 12, 0); break;
			case L2:  OLED_ShowNum_Inverse(35, THIRD_Y, show_en,   3, 12, 0); break;
            case L3:  OLED_ShowNum_Inverse(35, FORTH_Y, crash_judge_y,   3, 12, 0); break;
			case L4:  OLED_ShowNum_Inverse(35, FIFTH_Y, crash_check_time,3, 12, 0); break;
            case L5:  OLED_ShowNum_Inverse(35, FIRST_Y, gyro_kalman_q,3, 12, 0); break;
            case L6:  OLED_ShowNum_Inverse(35, SECOND_Y, gyro_kalman_r,3, 12, 0); break;
			case L7:  OLED_ShowNum_Inverse(35, THIRD_Y,  light_pit_ms2/10,  3, 12, 0); break;
            case L8:  OLED_ShowNum_Inverse(35, FORTH_Y, light_pit_ms/10,3, 12, 0); break;
			case L9:  OLED_ShowNum_Inverse(35, FIFTH_Y,  vy_pit,  3, 12, 0); break;
            default:  break;
		}
	}
    if (save_flag == 1)
    {
       OLED_ShowChar(120,0,'*',16,1);
    }
    else
    {
        OLED_ShowChar(120,0,' ',16,1);
    }
    if (interact_signal == OUTSIG && save_flag == 1)
    {
        save_flag = 0;
        FLASH_Write();	
    }
}

void MENU_Camera()	//摄像头界面
{
    static uint8 arrow = L0;	//箭头位置
    uint8 page = 0;		//页数0-5-10
    static uint8 save_flag = 0;
    uint8 *name[15] = 	//字符串//"CenterX2", "UP-Y2   ", "PD-Y2   ", "DOWN_Y2 ", "        ",
    {
		"isFuzzy ", "offsetC ", "cutDir  ", "turnDir ", "aim-cnt ",
		"centerX ", "centerX2", "p-down-v", "UP-Y    ", "UP-Y2   ", 
        "pd_y    ", "pd_y2   ", "add-of-x", "is_rush ", "cnt-minY", //强减速
    };
	if (parameter_en == 1) //开启参数更改
	{
		if (interact_signal == UPSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
				case L0:  is_fuzzy++;  break;
				case L1:  offset_center++; break;
				case L2:  force_dir++; break;
				case L3:  force_turn_dir++; break;
				case L4:  aim_cnt++; break;
				case L5:  center_x++; break;
                case L6:  center_x2++;  break;
				case L7:  para_speed_down+=0.01; break;
				case L8:  up_y++; break;
				case L9:  up_y2++; break;
				case L10: pd_y++; break;
				case L11: pd_y2++; break;
                case L12: offset_add++;  break;
				case L13: is_rush++; break;
                case L14: cnt_derta_y+=0.01; break;
				default:  break;
			}
			center_x = center_x>250?250:center_x;
		}
		else if (interact_signal == DOWNSIG)    
		{
            save_flag = 1;
			switch (arrow)
			{            
                case L0:  if (is_fuzzy >= 1)    is_fuzzy--;    break;
                case L1:  if (offset_center >= 1)    offset_center--;   break;
                case L2:  force_dir=0;   break;
                case L3:  if (force_turn_dir >= 1)  force_turn_dir--; break;
                case L4:  if (aim_cnt >= 1)  aim_cnt--; break;
                case L5:  if (center_x >= 1) center_x--;break;
                case L6:  if (center_x2 >= 1) center_x2--;break;
                case L7:  if (para_speed_down >= 0.01) para_speed_down-=0.01;
                          else para_speed_down=0;
                          break;
                case L8:  if (up_y >= 1) up_y--;break;
                case L9:  if (up_y2 >= 1) up_y2--;break;
                case L10:  if (pd_y >= 1) pd_y--;break;
                case L11:  if (pd_y2 >= 1) pd_y2--;break;
                case L12:  offset_add--;break;
                case L13:  if (is_rush >= 1) is_rush--;break;
                case L14: if (cnt_derta_y >= 0.01) cnt_derta_y-=0.01;
                          else cnt_derta_y=0;
                          break;
				default:  break;
			}
		}
	}
	else	//未开启参数更改
	{	
		if (interact_signal == UPSIG)
        {
			arrow--;
        }
		else if (interact_signal == DOWNSIG)
			arrow++;

		if (arrow == L15) arrow = L0;
        if (arrow > L15) arrow = L14;
	}

    
	//显示字符串
    page = arrow/5*5;
	OLED_ShowString(50, FIRST_Y,  name[page+0], 12);
	OLED_ShowString(50, SECOND_Y, name[page+1], 12);
	OLED_ShowString(50, THIRD_Y,  name[page+2], 12);
	OLED_ShowString(50, FORTH_Y,  name[page+3], 12);
	OLED_ShowString(50, FIFTH_Y,  name[page+4], 12);
	//显示参数
	switch (page)
	{
		case 0: OLED_ShowNum(100, FIRST_Y,  is_fuzzy, 3, 12, 0);
				OLED_ShowNum(100, SECOND_Y, offset_center, 3, 12, 0);
				OLED_ShowNum(100, THIRD_Y,  force_dir,  3, 12, 0);
				OLED_ShowNum(100, FORTH_Y,  force_turn_dir,  3, 12, 0);
				OLED_ShowNum(100, FIFTH_Y,  aim_cnt,  3, 12, 0);
				break;
		case 5:	OLED_ShowNum(100, FIRST_Y,  center_x,   3, 12, 0);
                OLED_ShowNum(100, SECOND_Y, center_x2,   3, 12, 0);
                OLED_ShowNum(100, THIRD_Y,  (int16)(para_speed_down*100),3, 12, 0);
                OLED_ShowNum(100, FORTH_Y,  up_y,   3, 12, 0);
                OLED_ShowNum(100, FIFTH_Y,  up_y2,   3, 12, 0);
				break;
        case 10:
                OLED_ShowNum(100, FIRST_Y,  pd_y,   3, 12, 0);
                OLED_ShowNum(100, SECOND_Y, pd_y2,   3, 12, 0);
                OLED_Showint(100, THIRD_Y,  offset_add,   2, 12);
                OLED_ShowNum(100, FORTH_Y,  is_rush,   3, 12, 0);
                OLED_ShowNum(100, FIFTH_Y,  (int16)(cnt_derta_y*100),   3, 12, 0);
                break;
		default:break;
	}
	//反色显示字符串
	switch (arrow%5)
	{
		case L0:  OLED_ShowString_Inverse(50, FIRST_Y,  name[page+0], 12); break;
		case L1:  OLED_ShowString_Inverse(50, SECOND_Y, name[page+1], 12); break;
		case L2:  OLED_ShowString_Inverse(50, THIRD_Y,  name[page+2], 12); break;
		case L3:  OLED_ShowString_Inverse(50, FORTH_Y,  name[page+3], 12); break;
        case L4:  OLED_ShowString_Inverse(50, FIFTH_Y,  name[page+4], 12); break;
		default:  break;
	}	
	//反色参数
	if (parameter_en == 1)
	{
		switch (arrow)
		{
            case L0:  OLED_ShowNum_Inverse(100, FIRST_Y,  is_fuzzy, 3, 12, 0); break;
            case L1:  OLED_ShowNum_Inverse(100, SECOND_Y, offset_center, 3, 12, 0); break;
            case L2:  OLED_ShowNum_Inverse(100, THIRD_Y,  force_dir,  3, 12, 0); break;
            case L3:  OLED_ShowNum_Inverse(100, FORTH_Y,  force_turn_dir,  3, 12, 0); break;
            case L4:  OLED_ShowNum_Inverse(100, FIFTH_Y,  aim_cnt,  3, 12, 0); break;
            case L5:  OLED_ShowNum_Inverse(100, FIRST_Y,  center_x,  3, 12, 0); break;
            case L6:  OLED_ShowNum_Inverse(100, SECOND_Y,  center_x2,  3, 12, 0); break;
            case L7:  OLED_ShowNum_Inverse(100, THIRD_Y,  (int16)(para_speed_down*100),  3, 12, 0); break;
            case L8:  OLED_ShowNum_Inverse(100, FORTH_Y,  up_y,  3, 12, 0); break;
            case L9:  OLED_ShowNum_Inverse(100, FIFTH_Y,  up_y2,  3, 12, 0); break;
            case L10: OLED_ShowNum_Inverse(100, FIRST_Y,  pd_y,  3, 12, 0); break;
            case L11: OLED_ShowNum_Inverse(100, SECOND_Y,  pd_y2,  3, 12, 0); break;
            case L12: OLED_Showint_Inverse(100, THIRD_Y,  offset_add,  2, 12); break;
            case L13: OLED_ShowNum_Inverse(100, FORTH_Y,  is_rush,  3, 12, 0); break;
            case L14: OLED_ShowNum_Inverse(100, FIFTH_Y,  (int16)(cnt_derta_y*100),  3, 12, 0); break;
            default:  break;
		}
	}

    OLED_Showint(0,0,(int)value1,5,12);
    OLED_Showint(0,12,(int)value2,5,12);
    OLED_Showint(0,24,(int)value3,5,12);
    OLED_Showint(0,36,(int)value4,5,12);
    //OLED_Showint(0,48,(int)pwm_buchang,4,12);
    OLED_ShowNum(0,48,crashkey_state,4,12,1);
    OLED_ShowString(0,0,"CA1",16);
    if (save_flag == 1)
    {
       OLED_ShowChar(120,0,'*',12,1);
    }
    else
    {
        OLED_ShowChar(120,0,' ',12,1);
    }
    if (interact_signal == OUTSIG && save_flag == 1)
    {
        save_flag = 0;
        FLASH_Write();
    }
    		
}

//
void MENU_Camera2()
{
    static uint8 arrow = L0;	//箭头位置
    uint8 page = 0;		//页数0-5-10
    static uint8 save_flag = 0;
    uint8 *name[10] = 	//字符串
    {
		"X-Speed", "Y-speed", "X-minus", "must-Y ", "must-Y2",
		"down_y ", "down_y2", "ahead_v", "delay  ", "CKey-en",
    };
	if (parameter_en == 1) //开启参数更改
	{
		if (interact_signal == UPSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
				case L0:  barrier_vx+=10;  break;
				case L1:  barrier_vy+=0.1; break;
				case L2:  barrier_x_minus+=5; break;
				case L3:  barrier_must_y+=1; break;
                case L4:  barrier_must_y2+=1; break;
                case L5:  down_y+=1; break;
				case L6:  down_y2+=1; break;
                case L7:  ahead_down_thd+=1; break;
                case L8:  barrier_delay+=10; break;
                case L9: crash_key_en = 1; break;
				default:  break;
			}
		}
		else if (interact_signal == DOWNSIG)    
		{
            save_flag = 1;
			switch (arrow)
			{
                case L0:  if (barrier_vx > 10)barrier_vx-=10;  break;
				case L1:  if (barrier_vy > 0.1)barrier_vy-=0.1; break;
				case L2:  if (barrier_x_minus > 5)barrier_x_minus-=5; break;
				case L3:  if (barrier_must_y > 1)barrier_must_y-=1; break;
                case L4:  if (barrier_must_y2 > 1)barrier_must_y2-=1; break;
                case L5:  if (down_y > 1)down_y-=1; break;
				case L6:  if (down_y2 > 1)down_y2-=1; break;
                case L7:  if (ahead_down_thd > 1)ahead_down_thd-=1; break;
                case L8:  if (barrier_delay >=10)barrier_delay-=10; break;
                case L9:   crash_key_en = 0; break;
				default:  break;
			}
		}
	}
	else	//未开启参数更改
	{	
		if (interact_signal == UPSIG)
        {
			arrow--;
        }
		else if (interact_signal == DOWNSIG)
			arrow++;

		if (arrow == L10) arrow = L0;
        if (arrow > L10) arrow = L9;
	}

	//显示字符串
    page = arrow/5*5;
	OLED_ShowString(50, FIRST_Y,  name[page+0], 12);
	OLED_ShowString(50, SECOND_Y, name[page+1], 12);
	OLED_ShowString(50, THIRD_Y,  name[page+2], 12);
	OLED_ShowString(50, FORTH_Y,  name[page+3], 12);
	OLED_ShowString(50, FIFTH_Y,  name[page+4], 12);
	//显示参数
	switch (page)
	{
		case 0: OLED_ShowNum(100, FIRST_Y,  barrier_vx, 3, 12, 0);
				OLED_ShowNum(100, SECOND_Y, (int)(barrier_vy*10), 3, 12, 0);
				OLED_ShowNum(100, THIRD_Y,  barrier_x_minus,  3, 12, 0);
				OLED_ShowNum(100, FORTH_Y,  barrier_must_y,  3, 12, 0);
                OLED_ShowNum(100, FIFTH_Y,  barrier_must_y2,  3, 12, 0);
				break;
        case 5: OLED_ShowNum(100, FIRST_Y,  down_y, 3, 12, 0);
				OLED_ShowNum(100, SECOND_Y, down_y2, 3, 12, 0);
				OLED_ShowNum(100, THIRD_Y,  ahead_down_thd,  3, 12, 0);
				OLED_ShowNum(100, FORTH_Y,  barrier_delay,  3, 12, 0);
                OLED_ShowNum(100, FIFTH_Y,  crash_key_en,  3, 12, 0);
				break;
                    
		default:break;
	}
	//反色显示字符串
	switch (arrow%5)
	{
		case L0:  OLED_ShowString_Inverse(50, FIRST_Y,  name[page+0], 12); break;
		case L1:  OLED_ShowString_Inverse(50, SECOND_Y, name[page+1], 12); break;
		case L2:  OLED_ShowString_Inverse(50, THIRD_Y,  name[page+2], 12); break;
		case L3:  OLED_ShowString_Inverse(50, FORTH_Y,  name[page+3], 12); break;
        case L4:  OLED_ShowString_Inverse(50, FIFTH_Y,  name[page+4], 12); break;
		default:  break;
	}	
	//反色参数
	if (parameter_en == 1)
	{
		switch (arrow)
		{
			case L0:  OLED_ShowNum_Inverse(100, FIRST_Y,  barrier_vx, 3, 12, 0); break;
			case L1:  OLED_ShowNum_Inverse(100, SECOND_Y,  (int)(barrier_vy*10), 3, 12, 0); break;
			case L2:  OLED_ShowNum_Inverse(100, THIRD_Y,  barrier_x_minus,  3, 12, 0); break;
			case L3:  OLED_ShowNum_Inverse(100, FORTH_Y,  barrier_must_y,  3, 12, 0); break;
            case L4:  OLED_ShowNum_Inverse(100, FIFTH_Y,  barrier_must_y2,  3, 12, 0);break;
            case L5:  OLED_ShowNum_Inverse(100, FIRST_Y,  down_y,  3, 12, 0); break;
			case L6:  OLED_ShowNum_Inverse(100, SECOND_Y,  down_y2,  3, 12, 0); break;
            case L7:  OLED_ShowNum_Inverse(100, THIRD_Y,  ahead_down_thd,  3, 12, 0);break;
            case L8:  OLED_ShowNum_Inverse(100, FORTH_Y,  barrier_delay,  3, 12, 0);break;
            case L9:  OLED_ShowNum_Inverse(100, FIFTH_Y,  crash_key_en,  3, 12, 0);break;
            
			default:  break;
		}
	}
    OLED_ShowString(0,0,"CA2",16);
    if (save_flag == 1)
    {
       OLED_ShowChar(120,0,'*',12,1);
    }
    else
    {
        OLED_ShowChar(120,0,' ',12,1);
    }
    if (interact_signal == OUTSIG && save_flag == 1)
    {
        save_flag = 0;
        FLASH_Write();
    }
}
void MENU_Motor()    
{
    static uint8 arrow = L4;	//箭头方向
    uint8 page = 0;		//页数0-5-10
    static uint8 save_flag = 0;
    uint8 *name[15] = 	//字符串
    {
    	"v1  ", "v2  ", "v3  ", "v4  ", "defV",
        "higV", "lowV", "sKP ", "sKI ", "sKD ",
        "c_vx", "c_vy", "c_vz", "cvy2", "cvz2",
    };
    
    //特殊显示start
     OLED_Showint(70+24, THIRD_Y, wheel.v1, 3,12);
     OLED_Showint(70, THIRD_Y, wheel.v2, 3,12);
     OLED_Showint(70, FIFTH_Y, wheel.v3, 3,12);
     OLED_Showint(70+24, FIFTH_Y, wheel.v4, 3,12);
    //特殊显示end
	if (parameter_en == 1) //开启参数更改
	{
		if (interact_signal == UPSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
                case L4:  def_speed+=10; break;
                case L5:  up_speed+=10; break;
                case L6:  down_speed+=10;break;
				case L7:  motor_kp+=10; break;
				case L8:  motor_ki+=10; break;
				case L9:  motor_kd+=10; break;
                case L10: curve_vx+=10;break;
                case L11: curve_vy+=10;break;
                case L12: curve_vz+=10;break;
                case L13: curve_vy2+=10;break;
                case L14: curve_vz2+=10;break;
				default:  break;
			}
            motor_kp = motor_kp>15000?15000:motor_kp;
            motor_ki = motor_ki>10000?10000:motor_ki;
            motor_kd = motor_kd>10000?10000:motor_kd;
      
            
            up_speed = up_speed>900?900:up_speed;
            def_speed = def_speed>900?900:def_speed;
            def_speed = def_speed<0?0:def_speed;
            down_speed = down_speed>900?900:down_speed;             
            curve_vx = curve_vx>900?900:curve_vx;
            curve_vy = curve_vy>900?900:curve_vy;
            curve_vz = curve_vz>900?900:curve_vz; 
		}
		else if (interact_signal == DOWNSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
                case L4: if (def_speed >= 10) def_speed-=10;break;
                case L5: if (up_speed >= 10) up_speed-=10;break;
                case L6: if (down_speed >= -100) down_speed-=10;break;
                case L7: if (motor_kp >= 10) motor_kp-=10; break;
                case L8: if (motor_ki >= 10) motor_ki-=10; break;
                case L9: if (motor_kd >= 10) motor_kd-=10; break;
                case L10: if (curve_vx >= -900) curve_vx-=10;break;
                case L11: if (curve_vy >= -900) curve_vy-=10;break;
                case L12: if (curve_vz >= 10 ) curve_vz-=10;
                          else                 curve_vz = 0;
                          break;
                case L13: if (curve_vy2 >= 10 ) curve_vy2-=10;
                          else                  curve_vy2 = 0;
                          break;
                case L14: if (curve_vz2 >= 10 ) curve_vz2-=10;
                          else                  curve_vz2 = 0;
                          break;
				default:  break;
			}
            
		}
	}
	else	//未开启参数更改
	{	
		if (interact_signal == UPSIG)
			arrow--;
		else if (interact_signal == DOWNSIG)
			arrow++;

		if (arrow > L14) arrow = L4;
		if (arrow < L4) arrow = L14;
	}
    
    OLED_ShowString(75,0,"MOTOR",16);

	//显示字符串
    page = arrow/5*5;
	OLED_ShowString(0, FIRST_Y,  name[page+0], 12);
	OLED_ShowString(0, SECOND_Y, name[page+1], 12);
	OLED_ShowString(0, THIRD_Y,  name[page+2], 12);
	OLED_ShowString(0, FORTH_Y,  name[page+3], 12);
	OLED_ShowString(0, FIFTH_Y,  name[page+4], 12);

	switch (page)
	{
		case 0: OLED_Showint(35, FIRST_Y, act_speed1,3,12);
                OLED_Showint(35, SECOND_Y,act_speed2,3,12);
                OLED_Showint(35, THIRD_Y, act_speed3,3,12);
                OLED_Showint(35, FORTH_Y, act_speed4,3,12);
                OLED_ShowNum(35, FIFTH_Y, def_speed,4, 12, 0);
				
				break;
		case 5:	OLED_ShowNum(35, FIRST_Y,  up_speed,  4, 12, 0);
				OLED_Showint(35, SECOND_Y,  down_speed, 3, 12);
                OLED_ShowNum(35, THIRD_Y, motor_kp,4, 12, 0);
				OLED_ShowNum(35, FORTH_Y, motor_ki,4, 12, 0);
				OLED_ShowNum(35, FIFTH_Y, motor_kd,4, 12, 0);
				break;
		case 10:
				OLED_Showint(35, FIRST_Y,  curve_vx,3,12);
                OLED_Showint(35, SECOND_Y, curve_vy,3,12);
                OLED_Showint(35, THIRD_Y,  curve_vz,3,12);
                OLED_Showint(35, FORTH_Y,  curve_vy2,3,12);
                OLED_Showint(35, FIFTH_Y,  curve_vz2,3,12);
				break;
		default:break;
	}
	//反色显示字符串
	switch (arrow%5)
	{
		case L0:  OLED_ShowString_Inverse(0, FIRST_Y,  name[page+0], 12); break;
		case L1:  OLED_ShowString_Inverse(0, SECOND_Y, name[page+1], 12); break;
		case L2:  OLED_ShowString_Inverse(0, THIRD_Y,  name[page+2], 12); break;
		case L3:  OLED_ShowString_Inverse(0, FORTH_Y,  name[page+3], 12); break;
		case L4:  OLED_ShowString_Inverse(0, FIFTH_Y,  name[page+4], 12); break;
		default:  break;
	}	
	//反色参数
	if (parameter_en == 1)
	{
		switch (arrow)
		{
            case L4: OLED_ShowNum_Inverse(35, FIFTH_Y,  def_speed,     4, 12, 0); break;
            case L5: OLED_ShowNum_Inverse(35, FIRST_Y, up_speed,  4, 12, 0); break;
			case L6: OLED_Showint_Inverse(35, SECOND_Y,  down_speed,    3, 12); break;
			case L7: OLED_ShowNum_Inverse(35, THIRD_Y,  motor_kp,  4, 12, 0); break;
			case L8: OLED_ShowNum_Inverse(35, FORTH_Y,  motor_ki,  4, 12, 0); break;
			case L9: OLED_ShowNum_Inverse(35, FIFTH_Y,  motor_kd,  4, 12, 0); break;   
            case L10: OLED_Showint_Inverse(35, FIRST_Y,  curve_vx,  3, 12); break;
			case L11: OLED_Showint_Inverse(35, SECOND_Y,  curve_vy, 3, 12); break;
			case L12: OLED_Showint_Inverse(35, THIRD_Y,  curve_vz,  3, 12); break;   
            case L13: OLED_Showint_Inverse(35, FORTH_Y,  curve_vy2, 3, 12); break;
			case L14: OLED_Showint_Inverse(35, FIFTH_Y,  curve_vz2,  3, 12); break;    
			default:  break;
		}
	}
    if (save_flag == 1)
    {
       OLED_ShowChar(120,0,'*',16,1);
    }
    else
    {
        OLED_ShowChar(120,0,' ',16,1);
    }
    if (interact_signal == OUTSIG && save_flag == 1)
    {
        save_flag = 0;
        PID_MotorInit(&pid_motor_1);
        PID_MotorInit(&pid_motor_2);
        PID_MotorInit(&pid_motor_3);
        PID_MotorInit(&pid_motor_4);
        FLASH_Write();	
    }
}


void MENU_Position() 
{
    static uint8 arrow = L0;	//箭头方向
    uint8 page = 0;		//页数0-5-10
    static uint8 save_flag = 0;
    uint8 *name[15] = 	//字符串 // 0是低速1是高速用（def——speed）
    {
        "zP0 ", "zP1 ", "zI0 ", "zD0 ", "ZTHD", 
        "cP  ", "cI  ", "cD  ", "plr0", "Plr1", 
        "OUTP", "OUTZ", "MDIR", "DOWN", "    ",
    };
    //特殊start
    OLED_Showint(75, THIRD_Y, wheel.vx,3,12);
    OLED_Showint(75, FORTH_Y, wheel.vy,3,12);
    OLED_ShowNumFloat(75, FIFTH_Y, wheel.vz,  3, 12, 1);
    //特殊end
	if (parameter_en == 1) //开启参数更改
	{
		if (interact_signal == UPSIG)
		{
            save_flag = 1;
			switch (arrow)
			{
				case L0:  position_zp0++;break;
				case L1:  position_zp1+=1; break;
				case L2:  position_zi0+=1; break;
				case L3:  position_zd0+=1; break;
				case L4:  gyro_thd+=1; break;
                case L5:  position_cp+=1; break;
				case L6:  position_ci+=1; break;
				case L7:  position_cd+=1; break;
				case L8:  pvlr0+=1; break;
				case L9:  pvlr1+=1; break;
                case L10: out0_limit+=1;break;
				case L11: out1_limit+=1;break;
                case L12: must_dir ++;break;
                case L13: must_down=1;break;
				default:  break;
			}
		}
		else if (interact_signal == DOWNSIG)
		{
            //z-pid  z低 单独pd    2
            //z高   两个方向不同pd 4
            //x-pid
            save_flag = 1;
			switch (arrow)
			{
                case L0: if (position_zp0 >= 1) position_zp0-=1;break;
				case L1: if (position_zp1 >=1) position_zp1-=1; break;
				case L2: if (position_zi0 >=1) position_zi0-=1; break;
				case L3: if (position_zd0 >=1) position_zd0-=1; break;
				case L4: if (gyro_thd >=1) gyro_thd-=1; break;
                case L5: if (position_cp >= 1) position_cp-=1; break;
				case L6: if (position_ci >= 1) position_ci-=1; break;
				case L7: if (position_cd >= 1) position_cd-=1; break;
				case L8: if (pvlr0 >= 1) pvlr0-=1; break;
				case L9: if (pvlr1 >= 1) pvlr1-=1; break;
                case L10:if (out0_limit >= 1) out0_limit-=1;break;
				case L11:if (out1_limit >= 1) out1_limit-=1;break;
                case L12:if (must_dir >= 1) must_dir-=1;break;
				case L13:if (must_down >= 1) must_down-=1;break;
				default:  break;
			}
		}
	}
	else	//未开启参数更改
	{	
		if (interact_signal == UPSIG)
			arrow--;
		else if (interact_signal == DOWNSIG)
			arrow++;

		if (arrow == L14) arrow = L0;
		if (arrow > L14) arrow = L13;
	}

    OLED_ShowString(75,0,"POS  ",16);
	//显示字符串
    page = arrow/5*5;
	OLED_ShowString(0, FIRST_Y,  name[page+0], 12);
	OLED_ShowString(0, SECOND_Y, name[page+1], 12);
	OLED_ShowString(0, THIRD_Y,  name[page+2], 12);
	OLED_ShowString(0, FORTH_Y,  name[page+3], 12);
	OLED_ShowString(0, FIFTH_Y,  name[page+4], 12);
	//显示参数
	switch (page)
	{
		case 0: OLED_ShowNum(30, FIRST_Y,  position_zp0, 5, 12, 0);
				OLED_ShowNum(30, SECOND_Y, position_zp1, 5, 12, 0);
				OLED_ShowNum(30, THIRD_Y, position_zi0,  5, 12, 0);
				OLED_ShowNum(30, FORTH_Y, position_zd0,  5, 12, 0);
				OLED_ShowNum(30, FIFTH_Y, gyro_thd,  5, 12, 0);
				break; 
        case 5: 
                OLED_ShowNum(30, FIRST_Y, position_cp,  5, 12, 0);
                OLED_ShowNum(30, SECOND_Y,position_ci,  5, 12, 0);
				OLED_ShowNum(30, THIRD_Y, position_cd,  5, 12, 0);
				OLED_ShowNum(30, FORTH_Y, pvlr0,  5, 12, 0);
                OLED_ShowNum(30, FIFTH_Y, pvlr1,      5, 12 ,0);
                break;
        case 10:
                OLED_ShowNumFloat(30, FIRST_Y, out0_limit,  3, 12, 1);
                OLED_ShowNumFloat(30, SECOND_Y,out1_limit,  3, 12, 1);
                OLED_ShowNum(30, THIRD_Y, must_dir,  5, 12, 0);
				OLED_ShowNum(30, FORTH_Y, must_down,  5, 12, 0);
                OLED_ShowNum(30, FIFTH_Y, 0,  5, 12 ,0);
                break;
		default:break;
	}
	//反色显示字符串
	switch (arrow%5)
	{
		case L0:  OLED_ShowString_Inverse(0, FIRST_Y,  name[page+0], 12); break;
		case L1:  OLED_ShowString_Inverse(0, SECOND_Y, name[page+1], 12); break;
		case L2:  OLED_ShowString_Inverse(0, THIRD_Y,  name[page+2], 12); break;
		case L3:  OLED_ShowString_Inverse(0, FORTH_Y,  name[page+3], 12); break;
		case L4:  OLED_ShowString_Inverse(0, FIFTH_Y,  name[page+4], 12); break;
		default:  break;
	}	
	//反色参数
	if (parameter_en == 1)
	{
		switch (arrow)
		{
			case L0:  OLED_ShowNum_Inverse(30, FIRST_Y, position_zp0,         5, 12, 0); break;
			case L1:  OLED_ShowNum_Inverse(30, SECOND_Y,position_zp1, 5, 12, 0); break;
			case L2:  OLED_ShowNum_Inverse(30, THIRD_Y, position_zi0, 5, 12, 0); break;
			case L3:  OLED_ShowNum_Inverse(30, FORTH_Y, position_zd0, 5, 12, 0); break;
            case L4:  OLED_ShowNum_Inverse(30, FIFTH_Y, gyro_thd, 5, 12, 0); break;
            
            case L5:  OLED_ShowNum_Inverse(30, FIRST_Y, position_cp, 5, 12, 0); break;
			case L6:  OLED_ShowNum_Inverse(30, SECOND_Y,position_ci, 5, 12, 0); break;
			case L7:  OLED_ShowNum_Inverse(30, THIRD_Y, position_cd, 5, 12, 0); break;
			case L8:  OLED_ShowNum_Inverse(30, FORTH_Y, pvlr0, 5, 12, 0); break;
            case L9:  OLED_ShowNum_Inverse(30, FIFTH_Y, pvlr1,     5, 12, 0); break;
            
            case L10:  OLED_ShowNumFloat(30, FIRST_Y, out0_limit,3, 12, 0); break;
			case L11:  OLED_ShowNumFloat(30, SECOND_Y,out1_limit,3, 12, 0); break;
            case L12:  OLED_ShowNum_Inverse(30, THIRD_Y, must_dir, 5, 12, 0);break;
            case L13:  OLED_ShowNum_Inverse(30, FORTH_Y, must_down, 5, 12, 0); break;
			default:  break;
		}
	}
    		
    if (save_flag == 1)
    {
       OLED_ShowChar(120,0,'*',16,1);
    }
    else
    {
        OLED_ShowChar(120,0,' ',16,1);
    }
    if (interact_signal == OUTSIG && save_flag == 1)
    {
       save_flag = 0;
       PID_PositionInit(&pid_position);
       FLASH_Write();
    }
}
//force_car_dir force_speed_down
void MENU_Array()
{
    uint8 i;
    static uint8 arrow = L0;
    OLED_ShowString(80,SECOND_Y,"head-dir",12);
    OLED_ShowString(80,FIFTH_Y, "v-down  ",12);
    if (parameter_en == 0)
    {
        if (interact_signal == UPSIG)
        {
           if (arrow > L0) arrow--;
           else if (arrow == L0) arrow = L29;
        }
        else if (interact_signal == DOWNSIG)
        {
          if (arrow < L29) arrow++;
          else if (arrow == L29) arrow = L0;
        }
    }
    else if (parameter_en == 1)
    {   //force_car_dir
        if (arrow < 15)
        {
            if (interact_signal == UPSIG)
            {
               force_car_dir[arrow]++;
               if (force_car_dir[arrow] == 3) force_car_dir[arrow] = 0;
               if (force_car_dir[arrow] > 3 ) force_car_dir[arrow] = 2;
               FLASH_Write();
            }
            else if (interact_signal == DOWNSIG)
            {
               force_car_dir[arrow]--;
               if (force_car_dir[arrow] == 3) force_car_dir[arrow] = 0;
               if (force_car_dir[arrow] > 3 ) force_car_dir[arrow] = 2;
               FLASH_Write();
            } 
        }
        else//force_speed_down
        {
            if (interact_signal == UPSIG)
            {
               force_speed_down[arrow-15]++;
               if (force_speed_down[arrow-15] == 3) force_speed_down[arrow-15] = 0;
               if (force_speed_down[arrow-15] > 3 ) force_speed_down[arrow-15] = 2;
               FLASH_Write();
            }
            else if (interact_signal == DOWNSIG)
            {
               force_speed_down[arrow-15]--;
               if (force_speed_down[arrow-15] == 3) force_speed_down[arrow-15] = 0;
               if (force_speed_down[arrow-15] > 3 ) force_speed_down[arrow-15] = 2;
               FLASH_Write();
            } 
        }
        
    }
    for (i=0; i<15; i++)
    {
        OLED_ShowNum(i%10*12,i/10*12, force_car_dir[i], 1, 12, 1);
        OLED_ShowNum(i%10*12,i/10*12+36, force_speed_down[i], 1, 12, 1);
        OLED_ShowChar(i%10*12+6,i/10*12,' ',12,1);
        OLED_ShowChar(i%10*12+6,i/10*12+36,' ',12,1);
    }
   
    if (arrow < 15)OLED_ShowChar(arrow%10*12+6,arrow/10*12,'*',12,1);
    else           OLED_ShowChar((arrow-15)%10*12+6,(arrow-15)/10*12+36,'*',12,1);
    //OLED_ShowChar(arrow%10*12+6,arrow/10*12,'*',12,1);
    if (parameter_en == 1)
    {
        if (arrow < 15)
            OLED_ShowNum_Inverse(arrow%10*12,arrow/10*12, force_car_dir[arrow], 1, 12, 1);
        else
            OLED_ShowNum_Inverse((arrow-15)%10*12,(arrow-15)/10*12+36, force_speed_down[arrow-15], 1, 12, 1);
        
    }
}
void MENU_Cut()
{
    uint8 i;
    static uint8 arrow = L0;
    OLED_ShowString(80,SECOND_Y,"cut-dir ",12);
    OLED_ShowString(80,FIFTH_Y, "turn-dir",12);
    if (parameter_en == 0)
    {
        if (interact_signal == UPSIG)
        {
           if (arrow > L0) arrow--;
           else if (arrow == L0) arrow = L29;
        }
        else if (interact_signal == DOWNSIG)
        {
          if (arrow < L29) arrow++;
          else if (arrow == L29) arrow = L0;
        }
    }
    else if (parameter_en == 1)
    {
        if (interact_signal == UPSIG)
        {
           cut_dir[arrow]++;
           if (cut_dir[arrow] == 3) cut_dir[arrow] = 0;
           if (cut_dir[arrow] > 3 ) cut_dir[arrow] = 2;
           FLASH_Write();
        }
        else if (interact_signal == DOWNSIG)
        {
           cut_dir[arrow]--;
           if (cut_dir[arrow] == 3) cut_dir[arrow] = 0;
           if (cut_dir[arrow] > 3 ) cut_dir[arrow] = 2;
           FLASH_Write();
        } 
    }
    for (i=0; i<15; i++)
    {
        OLED_ShowNum(i%10*12,i/10*12, cut_dir[i], 1, 12, 1);
        OLED_ShowChar(i%10*12+6,i/10*12,' ',12,1);
    }
    for (i=15; i<30; i++)
    {
        OLED_ShowNum((i-15)%10*12,(i-15)/10*12+36, cut_dir[i], 1, 12, 1);
        OLED_ShowChar((i-15)%10*12+6,(i-15)/10*12+36,' ',12,1);
    }
    if (arrow < 15)OLED_ShowChar(arrow%10*12+6,arrow/10*12,'*',12,1);
    else           OLED_ShowChar((arrow-15)%10*12+6,(arrow-15)/10*12+36,'*',12,1);
    //OLED_ShowChar(arrow%10*12+6,arrow/10*12,'*',12,1);
    if (parameter_en == 1)
    {
        if (arrow < 15)
            OLED_ShowNum_Inverse(arrow%10*12,arrow/10*12, cut_dir[arrow], 1, 12, 1);
        else
            OLED_ShowNum_Inverse((arrow-15)%10*12,(arrow-15)/10*12+36, cut_dir[arrow], 1, 12, 1);
        
    }
}
void MENU_ShowBinary(uint8 image_binary[ROW][COL])//显示二值化图像
{
    uint8 x,y;
    for(y=0; y<ROW; y++)
    {
        for (x=0; x<COL; x++)
        {
            OLED_DrawPoint(x,y,image_binary[y][x]);
        }
    }
}


void MENU_CoordCross(uint16 x,uint16 y)
{
   // uint8 i;
//    for (i=0; i<ROW; i++)
//    {
//      OLED_DrawPoint(x,i,1);
//    }
//    for (i=0; i<COL; i++)
//    {
//      OLED_DrawPoint(i,y,1);
//    }
//    
//    OLED_ShowChar(83,4*12+4,'[',12,1);
//    OLED_ShowNum(90,4*12+4,x,2,12,0);
//    OLED_ShowChar(103,4*12+4,',',12,1);
//    OLED_ShowNum(108,4*12+4,y,2,12,0);
//    OLED_ShowChar(120,4*12+4,']',12,1);

    OLED_ShowNum(0,0,x,3,12,0);
    OLED_ShowNum(0,12,y,3,12,0);
}

void MENU_CoordPoint(uint16 x, uint16 y)
{
    OLED_DrawPoint(x,y,0);
//    OLED_ShowChar(33,4*12,'[',12,1);
//    OLED_ShowNum(40,4*12,x,2,12,0);
//    OLED_ShowChar(53,4*12,',',12,1);
//    OLED_ShowNum(58,4*12,y,2,12,0);
//    OLED_ShowChar(70,4*12,']',12,1);
      OLED_ShowNum(0,0,x,3,12,0);
  
    OLED_ShowNum(0,12,y,3,12,0);
}
