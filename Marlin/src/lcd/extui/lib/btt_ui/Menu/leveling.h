#ifndef _LEVELING_H_
#define _LEVELING_H_

#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

void menuManualLeveling(void);
void menuAutoLeveling(void);
void LevelReminder(uint8_t* info, uint8_t* context);
void Leveling_ovel();
#endif
#endif

