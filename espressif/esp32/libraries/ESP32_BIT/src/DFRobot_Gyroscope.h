#ifndef _DFROBOT_GYROSCOPE_H_
#define _DFROBOT_GYROSCOPE_H_
#include "Arduino.h"
#include "Wire.h"

#define GYROSCOPE_CTRL1    0x02
#define GYROSCOPE_CTRL2    0x03
#define GYROSCOPE_CTRL3    0x04
#define GYROSCOPE_CTRL4    0x05
#define GYROSCOPE_CTRL5    0x06
#define GYROSCOPE_CTRL6    0x07
#define GYROSCOPE_CTRL7    0x08
#define ACCELERATIONX      0x35
#define ACCELERATIONY      0x37
#define ACCELERATIONZ      0x39
#define ANGULARRATEX       0x3B
#define ANGULARRATEY       0x3D
#define ANGULARRATEZ       0x3F

#define GY_ENABLE_DBG ///< 打开这个宏, 可以看到程序的详细运行过程
#ifdef GY_ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif
class DFRobot_Gyroscope
{
public:
  /**
   * @fn DFRobot_Gyroscope
   * @brief Construct a new dfrobot gyroscope object
   * @param pWire 
   * @param address 
   */
  DFRobot_Gyroscope(TwoWire *pWire = &Wire, uint8_t address = 0x6A){
    _pWire = pWire;
    _addr = address;
  }
  /**
   * @fn begin
   * @brief 初始化传感器
   * @return 返回初始化状态
   */
  uint8_t begin(void);
  /**
   * @fn readAccelerationX
   * @brief 获取X轴的加速度
   * @return 返回X轴加速度
   */
  int16_t readAccelerationX(void);
  /**
   * @fn readAccelerationY
   * @brief 获取Y轴的加速度
   * @return 返回Y轴的加速度
   */
  int16_t readAccelerationY(void);
  /**
   * @fn readAccelerationZ
   * @brief 获取Z轴的加速度
   * @return 返回Z轴的加速度
   */
  int16_t readAccelerationZ(void);
  /**
   * @fn readAngularRateX
   * @brief 获取X轴的角速度
   * @return 返回X轴的角速度
   */
  int16_t readAngularRateX(void);
  /**
   * @fn readAngularRateY
   * @brief 获取Y轴的角速度
   * @return 返回Y轴的角速度
   */
  int16_t readAngularRateY(void);
  /**
   * @fn readAngularRateZ
   * @brief 获取Z轴的角速度
   * @return 返回Z轴的角速度
   */
  int16_t readAngularRateZ(void);
private:
  uint8_t  readReg(uint8_t reg, void *pBuf, uint8_t size);
  uint8_t  writeReg(uint8_t reg, void *pBuf, size_t size);
  TwoWire *_pWire;
  uint8_t _addr;

};

#endif