#ifndef _Flash_H
#define _Flash_H

#include "../../../../../inc/MarlinConfig.h"
#if ENABLED(BTT_UI_SPI)
 #include <stdint.h>

 void FlashRead(uint8_t *data, uint32_t len);
 void FlashWrite(uint8_t *data, uint32_t len);

#endif
#endif
