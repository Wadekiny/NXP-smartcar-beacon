#ifndef _FLASH_H_
#define _FLASH_H_


/*
//K66FX1M0VLQ18     Flash参数     1M的flash，256K ROM API ,256K RAM
#define     FLASH_SECTOR_SIZE       (4*1024)                //扇区大小 为 4k 字节
#define     FLASH_SECTOR_NUM        (256)                   //扇区数

#define     FLASH_ALIGN_ADDR        8                       //地址对齐整数倍
typedef     uint64                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型
*/

#define FLASH_SECTOR_STORE      255
#define FLASH_OFFSET            8


void FLASH_Write();
void FLASH_Read();
void FLASH_Write0();
void FLASH_Read0();

#endif
