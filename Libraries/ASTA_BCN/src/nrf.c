#include "nrf.h"
#include "include.h"
#include "ALLBCN.h"

   
 /*!   
  *  @brief      main����   
  *  @since      v6.0   
  *  @note       SPI ���� NRF24L01+   
  */   
void NRF_Init()
{
    while(!nrf_init());                  //��ʼ��NRF24L01+ ,�ȴ���ʼ���ɹ�Ϊֹ    
//    {    
//     printf("\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\n");    
//    }  
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //���� PORTE ���жϷ�����Ϊ PORTE_VECTORn    
    enable_irq(PORTE_IRQn);    
}
void NRF_Send()
{
    uint8* buff= NULL;
    if(nrf_tx(buff,DATA_PACKET) == 1 )          //����һ�����ݰ���buff����Ϊ32�ֽڣ�    
    {    
    //�ȴ����͹����У��˴����Լ��봦������    

        while(nrf_tx_state() == NRF_TXING);         //�ȴ��������    

        if( NRF_TX_OK == nrf_tx_state () )    
        {    
//            printf("\n���ͳɹ�:%d",i);    
//            i++;                                    //���ͳɹ����1������֤�Ƿ�©��    
        }    
        else   
        {    
//            printf("\n����ʧ��:%d",i);    
        }    
    }    
//    else   
//    {    
//        printf("\n����ʧ��:%d",i);    
//    }    
}

