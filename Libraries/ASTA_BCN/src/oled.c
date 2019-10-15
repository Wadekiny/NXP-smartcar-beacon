#include "oled.h"
#include "oledfont.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127
uint8 OLED_GRAM[128][8];	 

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
	uint8_t i;	
    gpio_set(OLED_DC,cmd);//OLED_DC=cmd; //写命令 
    gpio_set(OLED_CS,0);	//OLED_CS=0;	  
	for(i=0;i<8;i++)
	{		
        gpio_set(OLED_SCLK,0);	  //OLED_SCLK=0;
		if(dat&0x80)gpio_set(OLED_SDIN,1);//OLED_SDIN=1;
		else gpio_set(OLED_SDIN,0);//OLED_SDIN=0;
		gpio_set(OLED_SCLK,1);//OLED_SCLK=1;
		dat<<=1;   
	}				 
	gpio_set(OLED_CS,1);//OLED_CS=1;		  
	gpio_set(OLED_DC,1);//OLED_DC=1;   	  
} 
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			 temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//mode:0,反白显示;1,正常显示				 
//no:字体索引号
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t no,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y; 
	uint8_t csize=(size/8+((size%8)?1:0))*size;			//得到一个字体汉字对应点阵集所占的字节数
    for(t=0;t<csize;t++)
    { 
		if(size==12)		temp=HanZi_12[2*no+t/12][t%12]; 	//调用1206字体
		else if(size==16)	temp=HanZi_16[2*no+t/16][t%16];		//调用1608字体
		else if(size==24)	temp=HanZi_24[2*no+t/24][t%24];		//调用2412字体
		else return;											//没有的字库
							
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==12)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }     

}

//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  

//计算数字位数
uint8 mybit(uint32 m)
{
	uint8 t=0;
	while(++t)
	{
		if( m/mypow(10,t)==0 )
			break;	
	}
	return t;
}

//显示数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,高位不补0;1,高位补0
//num:数值(0~4294967295);	 
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode==1)	OLED_ShowChar(x+(size/2)*t,y,'0',size,1);
				else		OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 


//反白显示数字
void OLED_ShowNum_Inverse(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode==1)	OLED_ShowChar(x+(size/2)*t,y,'0',size,0);
				else		OLED_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 


void OLED_Showint(uint8 x,uint8 y,int data,uint8 len,uint8 size)
{
	int temp;
	if(data<0)
	{
		temp=-data;
		OLED_ShowChar(x,y,'-',size,1);
	}
	else 
	{
		temp=data;
		OLED_ShowChar(x,y,' ',size,1);
	}
	OLED_ShowNum(x+size/2,y,temp,len,size,0);
}
void OLED_Showint_Inverse(uint8 x,uint8 y,int data,uint8 len,uint8 size)
{
	int temp;
	if(data<0)
	{
		temp=-data;
		OLED_ShowChar(x,y,'-',size,0);
	}
	else 
	{
		temp=data;
		OLED_ShowChar(x,y,' ',size,0);
	}
	OLED_ShowNum_Inverse(x+size/2,y,temp,len,size,0);
}


//显示浮点数
//x,y :起点坐标
//num:数值
//len :数字字符个数（不包括小数点）
//size:字体大小
//mode:0,反白显示;1,正常显示;	 
void OLED_ShowNumFloat(uint8_t x,uint8_t y,float num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8 len_I=0;			//整数和小数部分的长度
	uint32  n=0;				
    if (num < 0) 
    {
        num = -num;
        OLED_ShowChar(x,y,'-',12,mode);
    }
    else
    {
        OLED_ShowChar(x,y,' ',12,mode);
    }
    x = x+size/2;
    
	//显示整数部分
	n=(uint32)num;
	len_I=mybit(n);							//整数部分的显示位数
	for(t=0;t<len_I;t++)
	{
		temp=(n/mypow(10,len_I-t-1))%10;
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
	
	//显示小数部分
	n=(uint32)(num*mypow(10,len-len_I))%mypow(10,len-len_I);		//取出小数部分
	if(len-len_I)													//判断显示小数点
		OLED_ShowChar(x+(size/2)*t,y,'.',size,1);
	
	for(t=len_I;t<len;t++)
	{
		temp=(n/mypow(10,len-t-1))%10;
	 	OLED_ShowChar(x+(size/2)*(t+1),y,temp+'0',size,mode); 
	}
	
}

//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
}	

//反白显示字符串
void OLED_ShowString_Inverse(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,0);	 
        x+=size/2;
        p++;
    }  
}	

//显示图像函数
//以img_x，img_y为图像像素宽度,高度
//void Display_Image(uint8_t img_x,uint8_t img_y,uint8_t *image)
//{
//		
//	uint8_t temp,t,i,j;
//	uint8_t x1;
//	uint8_t x;
//	if(img_x%8==0) x1=img_x/8;      
//	else x1=img_x/8+1;
//	
//	for(j=0;j<img_y;j++)
//	{
//		for(i=0;i<x1;i++)
//		{   
//			x=0;
//			temp=image[j*10+i]; 	 					
//			for(t=0;t<8;t++)
//			{
//				if(temp&0x80)OLED_DrawPoint(i*8+x,j,0);
//				else OLED_DrawPoint(i*8+x,j,1);
//				temp<<=1;
//				x++;
//			}  	 
//		}
//	} 
//	OLED_Refresh_Gram();
//}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//wide:图片像素宽度
//hight:图片像素高度
//mode:0,反白显示;1,正常显示	
//BMP:图片数组名
void OLED_ShowBMP(uint8_t x,uint8_t y,uint8_t wide,uint8_t hight,uint8_t mode,unsigned char* BMP)
{      			    
	uint8_t temp,t,i,j;
	uint8_t y0=y; 
	uint8_t x0=x;
	uint8_t y1;
	
	if(hight%8==0) y1=hight/8;      
	else y1=hight/8+1;
	
	for(i=0;i<wide;i++)
	{
		y=y0;
		x=x0+i;
		for(j=0;j<y1;j++)
		{   
			temp=BMP[i*8+j]; 	 					
			for(t=0;t<8;t++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				y++;
				if(y>64) break;
			}  	 
		}
	}     
}

//初始化SSD1306					    
void OLED_Init(void)
{ 	 	
    gpio_init  (OLED_CS,  GPO, 1); //初始化SCL    
    gpio_init  (OLED_RST, GPO, 1); //初始化SDA    
    gpio_init  (OLED_DC,  GPO, 1); //初始化SCL    
    gpio_init  (OLED_SCLK,GPO, 1); //初始化SDA   
    gpio_init  (OLED_SDIN,GPO, 1); //初始化SCL

    port_init_NoAlt(OLED_CS,  PULLUP);
    port_init_NoAlt(OLED_RST, PULLUP);
    port_init_NoAlt(OLED_DC,  PULLUP);
    port_init_NoAlt(OLED_SCLK,PULLUP);
    port_init_NoAlt(OLED_SDIN,PULLUP);
      
	gpio_set(OLED_CS,1);//OLED_CS=1;
	gpio_set(OLED_DC,1);//OLED_DC=1;	 
//复位引脚接芯片复位引脚可注释	
	gpio_set(OLED_RST,0);//OLED_RST=0;
	systick_delay_ms(100);//DELAY_MS(100);
	gpio_set(OLED_RST,1);//OLED_RST=1; 
//////////////////////////////	
    
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
}  

