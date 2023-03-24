#ifndef DFROBOT_BIT_GENERAL_H_
#define DFROBOT_BIT_GENERAL_H_
#include "Arduino.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_camera.h"
#include "screen_driver.h"
#include "logo_en_240x240_lcd.h"
#include <string.h>
#include "dl_define.hpp"
#include "dl_image.hpp"
#include "human_face_detect_msr01.hpp"
#include "human_face_detect_mnp01.hpp"
#include "face_recognition_tool.hpp"
#include "fb_gfx.h"
#include "face_recognition_112_v1_s8.hpp"
#include "face_recognition_112_v1_s16.hpp"
#include "who_ai_utils.hpp"
#include "color_detector.hpp"

#define ENABLE_DBG ///< 打开这个宏, 可以看到程序的详细运行过程
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif
typedef enum{
  IDLE = 0,
  ENROLL,//注册人脸
  RECOGNIZE,//识别人脸
  DELETE,//删除人脸
  SHOW,//在屏幕指定位置输出字符串
}eCmd_t;

typedef struct{
  eCmd_t cmd;//命令
  char* name;//数据
  uint8_t id;//id
}sFaceReceiveData_t;

typedef struct{
  uint32_t color;
  uint8_t x;
  uint8_t y;
  char* str;
}sPrintfStr;

class DFRobot_ESP32S3_BIT
{
public:
  DFRobot_ESP32S3_BIT(void){};
  /**
   * @brief 初始化摄像头
   * 
   * @param frame_o 摄像头数据返回队列 
   */
  void initCamera(QueueHandle_t frame_o);
  /**
   * @brief 初始化运动检测
   * 
   * @param frame_i  数据输入队列
   * @param frame_o  数据输出队列
   * @param result   检测状态队列
   * @param sensitivity 阈值配置
   */
  void initMotionDetetion(QueueHandle_t frame_i,QueueHandle_t frame_o, QueueHandle_t result,uint8_t sensitivity);
  /**
   * @brief 停止运动检测
   */
  void closeMotionDetection(void);
  /**
   * @brief 初始化人脸识别
   * 
   * @param frame_i 数据输入队列
   * @param frame_o 数据输出队列
   * @param contorl 控制队列
   * @param result 数据返回队列
   * @param state true:一直处于人脸识别模式，false: 通过命令进行识别
   */
  void initFaceRecognition(QueueHandle_t frame_i,QueueHandle_t frame_o, QueueHandle_t contorl, QueueHandle_t result, bool state);
  /**
   * @brief 关闭人脸检测
   * 
   */
  void closeFaceRecognition();
  /**
   * @brief 初始化颜色识别
   * 
   * @param frame_i 数据输入队列 
   * @param frame_o 数据输出队列
  * @param contorl 控制队列
   * @param result 返回颜色识别
   */
  void initColorDetection(QueueHandle_t frame_i,QueueHandle_t frame_o, QueueHandle_t contorl, QueueHandle_t result);
  // /**
  //  * @brief 在指定位置显示字符串
  //  * 
  //  * @param frame_i 
  //  * @param frame_o 
  //  * @param x 
  //  * @param y 
  //  * @param str 
  //  */
  // void printfStr(QueueHandle_t frame_i,QueueHandle_t frame_o,QueueHandle_t printf);

private:

};
#endif
