#include "DFRobot_Gyroscope.h"

DFRobot_Gyroscope gyroscope;
void setup()
{
  Serial.begin(115200);
  while(gyroscope.begin() != 0){
    Serial.println("Sensor init error");
    delay(1000);
  }
  Serial.println("Sensor init success");
}

void loop()
{
  Serial.print("AccelerationX:");
  Serial.println(gyroscope.readAccelerationX());
  Serial.print("AccelerationY:");
  Serial.println(gyroscope.readAccelerationY());
  Serial.print("AccelerationY:");
  Serial.println(gyroscope.readAccelerationZ());
  Serial.print("AngularRateX:");
  Serial.println(gyroscope.readAngularRateX());
  Serial.print("AngularRateY:");
  Serial.println(gyroscope.readAngularRateY());
  Serial.print("AngularRateZ:");
  Serial.println(gyroscope.readAngularRateZ());
  delay(500);

}