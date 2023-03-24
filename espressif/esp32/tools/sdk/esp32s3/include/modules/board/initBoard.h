
#include "driver/i2c.h"
#include <stdio.h>
#include "sdkconfig.h"
#include "i2c_bus.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define INTR_POSEDGE           0 //上升沿
#define INTR_NEGEDGE          1 //下降沿
#define INTR_ANYEDGE          2//双边沿

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 引脚选择
 */
typedef enum{
    eP13 = 0,
    eP11,
    eP14,
    eP10,
    eP15,
    eP9,
    eP12,
    eP16,
    eP8,
    eP3,
    eP5
}ePin_t;


typedef void (*interruptFunc)(void);//中断处理函数

typedef struct sInterrupt_t{
    uint8_t pin;         // 节点存储的数据
    uint8_t modelInter;      //中断模式
    interruptFunc func;    // 存储函数指针
    struct sInterrupt_t* next;   // 指向下一个节点的指针
} sInterrupt_t;


/**
 * @brief 初始化主板电源芯片
 * 
 * @return 返回初始化状态 
 */
esp_err_t init_board(void);
/**
 * @brief 设置引脚输出电平
 * 
 * @param pin 引脚选择
 * @param state 输出电平
 */
void digital_write(ePin_t pin,uint8_t state);
/**
 * @brief 获取引脚电平
 * 
 * @param pin 引脚选择
 * @return 返回引脚电平
 */
uint8_t digital_read(ePin_t pin);
/**
 * @brief 配置引脚中断
 * 
 * @param pin 引脚号
 * @param function 中断处理函数
 */
void digitalInterrupt(ePin_t pin,interruptFunc function, uint8_t state);
#ifdef __cplusplus
}
#endif

