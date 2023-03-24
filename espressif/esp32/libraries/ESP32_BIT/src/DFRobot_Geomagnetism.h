#ifndef _DFROBOT_CPMPASS_H_
#define _DFROBOT_CPMPASS_H_
#include "Arduino.h"
#include "Wire.h"

#define GE_ENABLE_DBG ///< 打开这个宏, 可以看到程序的详细运行过程
#ifdef GE_ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif
class DFRobot_Geomagnetism
{
public:
  /**
   * @fn DFRobot_Cpmpass
   * @brief Construct a new dfrobot cpmpass object
   * @param pWire 
   */
  DFRobot_Geomagnetism(TwoWire *pWire = &Wire, uint8_t address = 0x1C){
  	_pWire = pWire;
    _addr = address;
  }
  /**
   * @fn begin
   * @brief 初始化地磁传感器
   * @return 返回初始换状态
   */
  uint8_t begin(void);
  /**
   * @fn readX
   * @brief 获取X方向的地磁
   * @return 返回X方向地磁
   */
  int16_t readX();
  /**
   * @fn readY
   * @brief 获取Y方向地磁
   * @return 返回Y方向地磁
   */
  int16_t readY();
  /**
   * @fn readZ
   * @brief 获取X方向地磁
   * @return 返回Z方向地磁
   */
  int16_t readZ();

private:
  uint8_t  readReg(uint8_t reg, void *pBuf, uint8_t size);
  uint8_t  writeReg(uint8_t reg, void *pBuf, size_t size);
  TwoWire *_pWire;
  uint8_t _addr;
};


#endif