#include "DFRobot_Gyroscope.h"

uint8_t DFRobot_Gyroscope::begin(void)
{
   _pWire->setPins(47,48);
  _pWire->begin();
  uint8_t buf[3];
  uint8_t reg = 0x01;
  uint8_t data = 0xA2;
  uint8_t sensorID = 0;
  uint8_t state = 0;
  if(readReg(reg, buf, 3) != 0)
    return 1;
  writeReg(0x0A,&data,1);
  delay(1800);
  while((sensorID!=0x05)&&(state < 5)){
    readReg(0x00,buf,2);
    sensorID = buf[0];
    DBG(sensorID);
    state++;
  }
  if(state>= 5)
    return 1;
  
  data = 0x60;
  writeReg(GYROSCOPE_CTRL1,&data,1);
  data = 0xB4;
  writeReg(GYROSCOPE_CTRL2,&data,1);
  data = 0xF4;
  writeReg(GYROSCOPE_CTRL3,&data,1);
  data = 0x11;
  writeReg(GYROSCOPE_CTRL5,&data,1);
  data = 0x03;
  writeReg(GYROSCOPE_CTRL7,&data,1);
  return 0;
}

int16_t DFRobot_Gyroscope::readAccelerationX(void)
{
  int16_t data = 0;
  uint8_t buffer[2];
  readReg(ACCELERATIONX,buffer,2);
  data = (buffer[0]<<8) | buffer[1];
  return data;
}

int16_t DFRobot_Gyroscope::readAccelerationY(void)
{
  int16_t data = 0;
  uint8_t buffer[2];
  readReg(ACCELERATIONY,buffer,2);
  data = (buffer[0]<<8) | buffer[1];
  return data;
}

int16_t DFRobot_Gyroscope::readAccelerationZ(void)
{
  int16_t data = 0;
  uint8_t buffer[2];
  readReg(ACCELERATIONZ,buffer,2);
  data = (buffer[0]<<8) | buffer[1];
  return data;
}

int16_t DFRobot_Gyroscope::readAngularRateX(void)
{
  int16_t data = 0;
  uint8_t buffer[2];
  readReg(ANGULARRATEX,buffer,2);
  data = (buffer[0]<<8) | buffer[1];
  return data;
}

int16_t DFRobot_Gyroscope::readAngularRateY(void)
{
  int16_t data = 0;
  uint8_t buffer[2];
  readReg(ANGULARRATEY,buffer,2);
  data = (buffer[0]<<8) | buffer[1];
  return data;
}

int16_t DFRobot_Gyroscope::readAngularRateZ(void)
{
  int16_t data = 0;
  uint8_t buffer[2];
  readReg(ANGULARRATEZ,buffer,2);
  data = (buffer[0]<<8) | buffer[1];
  return data;
}

uint8_t DFRobot_Gyroscope::readReg(uint8_t reg, void *pBuf, uint8_t size)
{
  uint8_t* _pBuf = (uint8_t*)pBuf;
  if(pBuf == NULL){
    return 0;
  }
  _pWire->beginTransmission(_addr);
  _pWire->write(reg);
  if(_pWire->endTransmission() != 0)
      return 1;
  delay(10);
  _pWire->requestFrom(_addr, size);
  for(uint8_t i = 0; i < size; i++)
    _pBuf[i] = _pWire->read();
  return 0;
}

uint8_t DFRobot_Gyroscope::writeReg(uint8_t reg, void *pBuf, size_t size)
{
  uint8_t *_pBuf = (uint8_t*)pBuf;
  uint8_t ret = 0;
  _pWire->beginTransmission(_addr);
  _pWire->write(reg);
  for(size_t i = 0; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
  return ret;
}
