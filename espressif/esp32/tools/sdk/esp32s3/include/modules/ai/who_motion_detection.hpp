#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
typedef struct{
    bool state;
    uint8_t sensitivity;
}motion_state;
/**
 * @brief 实现移动检测
 * 
 * @param frame_i 输入摄像头数据队列
 * @param result 返回检测结果队列
 * @param frame_o 输出摄像头数据队列
 * @param sensitivity 阈值设置
 */
void register_motion_detection(QueueHandle_t frame_i , QueueHandle_t result , QueueHandle_t frame_o , uint8_t sensitivity = 50);
