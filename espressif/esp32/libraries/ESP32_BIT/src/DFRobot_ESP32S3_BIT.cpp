
// #include "DFRobot_ESP32S3_BIT.h"
// static QueueHandle_t xQueueCameraFrameO = NULL;
// static QueueHandle_t xQueueCameraFrameMotioni = NULL;
// static QueueHandle_t xQueueCameraFrameMotionO = NULL;
// static QueueHandle_t xQueueResultMotion = NULL;
// static QueueHandle_t xQueueCameraFrameFacei = NULL;
// static QueueHandle_t xQueueCameraFrameFaceO = NULL;
// static QueueHandle_t xQueueResultFace = NULL;
// static QueueHandle_t xQueueReceiveFace = NULL;
// static QueueHandle_t xQueueCameraFrameColori = NULL;
// static QueueHandle_t xQueueCameraFrameColorO = NULL;
// static QueueHandle_t xQueueResultColor = NULL;
// // static QueueHandle_t xQueueStrI = NULL;
// // static QueueHandle_t xQueueStrO = NULL;


// static bool _motionSwitch = false;
// static bool _faceSwitchs = false;
// static uint8_t _sensitivity = 50;
// uint16_t statiA = 0;

// char* TAG = "ESP32_BIT";
// SemaphoreHandle_t xMutex;

// sFaceReceiveData_t faceReceive;
// sFaceReceiveData_t* fa = &faceReceive;

// static face_info_t recognize_result;

// static color_detection_state_t gEvent = COLOR_DETECTION_IDLE;
// static bool register_mode = false;
// static bool gReturnFB = true;
// static bool draw_box = true;

// static SemaphoreHandle_t xMutex;
// vector<color_info_t> std_color_info = {{{156, 10, 70, 255, 90, 255}, 64, "red"},
//                                        {{11, 22, 70, 255, 90, 255}, 64, "orange"},
//                                        {{23, 33, 70, 255, 90, 255}, 64, "yellow"},
//                                        {{34, 75, 70, 255, 90, 255}, 64, "green"},
//                                        {{76, 96, 70, 255, 90, 255}, 64, "cyan"},
//                                        {{97, 124, 70, 255, 90, 255}, 64, "blue"},
//                                        {{125, 155, 70, 255, 90, 255}, 64, "purple"},
//                                        {{0, 180, 0, 40, 220, 255}, 64, "white"},
//                                        {{0, 180, 0, 50, 50, 219}, 64, "gray"},
//                                     //    {{0, 180, 0, 255, 0, 45}, 64, "black"}
//                                        };

// #define RGB565_LCD_RED 0x00F8
// #define RGB565_LCD_ORANGE 0x20FD
// #define RGB565_LCD_YELLOW 0xE0FF
// #define RGB565_LCD_GREEN 0xE007
// #define RGB565_LCD_CYAN 0xFF07
// #define RGB565_LCD_BLUE 0x1F00
// #define RGB565_LCD_PURPLE 0x1EA1
// #define RGB565_LCD_WHITE 0xFFFF
// #define RGB565_LCD_GRAY 0x1084
// #define RGB565_LCD_BLACK 0x0000


// typedef enum
// {
//     COLOR_DETECTION_IDLE = 0,
//     OPEN_REGISTER_COLOR_BOX,
//     CLOSE_REGISTER_COLOR_BOX,
//     REGISTER_COLOR,
//     DELETE_COLOR,
//     INCREASE_COLOR_AREA,
//     DECREASE_COLOR_AREA,
//     SWITCH_RESULT,
// } color_detection_state_t;


// void task_process_handler_camera(void *arg)
// {
//   while (true){
//     camera_fb_t *frame = esp_camera_fb_get();
//     if (frame)
//       xQueueSend(xQueueCameraFrameO, &frame, portMAX_DELAY);
//     vTaskDelay(1);
//   }
// }


