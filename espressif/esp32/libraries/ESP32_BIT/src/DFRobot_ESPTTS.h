/*!
 * @file DFRobot_ESPTTS.h
 * @brief Define the basic structure of class DFRobot_ESPTTS 
 * @details This library is used to drive the analog electrical conductivity meter to measure solution EC. 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2022-5-5
 * @https://github.com/DFRobot/DFRobot_ESPTTS
 */

#include <driver/i2s.h>
#include "esp_tts.h"
#include "esp_tts_voice_xiaole.h"
#include "esp_tts_voice_template.h"
#include "esp_tts_player.h"
#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>



#ifndef _DFROBOT_ESPTTS_H
#define _DFROBOT_ESPTTS_H


//#define ENABLE_DBG
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif
#define I2S_NUM         (i2s_port_t)(0)
#define SAMPLE_RATE1     (16000)
typedef struct {

  const char *prompt;
  uint8_t state;

}sTts_t;
class DFRobot_ESPTTS{

public:
  DFRobot_ESPTTS();
  bool begin();
  bool busy();
  uint8_t speak(const char *prompt);

private:
  uint8_t _i2s_bck;
  uint8_t _i2s_ws;
  uint8_t _i2s_data;
  uint8_t _i2s_en;
};



#endif