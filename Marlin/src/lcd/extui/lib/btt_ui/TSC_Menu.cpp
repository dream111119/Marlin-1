/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"
#if ENABLED(BTT_UI_SPI)
#include "../../ui_api.h"
#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../libs/buzzer.h"
#include "../../../../module/printcounter.h"
#include "TSC_Menu.h"


MENU  infoMenu = {.menu = {menuStatus}, .cur = 0};  // Menu structure
FP_MENU menuFrontCallBack = 0; 

TSCBoot boot;


#if defined(ARDUINO_ARCH_STM32)
  #ifdef __STM32F1__
    void BuzzerTone(const uint16_t fre,const uint16_t ms)
    {
      uint32_t nowT = millis();
      uint16_t fredT=(uint16_t)(fre/2);
      
      for(;millis()<(nowT+ms);)
      {
        WRITE(BEEPER_PIN, HIGH);
        delay_us(fredT);
        WRITE(BEEPER_PIN, LOW);
        delay_us(fredT);
     }
    }
    #define Buzzer_TurnOn(fre, ms) BuzzerTone(fre,ms)
  #else
    #define Buzzer_TurnOn(fre, ms) BUZZ(ms, fre)
  #endif
#else
 #define Buzzer_TurnOn(fre, ms) BUZZ(ms, fre)
#endif

void Buzzer_play(SOUND sound){
switch (sound)
{
case sound_ok:
  Buzzer_TurnOn(3800,40);
  Buzzer_TurnOn(0,20);
  Buzzer_TurnOn(5500,50);
  break;
case sound_success:

  Buzzer_TurnOn(3500,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(3500,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(3500,50);
  break;
case sound_cancel:
  Buzzer_TurnOn(5500,50);
  Buzzer_TurnOn(0,20);
  Buzzer_TurnOn(3800,40);
  break;
  case sound_notify:
  Buzzer_TurnOn(3090,50);
  Buzzer_TurnOn(0,50);
  Buzzer_TurnOn(4190,50);
  break;
case sound_error:
   Buzzer_TurnOn(2200,200);
   Buzzer_TurnOn(0,60);
   Buzzer_TurnOn(2200,200);
   Buzzer_TurnOn(0,60);
   Buzzer_TurnOn(2200,200);
  break;
case sound_keypress:
default:
  Buzzer_TurnOn(LCD_FEEDBACK_FREQUENCY_HZ, LCD_FEEDBACK_FREQUENCY_DURATION_MS);
  break;
}
}



// Store gcode cmd to gcode queue
// If the gcode queue is full, reminde in title bar.
bool storeCmd(const char *cmd)
{  
  if (queue.length >= BUFSIZE) {  
    reminderMessage(LABEL_BUSY, STATUS_BUSY);
    return false;
  }
  queue.enqueue_one_now(cmd);
  return true;
}

// Store gcode cmd to gcode queue
// If the gcode queue is full, reminde in title bar,  waiting for available queue and store the command.
void mustStoreCmd(const char *cmd)
{  
  if (queue.length >= BUFSIZE) reminderMessage(LABEL_BUSY, STATUS_BUSY);
  queue.enqueue_one_now(cmd);
}

extern SETTINGS lastSettings; // Last Settings para
extern uint32_t TSC_Para[7],
                lastTSC_Para[7];
void LCD_Setup() {
  
  XPT2046_Init();
  W25Qxx_Init();
  readStoredPara();
  LCD_Init();

 #ifdef LCD_ENCODER_SUPPORT
  HW_EncoderInit();
 #endif

 #ifdef LOGO_DISPLAY
  LOGO_ReadDisplay();
 #endif
  storeCmd("M420 S");
  //storeCmd("M420 Z30");
  boot.scanUpdates();
   
  // if(readStoredPara() == false) // Read settings parameter
  // {
    TSC_Calibration();
    storePara();
    
    #ifdef LOGO_DISPLAY
      LOGO_ReadDisplay();
    #endif
  //}
  memcpy(&lastSettings, &infoSettings, sizeof(SETTINGS));
  memcpy(lastTSC_Para, TSC_Para, sizeof(TSC_Para));
  #ifdef FILAMENT_RUNOUT_SENSOR
  ExtUI::setFilamentRunoutEnabled(infoSettings.runout);
  #endif
}

 //  GUI_Clear(0Xf800);
  // SERIAL_ECHO("SCAN-OK\r\n");
  // while(1);
static bool hasPrintingMenu = false;
void menuUpdate(void) {
  static FP_MENU lastMenu = 0;
  if (lastMenu != infoMenu.menu[infoMenu.cur]) { // Menu switch
    lastMenu = infoMenu.menu[infoMenu.cur];
    lastMenu();
    #ifdef TEMP_LOAG
    temp_logo_display();
    #endif
    #ifdef TEMP_LOAG
     if((infoMenu.menu[infoMenu.cur]!=menuPrinting)&&(infoMenu.menu[infoMenu.cur]!=menuStatus)&&((infoMenu.menu[infoMenu.cur]!=menuMove)))
     {
      GUI_DispString(PEN_LOAG_X- 6*BYTE_WIDTH, TEMP_LOAG_Y, (uint8_t *)"T0:"); 
      GUI_DispString(PEN_LOAG_X, TEMP_LOAG_Y, (uint8_t *)"/"); // Ext value
      GUI_DispString(BED_LOAG_X- 7*BYTE_WIDTH, TEMP_LOAG_Y, (uint8_t *)"Bed:"); 
      GUI_DispString(BED_LOAG_X, TEMP_LOAG_Y, (uint8_t *)"/"); // Bed value
      GUI_DispDec(PEN_LOAG_X + BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degTargetHotend(0), 3, LEFT);
      GUI_DispDec(BED_LOAG_X + BYTE_WIDTH, TEMP_LOAG_Y, thermalManager.degTargetBed(), 3, LEFT);
     }
    #endif
  }

  if (menuFrontCallBack) { // Call back is vaild
    menuFrontCallBack();
  }
  
 #ifdef LCD_ENCODER_SUPPORT
    loopCheckEncoderSteps(); //check change in encoder steps  
 #endif

  if (isPrinting()) {
    if (!hasPrintingMenu) {
      hasPrintingMenu = true;
      infoMenu.menu[++infoMenu.cur] = menuPrinting;
    }
  } else {
     loopFrontEnd();
    if (hasPrintingMenu) {
      if(infoMenu.menu[infoMenu.cur] == menuStopMotor)
      {
       if(LCD_home_flag==false)
        {
            hasPrintingMenu = false;
              while (infoMenu.menu[infoMenu.cur] != menuPrinting && infoMenu.cur) {
              infoMenu.cur--;
              }
            
            if (infoMenu.cur)
            {
              infoMenu.cur--;
            } 
        }
      }
      else
      {
         hasPrintingMenu = false;
        while (infoMenu.menu[infoMenu.cur] != menuPrinting && infoMenu.cur) {
          infoMenu.cur--;
        }
        
        if (infoMenu.cur) infoMenu.cur--;
        // Abort print not print completed, need not popup
        if (!card.flag.abort_sd_printing) {
          if(infoSettings.auto_off) {
            // Auto shut down after printing
            infoMenu.menu[++infoMenu.cur] = menuShutDown;
          } else {
            // popup print completed
            Buzzer_play(sound_success);
            popupReminder(textSelect(LABEL_PRINT), textSelect(LABEL_PRINTING_COMPLETED));
          }
         }
      }
    }
  }
}

void menuSetFrontCallBack(FP_MENU cb) {
  menuFrontCallBack = cb;
}

#endif