// void DFRobot_ESP32S3_BIT::initCamera(const QueueHandle_t frame_o){
//   printf("Start init caram!!\n");
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = 9;
//   config.pin_d1 = 11;
//   config.pin_d2 = 12;
//   config.pin_d3 = 10;
//   config.pin_d4 = 8;
//   config.pin_d5 = 6;
//   config.pin_d6 = 5;
//   config.pin_d7 = 3;
//   config.pin_xclk = 4;
//   config.pin_pclk = 7;
//   config.pin_vsync = 1;
//   config.pin_href = 2;
//   config.pin_sscb_sda = 47;
//   config.pin_sscb_scl = 48;
//   config.pin_pwdn = -1;
//   config.pin_reset = -1;
//   config.xclk_freq_hz = 15000000;
//   config.pixel_format = PIXFORMAT_RGB565;
//   config.frame_size = FRAMESIZE_240X240;
//   config.jpeg_quality = 12;
//   config.fb_count = 2;
//   config.fb_location = CAMERA_FB_IN_PSRAM;
//   config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
//   // camera init
//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK){
//     printf("Camera init failed with error 0x%x\n", err);
//     return;
//   }

//   sensor_t *s = esp_camera_sensor_get();
//   s->set_vflip(s, 1); //flip it back
//   printf("Camera module is %d", s->id.PID);
//   //initial sensors are flipped vertically and colors are a bit saturated
//   if (s->id.PID == OV3660_PID){
//     s->set_brightness(s, 1);  //up the blightness just a bit
//     s->set_saturation(s, 2); //lower the saturation
//     s->set_brightness(s,2);
//   }

//   xQueueCameraFrameO = frame_o;
//   xTaskCreatePinnedToCore(task_process_handler_camera, "camera_l", 5 * 1024, NULL, 5, NULL, 1);
// }


// static void task_process_handler_motion(void *arg)
// {
//   ESP_LOGD(TAG, "Start");
  
//   while (true){   
//     camera_fb_t *frame1 = NULL;
//     camera_fb_t *frame2 = NULL;
//     uint8_t is_moved = 0;
//     if(xQueueCameraFrameMotioni == nullptr)
//       break;
//     if (xQueueReceive(xQueueCameraFrameMotioni, &(frame1), portMAX_DELAY)){
//       if (_motionSwitch){
//         if (xQueueReceive(xQueueCameraFrameMotioni, &(frame2), portMAX_DELAY)){
//           uint32_t moving_point_number = dl::image::get_moving_point_number((uint16_t *)frame1->buf, (uint16_t *)frame2->buf, frame1->height, frame1->width, 16, 15);
//           if (moving_point_number > _sensitivity){
//             //ESP_LOGI(TAG, "Something moved!");
//             //dl::image::draw_filled_rectangle((uint16_t *)frame1->buf, frame1->height, frame1->width, 0, 0, 20, 20);
//             is_moved = 1;
//           }
//         esp_camera_fb_return(frame2);
//         }
//       }

//       if (xQueueCameraFrameMotionO)
//         xQueueSend(xQueueCameraFrameMotionO, &frame1, portMAX_DELAY);
//       else
//         esp_camera_fb_return(frame1);

//       if (xQueueResultMotion)
//         xQueueSend(xQueueResultMotion, &is_moved, portMAX_DELAY);
//     }

    
//     vTaskDelay(1);
//     }
//     ESP_LOGD(TAG, "Stop");
//     vTaskDelete(NULL);
// }

// void DFRobot_ESP32S3_BIT::initMotionDetetion(QueueHandle_t frame_i,QueueHandle_t frame_o, QueueHandle_t result,uint8_t sensitivity)
// {
//     xQueueCameraFrameMotioni = frame_i;
//     xQueueCameraFrameMotionO = frame_o;
//     xQueueResultMotion = result;
//     _sensitivity = sensitivity;
//     _motionSwitch = true;
//     xTaskCreatePinnedToCore(task_process_handler_motion, "motion", 3 * 1024, NULL, 5, NULL, 0);
// }

// void DFRobot_ESP32S3_BIT::closeMotionDetection(void){
//      _motionSwitch = false;
// }

// static void task_event_handler_face(void *arg)
// {
//     sFaceReceiveData_t _faceReceive;
//     sFaceReceiveData_t* _fa = &_faceReceive;
//     while (true)
//     {
//       xQueueReceive(xQueueReceiveFace, &(_fa), portMAX_DELAY);
//       xSemaphoreTake(xMutex, portMAX_DELAY);
//       fa = _fa;
//       xSemaphoreGive(xMutex);
//       vTaskDelay(1);
//     }
// }

// static void task_process_handler_face(void *arg)
// {
  
