#include "nrf.h"
#include "include.h"
#include "ALLBCN.h"

   
 /*!   
  *  @brief      main函数   
  *  @since      v6.0   
  *  @note       SPI 驱动 NRF24L01+   
  */   
void NRF_Init()
{
    while(!nrf_init());                  //初始化NRF24L01+ ,等待初始化成功为止    
//    {    
//     printf("\n  NRF与MCU连接失败，请重新检查接线。\n");    
//    }  
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //设置 PORTE 的中断服务函数为 PORTE_VECTORn    
    enable_irq(PORTE_IRQn);    
}
void NRF_Send()
{
    uint8* buff= NULL;
    if(nrf_tx(buff,DATA_PACKET) == 1 )          //发送一个数据包：buff（包为32字节）    
    {    
    //等待发送过程中，此处可以加入处理任务    

        while(nrf_tx_state() == NRF_TXING);         //等待发送完成    

        if( NRF_TX_OK == nrf_tx_state () )    
        {    
//            printf("\n发送成功:%d",i);    
//            i++;                                    //发送成功则加1，可验证是否漏包    
        }    
        else   
        {    
//            printf("\n发送失败:%d",i);    
        }    
    }    
//    else   
//    {    
//        printf("\n发送失败:%d",i);    
//    }    
}

