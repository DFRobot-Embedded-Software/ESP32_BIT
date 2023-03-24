#include "DFRobot_Geomagnetism.h"
uint8_t DFRobot_Geomagnetism::begin(void)
{
  _pWire->setPins(47,48);
  _pWire->begin();
  
  uint8_t buf[3];
  uint8_t reg = 0x01;
  uint8_t data = 0x06;
  if(readReg(reg, buf, 3) != 0)
    return 1;
  delay(100);
  writeReg(0X29,&data,1);
  data = 0x08;
  writeReg(0X0B,&data,1);
  data = 0xC3;
  writeReg(0X0A,&data,1);
  return 0;
 }

int16_t DFRobot_Geomagnetism::readX(void)
{  
  uint8_t buffer[2];
  int16_t data;
  uint8_t i = 0;
  while(!(buffer[0]&0x01)&& (i<5)){
    readReg(0x09, buffer, 1);
    i++;
  }
  readReg(0x01, buffer, 2);
  data = (buffer[1] << 8) + buffer[0];
  return data;
 }

int16_t DFRobot_Geomagnetism::readY(void)
{  
  uint8_t buffer[2];
  int16_t data;
  uint8_t i = 0;
  while(!(buffer[0]&0x01)&& (i<5)){
    readReg(0x09, buffer, 1);
    i++;
  }
  readReg(0x03, buffer, 2);
  data = (buffer[1] << 8) + buffer[0];
  return data;
 }

int16_t DFRobot_Geomagnetism::readZ(void)
{  
  uint8_t buffer[2];
  int16_t data;
  uint8_t i = 0;
  while(!(buffer[0]&0x01)&& (i<5))
  {
    readReg(0x09, buffer, 1);
    i++;
  }
  readReg(0x05, buffer, 2);
  data = (buffer[1] << 8) + buffer[0];
  return data;
}

uint8_t DFRobot_Geomagnetism::readReg(uint8_t reg, void *pBuf, uint8_t size)
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

uint8_t DFRobot_Geomagnetism::writeReg(uint8_t reg, void *pBuf, size_t size)
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
