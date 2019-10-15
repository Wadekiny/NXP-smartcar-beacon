#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "ALLBCN.h"

struct sBrightRegion
{	
	uint8 center_x; //中心x
	uint8 center_y;	//中心y
	uint16 counter;	//亮点个数
	uint32 sum_x;	//x和
	uint32 sum_y;	//y和
};

#define BCN_DARK    2
#define BCN_LIGHT   1
#define BCN_NONE    0
#define DARK    0
#define LIGHT   1

typedef struct
{
    uint8 x;
    uint8 y;
}Point;
int MyOSTU(int width,int height,uint8 *Image);
void IMAGE_TempCoord(uint8 * bcn_x, uint8 * bcn_y);
void IMAGE_MedianFilter(uint8 dst[CAMERA_H][CAMERA_W], uint16 img_filter[CAMERA_H][CAMERA_W]);
void IMAGE_FastFilter(uint8 dst[ROW][COL],uint16 img_filter[ROW][COL]);
void IMAGE_CreatPoint();
void IMAGE_ChangeStructBr(struct sBrightRegion br[10],uint8 dst[CAMERA_H][CAMERA_W]);
void IMAGE_GetBinary(uint8 img_before[ROW][COL],uint8 img_after[ROW][COL]);
void IMAGE_GetFlick(uint8* bcn_x, uint8* bcn_y);
void CreatCoord(uint8 img[ROW][COL],uint8* bcn_x, uint8* bcn_y);
//void IMAGE_FindCoord(uint8* bcn_x, uint8* bcn_y);
//void IMAGE_MvCoord();
void accumulateForeground(uint8 *img,uint8 *preimg,uint8 *diffimg,uint8 Threshold);

#endif