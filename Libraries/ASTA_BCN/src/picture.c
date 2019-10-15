#include "picture.h"
#include "ALLBCN.h"
#include "headfile.h"
#include <string.h>

uint8 image_binary[ROW][COL];//二值化之后的图像
uint8 image_binary2[ROW][COL];//二值化之后的图像
uint16 threshold = 100;

uint8 Pre_img[ROW][COL];// 前一帧图像
uint8 Diff_Img[ROW][COL];//帧差累加图像

uint8 bcn_state[20] = {0};

void IMAGE_GetBinary(uint8 img_before[ROW][COL], uint8 img_after[ROW][COL])
{
    uint8 i,j;
    for (i=0; i<ROW; i++)   //60
    {
        threshold = (100 );//thd_l
        for (j=0; j<COL; j++)//80
        {
            if (img_before[i][j] >= threshold)
            {
                img_after[i][j] = LIGHT;
            }
            else 
            {
                img_after[i][j] = DARK;
            }
        }
    }
}

/**************************************************************************
  * @brief        : 整幅图像二值化中值滤波
  * @param[in]    : dst->解压之后的60*80数组
  * @param[out]   : img_filter->滤波之后的60*80数组
  * @others       : 滤波会导致远点看不到
***************************************************************************/
uint16 img_filter[ROW][COL];
uint16 img_filter2[ROW][COL];
void IMAGE_MedianFilter(uint8 dst[CAMERA_H][CAMERA_W],uint16 img_filter[CAMERA_H][CAMERA_W]) //中值滤波
{
    uint8 hi,wi;
//    uint8 temp_sum;
    for (hi=0; hi<CAMERA_H; hi++)
    {
        for (wi=0; wi<CAMERA_W; wi++)
        {
            if (hi!=0 && hi!=CAMERA_H-1 && wi!=0 && wi!=CAMERA_W-1)
            {
                img_filter[hi][wi] = (dst[hi-1][wi-1]+dst[hi-1][wi]+dst[hi-1][wi+1]
                                    + dst[hi][wi-1]+dst[hi][wi]+dst[hi][wi+1]
                                    + dst[hi+1][wi-1]+dst[hi+1][wi]+dst[hi+1][wi+1]);
            }
            else
            {
                img_filter[hi][wi] = dst[hi][wi];
            }
        }
    }
}
uint16 img_temp[ROW][COL];
void IMAGE_FastFilter(uint8 dst[ROW][COL],uint16 img_filter[ROW][COL])
{
    uint16 sum_temp;
    uint16 ix,iy;
    for (iy=0; iy<=ROW; iy++)   //水平方向滤波
    {   
        ix = 1;
        sum_temp = dst[iy][ix-1] + dst[iy][ix] + dst[iy][ix+1];
        img_temp[iy][ix] = sum_temp;
        for (ix=2; ix<COL; ix++)
        {
            sum_temp = sum_temp - dst[iy][ix-2] + dst[iy][ix+1];
            img_temp[iy][ix] = sum_temp;
        }
    }
    for (ix=0; ix<=COL; ix++)   //垂直方向滤波
    {   
        iy = 1;
        sum_temp = img_temp[iy-1][ix] + img_temp[iy][ix] + img_temp[iy+1][ix];
        img_filter[iy][ix] = sum_temp;
        for (iy=2; iy<ROW; iy++)
        {
            sum_temp = sum_temp - img_temp[iy-2][ix] + img_temp[iy+1][ix];
            img_filter[iy][ix] = sum_temp;
        }
    }
    for (ix=1; ix<COL; ix++)
    {
        img_filter[0][ix] = img_filter[1][ix];
        img_filter[ROW-1][ix]=img_filter[ROW-2][ix];
    }
    for (iy=1; iy<ROW; iy++)
    {
        img_filter[iy][0] = img_filter[iy][1];
        img_filter[iy][COL-1]=img_filter[iy][COL-2];
    }
    img_filter[0][0] = 0;
    img_filter[0][COL-1]=0;
    img_filter[ROW-1][0]=0;
    img_filter[ROW-1][COL-1]=0;
}
/**************************************************************************
  * @brief        : 创建预设点
  * @others       : 不均匀设置预设点，远点密度大，近点密度小，共871个点
***************************************************************************/
uint16 count_point=0;         //基准点的个数
uint8 fond_xy[1200][2]={{0,0}};   //基准点坐标
void IMAGE_CreatPoint()
{
    int i,j;
    for (i=0; i<5; i+=2)
    {
        for (j=0; j<128; j+=2)
        {
            fond_xy[count_point][0] = j;
            fond_xy[count_point][1] = i;
            count_point++;
        }
    }
    for (i=1; i<5; i+=2)
    {
        for (j=1; j<128; j+=2)
        {
            fond_xy[count_point][0] = j;
            fond_xy[count_point][1] = i;
            count_point++;
        }
        
    }
    for (i=6; i<15; i+=4)
    {
        for (j=1; j<128; j+=2)
        {
            fond_xy[count_point][0] = j;
            fond_xy[count_point][1] = i;
            count_point++;
        }
    }
    for (i=8; i<15; i+=4)
    {
        for (j=0; j<128; j+=2)
        {
            fond_xy[count_point][0] = j;
            fond_xy[count_point][1] = i;
            count_point++;
        }
    }
//    for (j=2; j<128; j+=4)
//    {
//        fond_xy[count_point][0] = j;
//        fond_xy[count_point][1] = 16;
//        count_point++;
//    }
    for (i=17; i<50; i+=4)
    {
        for (j=0; j<128; j+=4)
        {
            fond_xy[count_point][0] = j;
            fond_xy[count_point][1] = i;
            count_point++;
        }
    }
    for (i=19; i<50; i+=4)
    {
        for (j=2; j<128; j+=4)
        {
            fond_xy[count_point][0] = j;
            fond_xy[count_point][1] = i;
            count_point++;
        }
    }
//    for (i=17; i<60; i+=4)
//    {
//        fond_xy[count_point][0] = 79;
//        fond_xy[count_point][1] = i;
//        count_point++;
//    }
}

