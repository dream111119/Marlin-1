#include "../TSC_Menu.h"
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(BTT_UI_SPI)
#include "Flash.h"


void FlashWrite(uint8_t *data, uint32_t len)
{
    W25Qxx_EraseSector(SIGN_ADDRESS);
    W25Qxx_WriteBuffer(data,SIGN_ADDRESS,len);
}
void FlashRead(uint8_t *data, uint32_t len)
{
  W25Qxx_ReadBuffer(data,SIGN_ADDRESS,len);
}

#endif