//   HumanFaceDetectMSR01 detector(0.3F, 0.3F, 10, 0.3F);
//   HumanFaceDetectMNP01 detector2(0.4F, 0.3F, 10);

//   FaceRecognition112V1S8 *recognizer = new FaceRecognition112V1S8();
//   recognizer->set_partition(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "fr");
//   recognizer->set_ids_from_flash();
//   //sFaceReceiveData_t _faceReceive;
//   //sFaceReceiveData_t* _fa = &_faceReceive;
//   while(true){
//     camera_fb_t *frame = NULL;
//     xSemaphoreTake(xMutex, portMAX_DELAY);
//     //_fa = fa;
//     xSemaphoreGive(xMutex);
//     if (xQueueReceive(xQueueCameraFrameFacei,  &(frame), portMAX_DELAY)){
//       if(_faceSwitchs){
//         std::list<dl::detect::result_t> &detect_candidates = detector.infer((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3});
//         std::list<dl::detect::result_t> &detect_results = detector2.infer((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3}, detect_candidates);
//         if (detect_results.size()){//显示人脸框
//           draw_detection_result((uint16_t *)frame->buf, frame->height, frame->width, detect_results);
//         }
          
//         if (detect_results.size() == 1){
//           Serial.println(millis());
//           recognize_result = recognizer->recognize((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3}, detect_results.front().keypoint);
//           Serial.println(millis());
//           //print_detection_result(detect_results);
//           if (recognize_result.id > 0)
//             ESP_LOGI("RECOGNIZE", "Similarity: %f, Match ID: %d", recognize_result.similarity, recognize_result.id);
//           else
//             ESP_LOGE("RECOGNIZE", "Similarity: %f, Match ID: %d", recognize_result.similarity, recognize_result.id);
//           // switch(_fa->cmd){
//           //   case ENROLL:
//           //     recognizer->enroll_id((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3}, detect_results.front().keypoint, "", true);
//           //     ESP_LOGW("ENROLL", "ID %d is enrolled", recognizer->get_enrolled_ids().back().id);
//           //     break;

//           //   case RECOGNIZE:
//           //     recognize_result = recognizer->recognize((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3}, detect_results.front().keypoint);
//           //     print_detection_result(detect_results);
//           //     if (recognize_result.id > 0)
//           //       ESP_LOGI("RECOGNIZE", "Similarity: %f, Match ID: %d", recognize_result.similarity, recognize_result.id);
//           //     else
//           //       ESP_LOGE("RECOGNIZE", "Similarity: %f, Match ID: %d", recognize_result.similarity, recognize_result.id);
//           //     break;

//           //   case DELETE:
//           //     vTaskDelay(10);
//           //     recognizer->delete_id(true);
//           //     ESP_LOGE("DELETE", "% d IDs left", recognizer->get_enrolled_id_num());
//           //     break;

//           //   default:
//           //     break;
//           // }
//         }
//       }
     
//       if (xQueueCameraFrameFaceO){
//         xQueueSend(xQueueCameraFrameFaceO, &frame, portMAX_DELAY);
//       }else{
//         esp_camera_fb_return(frame);
//       }
//     }
//     vTaskDelay(1);
//   }
//   ESP_LOGD(TAG, "Stop");
//   vTaskDelete(NULL);
// }

// void DFRobot_ESP32S3_BIT::initFaceRecognition(QueueHandle_t frame_i,QueueHandle_t frame_o, QueueHandle_t receive, QueueHandle_t result, bool state){
//     xQueueCameraFrameFacei = frame_i;
//     xQueueCameraFrameFaceO = frame_o;
//     xQueueResultFace = result;
//     xQueueReceiveFace = receive;
//     xMutex = xSemaphoreCreateMutex();
//     // if(receive == NULL){
//     //   fa->cmd = IDLE;
//     // }
//     _faceSwitchs = true;

//     xTaskCreatePinnedToCore(task_process_handler_face, "face", 5 * 1024, NULL, 5, NULL, 0);
//     if (xQueueReceiveFace)
//       xTaskCreatePinnedToCore(task_event_handler_face, "face_event", 2 * 1024, NULL, 5, NULL, 1);
// }

// void DFRobot_ESP32S3_BIT::closeFaceRecognition(){

