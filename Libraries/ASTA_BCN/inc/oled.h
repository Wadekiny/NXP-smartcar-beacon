#ifndef __OLED_H
#define __OLED_H	
#include "headfile.h"

//#include "stdlib.h"	

//---------------------------OLED端口定义--------------------------  					   

//#define OLED_CS      D11
//#define OLED_RST     D13 	
//#define OLED_DC      D14    
////使用4线串行spi接口时使用 
//#define OLED_SCLK    E3
//#define OLED_SDIN    D15

#define OLED_SCLK    C0
#define OLED_SDIN    B23
#define OLED_RST     B22 	
#define OLED_DC      B21  
#define OLED_CS      B20

//#define OLED_CS  PTXn_T(CS,OUT)
//#define OLED_RST PTXn_T(RST,OUT) 	
//#define OLED_DC  PTXn_T(DC,OUT)   
////使用4线串行接口时使用 
//#define OLED_SCLK PTXn_T(SCLK,OUT)
//#define OLED_SDIN PTXn_T(SDIN,OUT)
		     

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_Init(void);       
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Clear(void);
//OLED显示函数
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void OLED_ShowNum_Inverse(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void OLED_Showint(uint8 x,uint8 y,int data,uint8 len,uint8 size);//显示整数,包括正负数
void OLED_Showint_Inverse(uint8 x,uint8 y,int data,uint8 len,uint8 size);
void OLED_ShowNumFloat(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size,uint8_t mode);//显示浮点数

void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);
void OLED_ShowString_Inverse(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);//懒得改之前用的string函数了= =直接加个函数
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no,uint8_t size,uint8_t mode);
void OLED_ShowBMP(uint8_t x,uint8_t y,uint8_t wide,uint8_t hight,uint8_t mode,unsigned char* BMP); 
//void Display_Image(uint8_t img_x,uint8_t img_y,uint8_t *image);


#endif


	 



