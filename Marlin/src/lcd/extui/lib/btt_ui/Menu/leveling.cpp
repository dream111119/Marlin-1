#include "../../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../TSC_Menu.h"


#ifdef AUTO_BED_LEVELING_BILINEAR

#include "../../../../../feature/bltouch.h"

#ifdef  FIX_MOUNTED_PROBE

#define LEVEL_PREHEAT_TEMP 140 //Preheating temperature
#define LEVEL_PREHEAT_BED 60 

extern uint16_t foreGroundColor;

const MENUITEMS Start_autoLevelingItems = {
// title
LABEL_ABL,
// icon                        label
 {{ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_EM_STOP,              LABEL_EMERGENCYSTOP},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},}
};

void menuCallBackLeveling(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  
  switch(key_num)
  {
    case KEY_ICON_6:
      flashFirmware(1);
      break;
    default:break;
  }

  if(G29_flag)
  {
     G29_flag=false;
     infoMenu.cur--;
     infoMenu.cur--;
  }
}
void menuLeveling()
{
  menuDrawPage(&Start_autoLevelingItems);
  foreGroundColor=RED;
  GUI_DispString(LCD_WIDTH_PIXEL/4+4*BYTE_WIDTH, LCD_HEIGHT_PIXEL/2-4*BYTE_HEIGHT, textSelect(LABEL_LEVELING_ING));
  foreGroundColor = FK_COLOR;
  menuSetFrontCallBack(menuCallBackLeveling);
}

void menuCallBackStartLeveling(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  
  switch(key_num)
  {
    case KEY_ICON_6:
      flashFirmware(1);
      break;
    default:break;
  }

  if((ExtUI::getActualTemp_celsius(ExtUI::E0)>=(LEVEL_PREHEAT_TEMP-5))&&(ExtUI::getActualTemp_celsius(ExtUI::E0)<=(LEVEL_PREHEAT_TEMP+5)))
  {
    if((ExtUI::getActualTemp_celsius(ExtUI::BED)>=(LEVEL_PREHEAT_BED)-1)&&(ExtUI::getActualTemp_celsius(ExtUI::BED)<=(LEVEL_PREHEAT_BED+3)))
    {
      queue.enqueue_one_now("G28");
      queue.enqueue_one_now("G29");
      queue.enqueue_one_now("M500");
      infoMenu.menu[++infoMenu.cur] = menuLeveling;
    }
  }
}
void menuStartLeveling()
{
  menuDrawPage(&Start_autoLevelingItems);
  foreGroundColor=RED;
  GUI_DispString(LCD_WIDTH_PIXEL/4+4*BYTE_WIDTH, LCD_HEIGHT_PIXEL/2-6*BYTE_HEIGHT, textSelect(LABEL_LEVEL_TEMP));
  GUI_DispString(LCD_WIDTH_PIXEL/4+5*BYTE_WIDTH, LCD_HEIGHT_PIXEL/2-4*BYTE_HEIGHT, textSelect(LABEL_LEVEL_BED_TEMP));
  foreGroundColor = FK_COLOR;
  menuSetFrontCallBack(menuCallBackStartLeveling);
}

#endif


const MENUITEMS autoLevelingItems = {
// title
LABEL_ABL,
// icon                        label
 {{ICON_LEVELING,              LABEL_ABL},

  #ifdef  FIX_MOUNTED_PROBE
  {ICON_X_HOME,               LABEL_X_HOME},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  {ICON_BACKGROUND,           LABEL_BACKGROUND},
  #else
  {ICON_BLTOUCH_DEPLOY,        LABEL_BLTOUCH_DEPLOY},
  {ICON_BLTOUCH_STOW,          LABEL_BLTOUCH_STOW},
  {ICON_BLTOUCH_TEST,          LABEL_BLTOUCH_TEST},
  {ICON_PROBE_OFFSET,          LABEL_PROBE_OFFSET},
  #endif
  {ICON_BABYSTEP,              LABEL_BABYSTEP},
  {ICON_EM_STOP,               LABEL_EMERGENCYSTOP},
  {ICON_BACK,                  LABEL_BACK},}
};

void menuCallBackAutoBedLeveling(void)
{
  KEY_VALUES key_num = menuKeyGetValue();
  switch(key_num)
  {
    case KEY_ICON_0:
      #ifdef  FIX_MOUNTED_PROBE
      
        //  thermalManager.setTargetHotend(LEVEL_PREHEAT_TEMP, 0);
        //  thermalManager.setTargetBed(LEVEL_PREHEAT_BED);
        //  infoMenu.menu[++infoMenu.cur] = menuStartLeveling;
        //  queue.enqueue_one_now("G28");
        //  queue.enqueue_one_now("G29");
        //  queue.enqueue_one_now("M500");
      #else
      storeCmd("G28");
      storeCmd("G29");
      storeCmd("M500");
      #endif
    break;
  #ifdef  FIX_MOUNTED_PROBE
    case KEY_ICON_1:
      storeCmd("G28 X");
      break;
  #else
    case KEY_ICON_1:
      storeCmd("M280 P0 S10\n");
      break;
    case KEY_ICON_2:
      storeCmd("M280 P0 S90\n");
      break;
    case KEY_ICON_3:
      storeCmd("M280 P0 S120\n");
      break;
    case KEY_ICON_4:
      infoMenu.menu[++infoMenu.cur] = menuProbeOffset;
      break;
  #endif

    case KEY_ICON_5:
      infoMenu.menu[++infoMenu.cur] = menuBabyStep;
      break;
    case KEY_ICON_6:
      flashFirmware(1);
      break;
    case KEY_ICON_7:
      infoMenu.cur--; break;
    default:break;
  }
}

void menuAutoLeveling()
{
  menuDrawPage(&autoLevelingItems);
  menuSetFrontCallBack(menuCallBackAutoBedLeveling);
}

#define LEVEL_BUTTON_NUM 1
static BUTTON *windowXButton =  NULL;
static uint16_t buttonXNum = 0;

void LevelDrawPage(BUTTON *btn, const uint8_t *title, const uint8_t *context, const uint8_t *yes, const uint8_t *no)
{
  buttonXNum = 0;
  windowXButton = btn;
  if(yes)
  {
    windowXButton[buttonXNum++].context = yes;
  }
  if(no)
  {
    windowXButton[buttonXNum++].context = no;
  }
  
  TSC_ReDrawIcon = windowReDrawButton;
  GUI_DrawWindow(&window, title, context);
  
  for(uint8_t i = 0; i < buttonXNum; i++)
    GUI_DrawButton(&windowXButton[i], 0);    
}

void menuCallBackLevel(void)
{
  uint16_t key_num = KEY_GetValue(LEVEL_BUTTON_NUM, &singleBtnRect);
  switch(key_num)
  {            
    case KEY_POPUP_CONFIRM: 
      infoMenu.cur--; 
      break;
    
    default:
      break;            
  }
}

void menuLevel(void)
{ 
   queue.enqueue_one_now("G28 X");
   queue.enqueue_one_now("G28 Y");
   menuSetFrontCallBack(menuCallBackLevel);
}

void LevelReminder(uint8_t* info, uint8_t* context)
{
  LevelDrawPage(&bottomSingleBtn , info, context, textSelect(LABEL_CONFIRM), NULL);    
  if(infoMenu.menu[infoMenu.cur] != menuLevel)
  {
    infoMenu.menu[++infoMenu.cur] = menuLevel;
  }
}

void Leveling_ovel()
{
 //  LevelReminder(textSelect(LABEL_LEVELING), textSelect(LABEL_LEVELING_OVEL));
}
#else

#endif
#endif
