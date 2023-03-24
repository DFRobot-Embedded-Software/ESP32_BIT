#include "Arduino.h"
#include "who_camera.h"
#include "who_human_face_detection.hpp"
#include "who_human_face_recognition.hpp"
#include "who_lcd.h"
static QueueHandle_t xQueueAIFrame = NULL;
static QueueHandle_t xQueueLCDFrame = NULL;

void setup(void){
  xQueueAIFrame = xQueueCreate(2, sizeof(camera_fb_t *));
    xQueueLCDFrame = xQueueCreate(2, sizeof(camera_fb_t *));

    register_camera(PIXFORMAT_RGB565, FRAMESIZE_240X240, 2, xQueueAIFrame);
    //register_human_face_detection(xQueueAIFrame, NULL, NULL, xQueueLCDFrame, false);
    register_human_face_recognition(xQueueAIFrame, NULL, NULL, xQueueLCDFrame, false);
    register_lcd(xQueueLCDFrame, NULL, true);

}

void loop(void){

}