/**************************************************************************
  * @brief        : 团点内容更新
  * @param[in]    : 团点结构体数组，输入图像
  * @param[out]   : 团点结构体数组，图像增加访问标志位
  * @others       : 其它
***************************************************************************/
uint8 br_index=0;               //团点个数
struct sBrightRegion br[10];   //团点结构体数组
struct sBrightRegion br2[10];   //团点结构体数组
uint8 outline[4800][2] = {{0,0}};//团点轮廓坐标数组
uint16 y_counter[60] =          //对应y理论counter
{
    4, 5, 6, 7, 8, 9,10,11,12,13,
};
uint16 counter_range[60] =     //实际counter浮动范围 
{
    0,
};
void IMAGE_ChangeStructBr(struct sBrightRegion br[10], uint8 dst[CAMERA_H][CAMERA_W])
{
	uint16 i,j;						//循环变量
	uint32 sum_x=0, sum_y=0;	    //团点坐标和
	uint16 counter=0;				//团点亮点个数
	uint16 last_end = 0, out_start=0, out_end = 0;  //团点轮廓数组标志变量
	uint8 n_x,n_y,l_x,r_x,u_y,d_y;
    uint8 center_x=0,center_y=0;
    br_index=0;
    for (i=0; i<871; i++)	//共871个预设点
    {
        if (dst[fond_xy[i][1]][fond_xy[i][0]] == 0x01)	//未访问的预设点亮点，找到团点
        {
            outline[0][0] = fond_xy[i][0];	//给第一个轮廓点赋坐标值
            outline[0][1] = fond_xy[i][1];	//给第一个轮廓点赋坐标值
            out_start = 0;	
            last_end = 0;
            out_end = 1;
            counter = 1;
            sum_x = outline[0][0];
            sum_y = outline[0][1];
            while (last_end != out_end)	//上一次末尾标志和本次末尾标志不同，有新点加入
            {
                last_end = out_end;		//更新last_end
                for (j=out_start; j<out_end; j++)	//循环扩散
                {
                    n_x = outline[j][0];
                    n_y = outline[j][1];
                    l_x = n_x - 1;
                    r_x = n_x + 1;
                    u_y = n_y - 1;
                    d_y = n_y + 1;
                    if (outline[j][0]!=0 && dst[n_y][l_x] == 0x01)//L
                    {
                        dst[n_y][l_x] = 0x81;	//图像数组元素设为已访问过的亮点
                        sum_x += l_x;					//x和更新
                        sum_y += n_y;				    //y和更新
                        outline[counter][0] = l_x;	//轮廓数组加入新值x
                        outline[counter][1] = n_y;	//轮廓数组加入新值y
                        counter++;					//团点计数，用于计算中心，更新end
                    }
                    if (outline[j][0]!=CAMERA_W-1 && dst[n_y][r_x] == 0x01)//R
                    {
                        dst[n_y][r_x] = 0x81;
                        sum_x += r_x;
                        sum_y += n_y;
                        outline[counter][0] = r_x;
                        outline[counter][1] = n_y;
                        counter++;
                    }
                    if (outline[j][1]!=0 && dst[u_y][n_x] == 0x01)//U
                    {
                        dst[u_y][n_x] = 0x81;
                        sum_x += n_x;
                        sum_y += u_y;
                        outline[counter][0] = n_x;
                        outline[counter][1] = u_y;
                        counter++;
                    }
                    if (outline[j][1]!=CAMERA_H-1 && dst[d_y][n_x] == 0x01)//D
                    {
                        dst[d_y][n_x] = 0x81;
                        sum_x += n_x;
                        sum_y += d_y;
                        outline[counter][0] = n_x;
                        outline[counter][1] = d_y;
                        counter++;
                    }
                }//for (j=out_start; j<out_end; j++)
                out_start = out_end;
                out_end = counter;
            }//while (last_end != out_end)
            if (counter >= 2 && counter <=1000)//大小合适
            {   
                center_x = (uint8)(sum_x*1.0/counter+0.5);
                center_y = (uint8)(sum_y*1.0/counter+0.5);
                //counter和y匹配
                //if (myabs((int16)counter-(int16)y_counter[center_y]) < counter_range[center_y])
                if (br_index >= 10)br_index = 10;
                else
                {
                    br[br_index].sum_x = sum_x;
                    br[br_index].sum_y = sum_y;
                    br[br_index].counter = counter;
                    br[br_index].center_x = center_x;
                    br[br_index].center_y = center_y;
                    br_index++;
                }
            }//if (counter >= 2 && counter <=1000)
        }//if (dst[fond_xy[i][1]][fond_xy[i][0]] == 0x01)
    }//for (i=0; i<871; i++)
}//IMAGE_ChangeStructBr
//寻找信标 

