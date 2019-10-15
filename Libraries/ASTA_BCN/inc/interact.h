#ifndef _INTERACT_H_
#define _INTERACT_H_

#include "ALLBCN.h"
#include "MT9V032.h"
#define LINE_HIGH	(1*12)

enum SIGx
{
    NONESIG,   
    UPSIG,		
    DOWNSIG,
    LEFTSIG,
    RIGHTSIG,
    ENTERSIG,	
    OUTSIG,    
};

enum LINEx
{
    FIRST_Y = 0,
    SECOND_Y = 1*12,
    THIRD_Y = 2*12,
    FORTH_Y = 3*12,
    FIFTH_Y = 4*12,  
};
enum PAGEx 
{
	MAIN_PAGE,
    CAMERA_PAGE,
    CAMERA2_PAGE,
	POSITION_PAGE,
	MOTOR_PAGE,
    GARISH_PAGE,
    //CUT_PAGE,
    //ARRAY_PAGE,
    MAX_PAGE,
};

enum OTHERx
{
    L0,L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13,L14,
    L15,L16,L17,L18,L19,L20,L21,L22,L23,L24,L25,L26,L27,L28,L29,
    LMAX,
};

enum SIGx MENU_GetSig();
void MENU_SigHandling();
void MENU_Display();
void MENU_Camera();
void MENU_Camera2();
void MENU_Motor();
void MENU_Position();
void MENU_Array();
void MENU_Main();
void MENU_Cut();
void MENU_Garish();
void MENU_ShowBinary(uint8 image_binary[ROW][COL]);//显示二值化图像
void MENU_CoordCross(uint16 x,uint16 y);
void MENU_CoordPoint(uint16 x, uint16 y);
#endif