// }

// static void task_process_handler_color(void *arg){
//   camera_fb_t *frame = NULL;

//     ColorDetector detector;
//     detector.set_detection_shape({80, 80, 1});
//     for (int i = 0; i < std_color_info.size(); ++i)
//     {
//         detector.register_color(std_color_info[i].color_thresh, std_color_info[i].area_thresh, std_color_info[i].name);
//     }

//     vector<vector<int>> color_thresh_boxes = {{110, 110, 130, 130}, {100, 100, 140, 140}, {90, 90, 150, 150}, {80, 80, 160, 160}, {60, 60, 180, 180}, {40, 40, 200, 200}, {20, 20, 220, 220}};
//     int color_thresh_boxes_num = color_thresh_boxes.size();
//     int color_thresh_boxes_index = color_thresh_boxes_num / 2;
//     vector<int> color_area_threshes = {1, 4, 16, 32, 64, 128, 256, 512, 1024};
//     int color_area_thresh_num = color_area_threshes.size();
//     int color_area_thresh_index = color_area_thresh_num / 2;
    
//     detector.set_area_thresh({color_area_threshes[color_area_thresh_index]});


//     vector<uint16_t> draw_lcd_colors = {RGB565_LCD_RED, 
//                                         RGB565_LCD_ORANGE, 
//                                         RGB565_LCD_YELLOW,
//                                         RGB565_LCD_GREEN, 
//                                         RGB565_LCD_CYAN, 
//                                         RGB565_LCD_BLUE,
//                                         RGB565_LCD_PURPLE, 
//                                         RGB565_LCD_WHITE, 
//                                         RGB565_LCD_GRAY, 
//                                         // RGB565_LCD_BLACK
//                                         };
//     int draw_colors_num = draw_lcd_colors.size();

//     color_detection_state_t _gEvent;
//     vector<uint8_t> color_thresh;
//   while (true)
//     {
//         xSemaphoreTake(xMutex, portMAX_DELAY);
//         _gEvent = gEvent;
//         gEvent = COLOR_DETECTION_IDLE;
//         xSemaphoreGive(xMutex);

//         if (xQueueReceive(xQueueCameraFrameColori, &frame, portMAX_DELAY))
//         {
//             if (register_mode)
//             {
//                 switch (_gEvent)
//                 {
//                 case INCREASE_COLOR_AREA:
//                     color_thresh_boxes_index = min(color_thresh_boxes_num - 1, color_thresh_boxes_index + 1);
//                     ets_printf("increase color area\n");
//                     dl::image::draw_hollow_rectangle((uint16_t *)frame->buf, (int)frame->height, (int)frame->width,
//                                                      color_thresh_boxes[color_thresh_boxes_index][0],
//                                                      color_thresh_boxes[color_thresh_boxes_index][1],
//                                                      color_thresh_boxes[color_thresh_boxes_index][2],
//                                                      color_thresh_boxes[color_thresh_boxes_index][3],
//                                                      draw_lcd_colors[1]);
//                     break;

//                 case DECREASE_COLOR_AREA:
//                     color_thresh_boxes_index = max(0, color_thresh_boxes_index - 1);
//                     ets_printf("decrease color area\n");
//                     dl::image::draw_hollow_rectangle((uint16_t *)frame->buf, (int)frame->height, (int)frame->width,
//                                                      color_thresh_boxes[color_thresh_boxes_index][0],
//                                                      color_thresh_boxes[color_thresh_boxes_index][1],
//                                                      color_thresh_boxes[color_thresh_boxes_index][2],
//                                                      color_thresh_boxes[color_thresh_boxes_index][3],
//                                                      draw_lcd_colors[1]);
//                     break;

//                 case REGISTER_COLOR:
//                     color_thresh = detector.cal_color_thresh((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3}, color_thresh_boxes[color_thresh_boxes_index]);
//                     detector.register_color(color_thresh);
//                     ets_printf("register color, color_thresh: %d, %d, %d, %d, %d, %d\n", color_thresh[0], color_thresh[1], color_thresh[2], color_thresh[3], color_thresh[4], color_thresh[5]);
//                     xSemaphoreTake(xMutex, portMAX_DELAY);
//                     register_mode = false;
//                     xSemaphoreGive(xMutex);
//                     break;