void IMAGE_TempCoord(uint8 * bcn_x, uint8 * bcn_y)
{
    static uint8 no_temp_cnt=0;
    if (br_index == 0)
    {
        no_temp_cnt ++;
        bcn_state[0] = BCN_DARK;
        if (no_temp_cnt >= 10)
        {
            no_temp_cnt = 10;
            bcn_state[0] = BCN_NONE;
            *bcn_x = 0;
            *bcn_y = 0;
        }
    }
    else
    {
        no_temp_cnt = 0;
        bcn_state[0] = BCN_LIGHT;
        *bcn_x = br[0].center_x;
        *bcn_y = br[0].center_y;
    }
}
/**************************************************************************
  * @brief        : 寻找闪烁坐标
  * @param[in]    : 多帧图像团点坐标
  * @param[out]   : 信标坐标
  * @return       : 
***************************************************************************/
uint8 br_mem[10][20][2];		//10组团点
uint8 br_grade[10][3];			//10组团点等级,取最高等级(优先级)  x,y,grade
uint8 empty_mem[10] = {0,};			//0为空，1为不空 初始化为0（全空）
uint8 x_offset=60,y_offset=10;
uint8 y_light[60] = //每个y对应的亮的次数
{
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
};

#define FLICK_FRAME 	20		//判断闪烁使用的帧数
#define BR_GROUP		10		//br_mem组数
void IMAGE_GetFlick(uint8* bcn_x, uint8* bcn_y)
{
	int8 i,j,k;		//循环变量
	int8 theory_light=0;//理论次数
	int8 real_light=0;//实际次数
    static uint8 no_bcn_cnt = 0;
	for (i=0; i<BR_GROUP; i++)//10组团点 每帧图像来到之后必须更新
	{
		for (j=FLICK_FRAME-1; j>0; j--)	//20帧图像后移 
		{
            br_mem[i][j][0] = br_mem[i][j-1][0];
            br_mem[i][j][1] = br_mem[i][j-1][1];
		}
		br_mem[i][0][0] = 0;
		br_mem[i][0][1] = 0;
        if (empty_mem[i] == 1)//不为空
        {
            for (j=0; j<20; j++)
            {
                if (br_mem[i][j][0] != 0 || br_mem[i][j][1] != 0) break;  //确实不为空
            }
            if (j == 20) empty_mem[i] = 0;//其实为空
        }
	}

    if (br_index == 0)  //连续10帧无任何团点，不等20帧，提前归0
    {
        no_bcn_cnt++;
        if (no_bcn_cnt > 10)
        {
            no_bcn_cnt = 10;
            *bcn_x = 0; 
            *bcn_y = 0;
        }
    }
    else
    {
        no_bcn_cnt = 0;
        //x 0 x1 x2 x3 x4 x5 x6 x7 x8 x9 (1个团点)
        //y 0 y1 y2 y3 y4 y5 y6 y7 y8 y9
        //对新加入的br_index个团点信息分类后放到br_mem[i][0][0],br_mem[i][0][1]中
        for (i=0; i<br_index; i++)	//新加入的10个团点
        {
            for (j=0; j<BR_GROUP; j++)	//j之前的团点10组，这十组团点分别往前找
            {
                k=0;
                if (empty_mem[j] == 0) continue;	//全空跳过，找下一组团点。
                while (k<20 && br_mem[j][k][0] == 0 && br_mem[j][k][1] == 0) k++;//针对每个团点向之前20帧图像找，找到不为0的地方
                if (k == 20) empty_mem[j] = 0;       //其实为空更新状态
                //x变化小于x_offset,y变化小于y_offset
                else
                {
                    if (myabs((int8)br[i].center_x-(int8)br_mem[j][k][0]) < x_offset 
                     && myabs((int8)br[i].center_y-(int8)br_mem[j][k][1]) < y_offset)
                    {
                        br_mem[j][0][0] = br[i].center_x;
                        br_mem[j][0][1] = br[i].center_y;
                        break;//此处break可优化，更符合哪个之前的团点？此处找到符合要求的直接break
                    }
                }
            }
            if (j == BR_GROUP)//10组都处理完了，没有合适的，放到全空的里，没有的话改代码
            {
                for (j=0; j<BR_GROUP; j++)
                {
                    if (empty_mem[j] == 0)
                    {
                        br_mem[j][0][1] = br[i].center_y;
                        br_mem[j][0][0] = br[i].center_x;
                        empty_mem[j] = 1; //empty_mem更新为1
                        break;
                    }
                }
            }
        }//至此新加入的br信息已经全部加入br_mem

        //确定10组团点优先级
        for (i=0; i<BR_GROUP; i++) //10组团点
        {
            if (empty_mem[i] == 0)//此组为空
            {
                br_grade[i][2] = 0;	//优先级最低
                br_grade[i][0] = br_grade[i][1] = 0;
                bcn_state[i] = BCN_NONE;
            }
            else
            {
                //得到理论闪烁频率
                for (j=0; j<FLICK_FRAME; j++) //找第一帧不为0的，根据第一个的y位置确定应该的闪烁频率
                {
                    if (br_mem[i][j][0] != 0 || br_mem[i][j][1] != 0)//从前往后找坐标不为0，且此坐标为最新坐标
                    {
                        if (j == 0)
                        {
                            bcn_state[i] = BCN_LIGHT;//灯暗
                        }
                        else 
                        {
                            bcn_state[i] = BCN_DARK;//灯亮
                        }
                        br_grade[i][0] = br_mem[i][j][0];
                        br_grade[i][1] = br_mem[i][j][1];
                        theory_light = y_light[br_mem[i][j][1]];	//根据y的位置更新theory_light（理论次数）
                        break;
                    }
                }//至此得到理论次数
                //得到实际频率
                real_light = 0;
                for (k=FLICK_FRAME-1; k>=j; k--)
                {
                    if (br_mem[i][k][0] != 0 || br_mem[i][k][1] != 0)//从后往前找坐标不为0
                    {
                        real_light++;
                    }
                }//至此得到实际次数
                
                //根据确定的theory_light确定此组团点的优先级
                if (myabs(theory_light - real_light) < 3)
                {
                    br_grade[i][2] = 3;
                }
                else if (myabs(theory_light - real_light) < 6)
                {
                    br_grade[i][2] = 2;
                }
                else if (myabs(theory_light - real_light) < 9)
                {
                    br_grade[i][2] = 1;
                }
                else
                {
                    br_grade[i][2] = 0;
                }//至此优先级确定完毕
            }
        }//至此10组团点优先级判断完毕

        //取最高优先级作为信标位置
        for (i=3; i>=1; i--)
        {
            for (j=0; j<BR_GROUP; j++)
            {
                if (br_grade[j][2] == i)//优先级满足要求
                {
                    *bcn_x = br_grade[j][0];
                    *bcn_y = br_grade[j][1];
                    return;
                }
            }
        }
        *bcn_x = 0;
        *bcn_y = 0;
        return;
    }
}






