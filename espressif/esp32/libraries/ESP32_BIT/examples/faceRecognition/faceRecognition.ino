//#include "DFRobot_ESP32S3_BIT.h"
#include "who_human_face_recognition.hpp"
#include "who_camera.h"
#include "who_lcd.h"
#include "who_ai_utils.hpp"
#include "initBoard.h"
//DFRobot_ESP32S3_BIT espbit;
sFaceData_t* face;
static QueueHandle_t xQueueAIFrame = NULL;
static QueueHandle_t xQueueLCD = NULL;
static QueueHandle_t xQueueResult = NULL;
static QueueHandle_t xQueueEvent = NULL;
uint8_t recognize = RECOGNIZE;
uint8_t enroll = ENROLL;

void KEYB(void)//识别按键
{
  xQueueSend(xQueueEvent, &recognize, portMAX_DELAY);
    printf("KEY B\n");
    
}

void KEYA(void)//添加人脸
{
  xQueueSend(xQueueEvent, &enroll, portMAX_DELAY);
    printf("KEY A\n");
}

void setup(void){
    xQueueAIFrame = xQueueCreate(2, sizeof(camera_fb_t *));
    xQueueLCD = xQueueCreate(2, sizeof(camera_fb_t *));
    xQueueResult = xQueueCreate(1, sizeof(sFaceData_t *));
    xQueueEvent = xQueueCreate(1, sizeof(uint8_t));
    init_board();
    digitalInterrupt(eP11,KEYB,INTR_NEGEDGE);
    digitalInterrupt(eP5,KEYA,INTR_POSEDGE);
    register_camera(PIXFORMAT_RGB565, FRAMESIZE_240X240, 2, xQueueAIFrame);
    register_human_face_recognition(xQueueAIFrame,xQueueEvent,xQueueResult,xQueueLCD,false);
    register_lcd(xQueueLCD,NULL,true);   
}

void loop(void){
  if(xQueueReceive(xQueueResult, &face, portMAX_DELAY)){
      printf("index:%d\n",face->index);
      printf("face frame width :%d\n",face->faceFrameWidth);
      printf("face frame length :%d\n",face->faceFrameLength);
      printf(" center Of Inertia: (%d, %d)\n",face->centerOfInertia[0],face->centerOfInertia[1]);
      printf(" left eye: (%d, %d)\n",face->leftEye[0],face->leftEye[1]);
      printf(" nose: (%d, %d)\n",face->nose[0],face->nose[1]);
      printf("id:%d\n",face->infoData.id);
    }
  delay(100);
}