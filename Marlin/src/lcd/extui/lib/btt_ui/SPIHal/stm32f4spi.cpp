#if defined(ARDUINO_ARCH_STM32)
#ifndef __STM32F1__
#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
#include "SPI.h"
#include "share.h"
#include "stm32f4xx_hal_spi.h"
#include "pinconfig.h"


void SPI_INIT(void)
{
    __HAL_RCC_GPIOB_IS_CLK_ENABLED();	 
    __HAL_RCC_SPI2_CLK_ENABLE();
    
    pinmap_pinout(PB_15,PinMap_SPI_MOSI);
    pinmap_pinout(PB_14, PinMap_SPI_MISO);
    pinmap_pinout(PB_13, PinMap_SPI_SCLK);

    SPI2->CR1|=0<<10;		
    SPI2->CR1|=1<<9; 	
    SPI2->CR1|=1<<8;  

    SPI2->CR1|=1<<2; 	
    SPI2->CR1|=0<<11;	
    SPI2->CR1|=1<<1; 		
    SPI2->CR1|=1<<0; 		
   
    SPI2->CR1|=3<<3; 		
    SPI2->CR1|=0<<7; 		
	SPI2->CR1|=1<<6; 	
    
    SPI_SetSpeed(0);
    SPI_ReadWriteByte(0xff);
}

void SPI_SetSpeed(uint8_t SpeedSet)
{
	SpeedSet&=0X07;		
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet<<3;	  
	SPI2->CR1|=1<<6; 		  
}

uint8_t SPI_ReadWriteByte(uint8_t TxData)
{		
	uint16_t retry=0;				 
	while((SPI2->SR&1<<1)==0)		
	{
		retry++;
		if(retry>=0XFFFE)return 0; 	
	}			  
	SPI2->DR=TxData;	 	  	
	retry=0;
	while((SPI2->SR&1<<0)==0) 		
	{
		retry++;
		if(retry>=0XFFFE)return 0;	
	}	  						    
	return SPI2->DR;        
}


uint16_t SPI_Write2Byte(uint16_t TxData)
{		
	uint16_t retry=0;				 
	while((SPI2->SR&1<<1)==0)		
	{
		retry++;
		if(retry>=0XFFFE)return 0; 	
	}			  
	SPI2->DR=TxData;	 	  	
	retry=0;
	while((SPI2->SR&1<<0)==0) 		
	{
		retry++;
		if(retry>=0XFFFE)return 0;	
	}	  						    
	return SPI2->DR;          		
}


void  SPI8BITMODE(void)
{
  SPI2->CR1|=(0<<10)|(1<<9)|(1<<8)|(1<<2)|(0<<11)|(1<<1)|(1<<0)|(3<<3)|(0<<7)|(1<<6);
}

void  SPI16BITMODE(void)
{
	SPI2->CR1|=(0<<10)|(1<<9)|(1<<8)|(1<<2)|(1<<11)|(1<<1)|(1<<0)|(3<<3)|(0<<7)|(1<<6);		 
}


void SPIReadBuff(uint8_t* buff,uint32_t len)
{
     uint16_t retry=0;	
   for(uint32_t i=0;i<len;i++)
   {
	      retry=0;				 
			while((SPI2->SR&1<<1)==0)		
			{
				retry++;
				if(retry>=0XFFFE)break; 	
			}			  
			SPI2->DR=0XFF;	 	  	
			retry=0;
			while((SPI2->SR&1<<0)==0) 		
			{
				retry++;
				if(retry>=0XFFFE)break;	
			}	  						    
	*buff=(SPI2->DR);
	    buff++;
   }

}

void SPIWriteBuff(uint8_t* buff,uint32_t len)
{
        uint16_t retry=0;
		uint8_t temp=0;	
   for(uint32_t i=0;i<len;i++)
   {				 
			while((SPI2->SR&1<<1)==0)		
			{
				retry++;
				if(retry>=0XFFFE)break; 	
			}			  
			SPI2->DR=*buff;	 	  	
			retry=0;
			while((SPI2->SR&1<<0)==0) 		
			{
				retry++;
				if(retry>=0XFFFE)break;	
			}	  						    
	    temp=(SPI2->DR);
	    buff++;
   }
 
}

void SPIWriteBuff(uint16_t* buff,uint32_t len)
{
    for(uint32_t i=0;i<len;i++)
	{
       SPI_ReadWriteByte((*buff)>>8);
       SPI_ReadWriteByte((*buff)&0xFF);
	   buff++;
	}
}
void SPIWriteBuff(uint16_t buff,uint32_t len)
{
    for(uint32_t i=0;i<len;i++)
	{
       SPI_ReadWriteByte(buff>>8);
       SPI_ReadWriteByte(buff&0xFF);
	}
}
#endif
#endif
#endif