/*
@brief:对图像进行差分并累加,差分得到的是移动或者闪烁的点
@param img:当前图像
@param preimg:前一帧图像
@param diffimg:差分图 
@param Threshold:阈值
*/
void accumulateForeground(uint8 *img,uint8 *preimg,uint8 *diffimg,uint8 Threshold)
{
   int16 i,j,k;
  // uint8 Icount=0;//累加帧数
   static uint8 first=1;
   uint8 temp;
   uint8 cn=1;
   uint16 imgstep=CAMERA_W*cn;
   
   //-cn是因为在每次通道循环计算结束后，都指向图像该元素的最后一个通道
   for(j=0;j<CAMERA_H;j++,img+=imgstep-cn,preimg+=imgstep-cn,diffimg+=imgstep-cn)//遍历行
   {
     
     for(k=0;k<cn;k++,img++,preimg++,diffimg++)//分通道
     {
         for(i=0;i<imgstep;i+=cn)//遍历列
         {
             //判断是否为第一帧图像
             if(!first)//并非第一帧图像
             {
                  temp=(uint8)abs(img[i]-preimg[i]);
                  temp=temp>Threshold?255:0;
                  diffimg[i]=temp;
                  preimg[i]=img[i];
                  img[i]=img[i]>Threshold?255:0;//当前图像二值化
             }
            else preimg[i]=img[i];//若为第一帧图像暂时只保存，不处理
         }
     }
   }
   first=0;
}

