#ifndef _FLASHSTORE_H_
#define _FLASHSTORE_H_

#include "../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)

bool readStoredPara(void);
void storePara(void);

#endif

#endif
