#ifndef _SHARE_H
#define _SHARE_H
#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)


void SPI8BITMODE();
void SPI16BITMODE();
void SPI_INIT(void);

uint8_t SPI_ReadWriteByte(uint8_t TxData);

#if defined(ARDUINO_ARCH_STM32)

void SPI_SetSpeed(uint8_t SpeedSet);

uint16_t SPI_Write2Byte(uint16_t TxData);
void SPIReadBuff(uint8_t* buff,uint32_t len);
void SPIWriteBuff(uint8_t* buff,uint32_t len);
void SPIWriteBuff(uint16_t* buff,uint32_t len);
void SPIWriteBuff(uint16_t buff,uint32_t len);

#ifdef __STM32F1__
void SPIReadBuff(uint16_t* buff,uint32_t len);
#endif

#elif defined(TARGET_LPC1768)

void SPI_Write2Byte(uint16_t TxData);
void SPIWriteBuff(uint16_t buff,uint32_t len);
void SPIWriteBuff(uint16_t* buff,uint32_t len);
uint16_t SPI_ReadWrite2Byte(uint16_t TxData);

void SPIReadBuff(uint16_t* buff,uint32_t len,uint16_t mode);
void SPIReadBuff(uint16_t* buff,uint32_t len);

#endif
#endif
#endif
