/*!
 * @file DFRobot_ESPTTS.cpp
 * @brief Define the basic structure of class DFRobot_ESPTTS 
 * @details This library is used to drive the analog electrical conductivity meter to measure solution EC. 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2022-5-5
 * @https://github.com/DFRobot/DFRobot_ESPTTS
 */

#include "DFRobot_ESPTTS.h"
bool _busy = false;
static QueueHandle_t g_result_que = NULL;
esp_tts_handle_t *tts_handle;
esp_tts_voice_t *voice =&esp_tts_voice_xiaole;

DFRobot_ESPTTS::DFRobot_ESPTTS(){


}
void i2sInit1()
{
i2s_config_t i2s_config = {
     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
     .sample_rate = SAMPLE_RATE1,
     .bits_per_sample = (i2s_bits_per_sample_t)16,
     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
     .communication_format = I2S_COMM_FORMAT_STAND_I2S,
     .intr_alloc_flags = 0, // default interrupt priority
     .dma_buf_count = 16,
     .dma_buf_len = 60,
     .use_apll = false
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = 21,
        .ws_io_num = 18,
        .data_out_num = 40,
        .data_in_num = I2S_PIN_NO_CHANGE,                                               //Not used
        //.mck_io_num = -1,
    };
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);
    
}
void audio_task(void *arg)
{
    size_t bytes_read;
    
    esp_tts_voice_t *voice = (esp_tts_voice_t *)arg;
    QueueHandle_t xQueue = (QueueHandle_t) arg;
	// while(1){  
	// delay(100);
	// }
    while (1) {
      sTts_t result;
      xQueueReceive(xQueue, &result, portMAX_DELAY);
	  _busy = true;
      tts_handle=(esp_tts_handle_t *)esp_tts_create(voice);
	
      if (esp_tts_parse_chinese(tts_handle, result.prompt)) {
        int len[1]={0};
        do {
            short *pcm_data=esp_tts_stream_play(tts_handle, len, 0);
           i2s_write(I2S_NUM, (char *)pcm_data, len[0]*2, &bytes_read, portMAX_DELAY);
           //delay(1);
        } while(len[0]>0);

        i2s_zero_dma_buffer(I2S_NUM);
		
	  }
      _busy = false;
      esp_tts_stream_reset(tts_handle); 
      delay(1000);
        
    }

    vTaskDelete(NULL);


}

bool DFRobot_ESPTTS::begin(){
   g_result_que = xQueueCreate(1, sizeof(sTts_t));
   
	i2sInit1();
	
   //xTaskCreatePinnedToCore(audio_task, "Task", 4 * 1024, voice, 5, NULL, 1);
   return true;
}
bool DFRobot_ESPTTS::busy(){


  return _busy;

}

uint8_t DFRobot_ESPTTS::speak(const char *prompt){
	  
      //sTts_t result = {
       //        .prompt = prompt,
       //        .state = 1,
       //    };
    //  xQueueSend(g_result_que, &result, 10);
    size_t bytes_read;

      tts_handle=(esp_tts_handle_t *)esp_tts_create(voice);
	
      if (esp_tts_parse_chinese(tts_handle, prompt)) {
        int len[1]={0};
        do {
            short *pcm_data=esp_tts_stream_play(tts_handle, len, 2);
           i2s_write(I2S_NUM, (char *)pcm_data, len[0]*2, &bytes_read, portMAX_DELAY);
           //delay(8);
        } while(len[0]>0);

        i2s_zero_dma_buffer(I2S_NUM);
		
	  }
      _busy = false;
      esp_tts_stream_reset(tts_handle);
	   
	   
       return 0;
	   
}
