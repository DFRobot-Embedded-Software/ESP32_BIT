#include "DFRobot_Geomagnetism.h"

DFRobot_Geomagnetism geomagnetism;
void setup()
{
  Serial.begin(115200);
  while(geomagnetism.begin() != 0){
    Serial.println("Sensor begin error");
    delay(1000);
  }
  Serial.println("Sensor begin success");
}

void loop()
{
  Serial.print("geomagnetism X:");
  Serial.println(geomagnetism.readX());
  Serial.print("geomagnetism Y:");
  Serial.println(geomagnetism.readY());
  Serial.print("geomagnetism Z:");
  Serial.println(geomagnetism.readZ());
  delay(1000);
}