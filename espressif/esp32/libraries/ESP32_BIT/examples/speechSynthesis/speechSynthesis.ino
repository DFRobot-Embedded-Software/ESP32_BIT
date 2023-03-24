

#include "DFRobot_ESPTTS.h"
#include "initBoard.h"
DFRobot_ESPTTS tts;
const char *prompt1="叫我干啥";  
const char *prompt2="温江，多云，空气质量，优";  
const char *prompt3="9点9分";  
const char *prompt4="28度";  
const char *prompt5="我没听清楚你在说什么，请在说一遍";  
const char *prompt6="上海";  
const char *prompt7="今日天气，多云";  
const char *prompt8="空气质量，优";  
const char *prompt9="温度，26摄氏度，湿度，百分之48";  
const char *prompt10="湿度，百分之48";  
void setup() {
 init_board();
  tts.begin();
  
}

void loop() {
  delay(3000);
  tts.speak("温度，26摄氏度，湿度，百分之48");
  
  delay(3000);
  tts.speak("今日天气，多云");
  delay(3000);  
  tts.speak("温江，多云，空气质量，优" );

}