#define GrayScale   256
int pixel[GrayScale]={0}; 

int MyOSTU(int width,int height,uint8 *Image)
{
	int threshold=0;
	int32_t sum_gray=0;
	int32_t sum_pix_num=0;
	int32_t pl_pix_num=0;
	int32_t p2_pix_mum=0;
	int32_t p1_sum_gray=0;
	float m1=0;
	float m2=0;
	float V=0;
	float variance=0;
	int i,j,k=0;
	
	for(i = 0;i<256;i++)
		pixel[i] = 0;
    for(i = 0; i < height; i++)  
    {  
        for(j = 0;j < width;j++)  
        {  
            pixel[(int)Image[i * width + j]]++;  
        }  
    } 

		for(k=0;k<GrayScale;k++)
		{
			sum_gray+=k*pixel[k];//»Ò¶ÈÖ±·½Í¼ÖÊÁ¿¾Ø
			sum_pix_num+=pixel[k];//×ÜÏñËØ¸öÊý
		}	
		
		for(k=0;k<GrayScale-1;k++)
		{
			pl_pix_num+=pixel[k];//µÚÒ»²¿·ÖÏñËØ¸öÊý
			p2_pix_mum=sum_pix_num-pl_pix_num;//µÚ¶þ²¿·ÖÏñËØ¸öÊý
			p1_sum_gray+=k*pixel[k];   //µÚÒ»²¿·ÖÖÊÁ¿¾Ø
			m1=(float)p1_sum_gray/pl_pix_num;//µÚÒ»²¿·Ö»Ò¶È¾ùÖµ
			m2=(float)(sum_gray-p1_sum_gray)/p2_pix_mum;//µÚ¶þ²¿·Ö»Ò¶È¾ùÖµ
			
			V=pl_pix_num*p2_pix_mum*(m1-m2)*(m1-m2);
			
			if(V>variance)//½«Àà¼ä·½²î½Ï´óÊ±µÄ»Ò¶ÈÖµ×÷ÎªãÐÖµ
			{
				variance=V;
				threshold=k;
			}
		}
		return threshold;
}

