///*****************************************************************
//Copyright: 2012-2016, Veepoo Tech. Co., Ltd.
//File name: 		spi_master.c
//Description: 
//Author: 			WJ
//Version: 
//Date: 			2017-03-09
//History: 
//*****************************************************************/
//#include "stdint.h"
//#include "nrf_delay.h"
//#include "spi_master.h"
//#include "board.h"

//#define PGA_SPI_CSO        PGA_CS_PIN
//#define PGA_SPI_SCK        PGA_SCLK_PIN
//#define PGA_SPI_MOSI       PGA_MOSI_PIN
//#define PGA_SPI_MISO       0

//#define SPI_CSO_HIGH()   do { NRF_GPIO->OUTSET = (1UL << PGA_SPI_CSO);   }   while(0)   /*!< Pulls SCL line high */
//#define SPI_CSO_LOW()    do { NRF_GPIO->OUTCLR = (1UL << PGA_SPI_CSO);   }   while(0)   /*!< Pulls SCL line low  */
//#define SPI_SCK_HIGH()   do { NRF_GPIO->OUTSET = (1UL << PGA_SPI_SCK);   }   while(0)   /*!< Pulls SCL line high */
//#define SPI_SCK_LOW()    do { NRF_GPIO->OUTCLR = (1UL << PGA_SPI_SCK);   }   while(0)   /*!< Pulls SCL line low  */
//#define SPI_SMO_HIGH()   do { NRF_GPIO->OUTSET = (1UL << PGA_SPI_MOSI);  }   while(0)   /*!< Pulls SDA line high */
//#define SPI_SMO_LOW()    do { NRF_GPIO->OUTCLR = (1UL << PGA_SPI_MOSI);  }   while(0)   /*!< Pulls SDA line low  */
//#define SPI_SMI_HIGH()   do { NRF_GPIO->OUTSET = (1UL << PGA_SPI_MISO);  }   while(0)   /*!< Pulls SDA line low  */
//#define SPI_SMI_LOW()    do { NRF_GPIO->OUTCLR = (1UL << PGA_SPI_MISO);  }   while(0)   /*!< Pulls SDA line low  */
//#define SPI_SMO_OUT(x)	 do {	if(x){SPI_SMO_HIGH();}	else{SPI_SMO_LOW();}}						while(0)


//void Spi_Init(void)
//{
//    PIN_LOW(PGA_SPI_CSO);   
//    PIN_HIGH(PGA_SPI_CSO);
//    
//    PIN_LOW(PGA_SPI_MOSI);
//    PIN_LOW(PGA_SPI_SCK);  
//    PIN_LOW(PGA_SPI_SCK); 
//    PIN_LOW(PGA_SPI_MOSI);
//}


//void Spi_WriteData(uint8_t *data, uint16_t data_len)
//{
//    uint8_t i = 0;
//    uint8_t j = 0;
//    
//    SPI_SCK_LOW();	
////    SPI_SMO_OUT(0x01);
//    nrf_delay_us(5);
//    
//    while( j < data_len)
//    {
//        for(i = 0; i < 8; i++) 
//        {
//            if(*data & (0x80 >> i))
//            {
//                SPI_SMO_OUT(0x01);
//            }
//            else
//            {
//                SPI_SMO_OUT(0x00);
//            }           
//            SPI_SCK_LOW();           
//            nrf_delay_us(4);
//            SPI_SCK_HIGH();            
//            nrf_delay_us(4);
//        }
//        j++;
//        data++;
//    }
//    SPI_SCK_LOW(); 
//    SPI_SMO_OUT(0x00);
//    nrf_delay_us(5);
//}


//void Spi_Select(uint8_t flag)
//{
//    if(flag)
//    {
//        SPI_CSO_LOW();
//        nrf_delay_us(5);
//    }
//    else
//    {       
//        nrf_delay_us(5);
//        SPI_CSO_HIGH(); 
//    }   
//}