//                 case CLOSE_REGISTER_COLOR_BOX:
//                     ets_printf("close register color box \n");
//                     xSemaphoreTake(xMutex, portMAX_DELAY);
//                     register_mode = false;
//                     xSemaphoreGive(xMutex);
//                     break;

//                 default:
//                     dl::image::draw_hollow_rectangle((uint16_t *)frame->buf, (int)frame->height, (int)frame->width,
//                                                      color_thresh_boxes[color_thresh_boxes_index][0],
//                                                      color_thresh_boxes[color_thresh_boxes_index][1],
//                                                      color_thresh_boxes[color_thresh_boxes_index][2],
//                                                      color_thresh_boxes[color_thresh_boxes_index][3],
//                                                      draw_lcd_colors[1]);
//                     break;
//                 }
//             }
//             else
//             {
//                 switch (_gEvent)
//                 {
//                 case INCREASE_COLOR_AREA:
//                     color_area_thresh_index = min(color_area_thresh_num - 1, color_area_thresh_index + 1);
//                     detector.set_area_thresh({color_area_threshes[color_area_thresh_index]});
//                     ets_printf("increase color area thresh to %d\n", color_area_threshes[color_area_thresh_index]);
//                     break;

//                 case DECREASE_COLOR_AREA:
//                     color_area_thresh_index = max(0, color_area_thresh_index - 1);
//                     detector.set_area_thresh({color_area_threshes[color_area_thresh_index]});
//                     ets_printf("decrease color area thresh to %d\n", color_area_threshes[color_area_thresh_index]);
//                     break;

//                 case DELETE_COLOR:
//                     detector.delete_color();
//                     ets_printf("delete color \n");
//                     break;

//                 default:
//                     std::vector<std::vector<color_detect_result_t>> &results = detector.detect((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3});
//                     if (draw_box)
//                     {
//                         for (int i = 0; i < results.size(); ++i)
//                         {
//                             draw_color_detection_result((uint16_t *)frame->buf, (int)frame->height, (int)frame->width, results[i], draw_lcd_colors[i % draw_colors_num]);
//                         }
//                     }
//                     else
//                     {
//                         detector.draw_segmentation_results((uint16_t *)frame->buf, {(int)frame->height, (int)frame->width, 3}, draw_lcd_colors, true, 0x0000);
//                     }
//                     break;
//                 }

//             }
//         }

//         if (xQueueCameraFrameColorO)
//         {

//             xQueueSend(xQueueCameraFrameColorO, &frame, portMAX_DELAY);
//         }
//         else if (gReturnFB)
//         {
//             esp_camera_fb_return(frame);
//         }
//         else
//         {
//             free(frame);
//         }
//         vTaskDelay(1);
//     }
// }

// void DFRobot_ESP32S3_BIT::initColorDetection(QueueHandle_t frame_i,QueueHandle_t frame_o, QueueHandle_t contorl, QueueHandle_t result){
//   xQueueCameraFrameColori = frame_i;
//   xQueueCameraFrameColorO = frame_o;
//   xQueueResultColor = result;
//   xMutex = xSemaphoreCreateMutex();
//   xTaskCreatePinnedToCore(task_process_handler_color, "color", 5 * 1024, NULL, 5, NULL, 0);
// }


// // void task_process_handler_printfstr(void *arg)
// // {
// //   camera_fb_t *frame = NULL;
// //   while(1){
// //     if(xQueueReceive(xQueueStrI, &(frame), portMAX_DELAY)){
// //       //fb_gfx_print(frame,x,y,color,str);
// //     }
// //     if(xQueueStrO)
// //       xQueueSend(xQueueStrO, &frame, portMAX_DELAY);
// //     else
// //       esp_camera_fb_return(frame);
// //     vTaskDelay(1);
// //   }
  
// //   xQueueSend(xQueueStrO, &frame, portMAX_DELAY);
// // }
// // void DFRobot_ESP32S3_BIT::printfStr(QueueHandle_t frame_i,QueueHandle_t frame_o,QueueHandle_t printf){
// //   xQueueStrI = frame_i;
// //   xQueueStrO = frame_o;
// //   xTaskCreatePinnedToCore(task_process_handler_printfstr, "printfstr", 1 * 1024, NULL, 5, NULL, 0);
  
// // }