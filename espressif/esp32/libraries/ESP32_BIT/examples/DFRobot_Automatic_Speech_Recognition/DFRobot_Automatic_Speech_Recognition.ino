#include "Arduino.h"
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_process_sdkconfig.h"
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "esp_afe_sr_iface.h"
#include "esp_afe_sr_models.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "model_path.h"
#include "driver/i2s.h"
#include "esp_mn_speech_commands.h"
#define CONFIG_USE_WAKENET 1

static esp_afe_sr_iface_t *afe_handle = NULL;
srmodel_list_t *models = NULL;
static esp_afe_sr_data_t *afe_data = NULL;
static int play_voice = -2;
int detect_flag = 0;
uint8_t state = 0;


void i2s_init(void)
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // the mode must be set according to DSP configuration
        .sample_rate = 16000,                                // must be the same as DSP configuration
        .bits_per_sample = (i2s_bits_per_sample_t)16,        // must be the same as DSP configuration
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,         // must be the same as DSP configuration
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags       = 0, 
        .dma_buf_count          = 6, 
        .dma_buf_len            = 160, 
        .use_apll               = false, 
        .tx_desc_auto_clear     = true, 
        .fixed_mclk             = 0, 
        .mclk_multiple          = I2S_MCLK_MULTIPLE_DEFAULT, 
        .bits_per_chan          = I2S_BITS_PER_CHAN_16BIT
    };
    
    i2s_pin_config_t pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = 45,                  // IIS_SCLK
        .ws_io_num = 46,                   // IIS_LCLK
        .data_out_num = I2S_PIN_NO_CHANGE, // IIS_DSIN
        .data_in_num = 44                   // IIS_DOUT
    };
    i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &pin_config);
    i2s_zero_dma_buffer(I2S_NUM_1);
}
size_t bytes_read;
void feed_Task(void *arg)
{
    esp_afe_sr_data_t *afe_data = (esp_afe_sr_data_t *)arg;
    int audio_chunksize = afe_handle->get_feed_chunksize(afe_data);
    int nch = afe_handle->get_total_channel_num(afe_data);
    size_t samp_len = audio_chunksize * nch;
    size_t samp_len_bytes = samp_len * sizeof(int16_t);
    int16_t *i2s_buff = (int16_t *)malloc(samp_len_bytes);
    while (1) {
        i2s_read(I2S_NUM_1, i2s_buff, audio_chunksize * sizeof(int16_t) * nch, &bytes_read, portMAX_DELAY);
        if(state == 1)
          afe_handle->feed(afe_data, i2s_buff);
    }
    if (i2s_buff) {
        free(i2s_buff);
        i2s_buff = NULL;
    }
    vTaskDelete(NULL);
}

void detect_Task(void *arg)
{
    esp_afe_sr_data_t *afe_data = (esp_afe_sr_data_t *)arg;
    int afe_chunksize = afe_handle->get_fetch_chunksize(afe_data);
    int16_t *buff = (int16_t*)malloc(afe_chunksize * sizeof(int16_t));
    char *mn_name = esp_srmodel_filter(models, ESP_MN_PREFIX, ESP_MN_CHINESE);
    printf("multinet:%s\n", mn_name);
    esp_mn_iface_t *multinet = esp_mn_handle_from_name(mn_name);
    model_iface_data_t *model_data = multinet->create(mn_name, 5760);
    char *en_commands_en = "da kai kong qi";
     char *en_commands_en1 = "tiao gao wen du";
     char *en_commands_en2 = "da kai kong tiao";
     char *en_commands_en3 = "guan bi kong tiao";
     esp_mn_commands_alloc();
     esp_mn_commands_add(0,en_commands_en);
     esp_mn_commands_add(1,en_commands_en1);
     esp_mn_commands_add(2,en_commands_en2);
     esp_mn_commands_add(3,en_commands_en3);
     esp_mn_commands_update(multinet, model_data);
    printf("------------detect start------------\n");
    state = 1;
    while (1) {
       afe_fetch_result_t* res = afe_handle->fetch(afe_data); 
        if (!res || res->ret_value == ESP_FAIL) {
            printf("fetch error!\n");
            break;
        }
        if (res->wakeup_state == WAKENET_DETECTED) {
            printf("WAKEWORD DETECTED\n");
     //multinet->clean(model_data);  // clean all status of multinet
        } else if (res->wakeup_state == WAKENET_CHANNEL_VERIFIED) {
            play_voice = -1;
            detect_flag = 1;
            printf("AFE_FETCH_CHANNEL_VERIFIED, channel index: %d\n", res->trigger_channel_id);
        }

        if (detect_flag == 1) {
            esp_mn_state_t mn_state = multinet->detect(model_data, res->data);

            if (mn_state == ESP_MN_STATE_DETECTING) {
                continue;
            }

            if (mn_state == ESP_MN_STATE_DETECTED) {
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                for (int i = 0; i < mn_result->num; i++) {
                    printf("TOP %d, command_id: %d, phrase_id: %d, prob: %f\n", 
                    i+1, mn_result->command_id[i], mn_result->phrase_id[i], mn_result->prob[i]);
                }
                detect_flag = 0;
                afe_handle->enable_wakenet(afe_data);
                printf("\n-----------listening-----------\n");
            }

            if (mn_state == ESP_MN_STATE_TIMEOUT) {
                afe_handle->enable_wakenet(afe_data);
                detect_flag = 0;
                printf("\n-----------awaits to be waken up-----------\n");
            }
        }
    }
    if (buff) {
        free(buff);
        buff = NULL;
    }
    vTaskDelete(NULL);
}


void setup(void){
  //srmodel_init();
  i2s_init();
  models = srmodel_spiffs_init("model");

      char* wn_name = esp_srmodel_filter(models, ESP_WN_PREFIX, NULL);
      printf("%s\n",wn_name);
      afe_handle = (esp_afe_sr_iface_t *)&ESP_AFE_SR_HANDLE;
 afe_config_t afe = { 
    .aec_init = true, \
    .se_init = false, \
    .vad_init = true, \
    .wakenet_init = true, \
    .voice_communication_init = false, \
    .voice_communication_agc_init = false, \
    .voice_communication_agc_gain = 15, \
    .vad_mode = VAD_MODE_3, \
    .wakenet_model_name = NULL, \
    .wakenet_mode = DET_MODE_90, \
    .afe_mode = SR_MODE_HIGH_PERF, \
    .afe_perferred_core = 0, \
    .afe_perferred_priority = 5, \
    .afe_ringbuf_size = 64, \
    .memory_alloc_mode = AFE_MEMORY_ALLOC_INTERNAL_PSRAM_BALANCE, \
    .agc_mode = AFE_MN_PEAK_AGC_MODE_2, \
};
  afe.wakenet_model_name = wn_name;
      afe.pcm_config.total_ch_num = 2;
  afe.pcm_config.mic_num = 1;
    afe.pcm_config.ref_num = 1;
    afe.pcm_config.sample_rate = 16000;
  afe_data = afe_handle->create_from_config(&afe);
  
  xTaskCreatePinnedToCore(&detect_Task, "detect", 8 * 1024, (void*)afe_data, 5, NULL, 1);
  xTaskCreatePinnedToCore(&feed_Task, "feed", 8 * 1024, (void*)afe_data, 5, NULL, 0);
}


void loop(void){
  
  
}
