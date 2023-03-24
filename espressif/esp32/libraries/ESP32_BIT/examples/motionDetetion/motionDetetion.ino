#include "who_motion_detection.hpp"
#include "who_camera.h"
#include "who_lcd.h"
#include "initBoard.h"
static QueueHandle_t xQueueAIFrame = NULL;
static QueueHandle_t xQueueLCD = NULL;
static QueueHandle_t xQueueResult = NULL;
uint8_t a;
uint8_t i = 0;
void setup(void){
    xQueueAIFrame = xQueueCreate(2, sizeof(camera_fb_t *));
    xQueueLCD = xQueueCreate(2, sizeof(camera_fb_t *));
    xQueueResult = xQueueCreate(1, sizeof(bool));
    init_board();
    register_camera(PIXFORMAT_RGB565, FRAMESIZE_240X240, 2, xQueueAIFrame);
    register_motion_detection(xQueueAIFrame,xQueueResult,xQueueLCD,50);
    register_lcd(xQueueLCD,NULL,true);
}

void loop(void){
  xQueueReceive(xQueueResult, &(a), portMAX_DELAY);
    if(a){
       printf("qSomething moved!\n");
       i++;
       if(i == 100){
        i = 0;
        }
      }
}