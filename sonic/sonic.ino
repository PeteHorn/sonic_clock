#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI
#include "sonic.h"

#include <WiFi.h>
#include "time.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

TFT_eSprite sonicSprite = TFT_eSprite(&tft);
TFT_eSprite background= TFT_eSprite(&tft);
TFT_eSprite txtSprite= TFT_eSprite(&tft);

//types
struct pos {
  int x;
  int y;
};

enum states {
  running,
  standing,
  jumping
};
String run_right[4] = {
  "/sonic_fast_1.bin",
  "/sonic_fast_2.bin",
  "/sonic_fast_3.bin",
  "/sonic_fast_4.bin"
};
String run_left[4] = {
  "/sonic_fast_1_rev.bin",
  "/sonic_fast_2_rev.bin",
  "/sonic_fast_3_rev.bin",
  "/sonic_fast_4_rev.bin",
};

//constants
#define move_period 10000   //Constant to define how often Sonic should move to a new location (ms)
#define left_limit 5       //Furthest left sonic can move
#define right_limit 385     //Furthest right sonic can move
#define num_jumps 5          //number of times sonic jumps every time he goes into the jump state
#define jump_points 10      //number of points on the sine curve that sonic jumps to for each jump
#define init_sonic_x -20     //Sonics initial X position
#define init_sonic_y 120     //Sonics initial Y position

const int states_arr[3] = {running, standing, jumping};
const float jump_y[jump_points] = {0, 0.309, 0.588, 0.809, 0.951, 1, 0.951, 0.809, 0.588, 0.309};
const float jump_heights[num_jumps] = {60, 120, 180, 200};

const char* ssid       = "Horn_Home";
const char* password   = "hJ4tq549zVN5";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

//variables
File dataFile;
int count = 0;
unsigned long due = 0;
unsigned long currenttime = 0;
pos sonic_pos = {init_sonic_x, init_sonic_y};
bool moving = true;
int state = running;
bool rev = false;
int jump_ani_count = 0;
int jump_height_count = 0;
float jumpheight = 0;
uint16_t sonic[5600];
String image;
int array_length;

void setup() {
  Serial.begin(115200);
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  else {
    Serial.println("SD card found");
  }
  Serial.println("Booting...");
  // Initialise the screen
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  background.createSprite(160, 300);
  background.setSwapBytes(true);
  background.fillSprite(TFT_BLACK);
  sonicSprite.createSprite(100, 100);
  sonicSprite.setSwapBytes(true);
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
//  currenttime = millis();
//  if (state == running) {
//    Serial.println(sonic_pos.x);
//    if (sonic_pos.x < left_limit || sonic_pos.x > right_limit) {
//      if (sonic_pos.x < 240) {
//        rev = false;
//      }
//      else {
//        rev = true;
//      }
//      due = currenttime + move_period;
//      state = standing;
//    }
//    if (rev == false) {
//      sonic_pos.x += 10;
//    }
//    else {
//      sonic_pos.x -= 10;
//    }
//    count = handle_fast (count, rev);
//  }
//  else if (state == standing) {
//    if (currenttime > due) {
//      state = states_arr[random(3)];
//    }
//    else {
//      count = handle_stand (count, rev);
//    }
//  }
//  else if (state == jumping) {
//    if (jump_ani_count >= num_jumps) {
//      //jump animation complete, reset and go back to standing
//      jump_ani_count = 0;
//      jump_height_count = 0;
//      sonic_pos.y = init_sonic_y;
//      state = standing;
//    }
//    else {
//      if (jump_height_count >= jump_points) {
//        //Finished a single jump, move on to the next height
//        jump_ani_count++;
//        jump_height_count = 0;
//      }
//      else {
//        jumpheight = jump_heights[jump_ani_count] * jump_y[jump_height_count];
//        jump_height_count++;
//        sonic_pos.y = init_sonic_y - jumpheight;
//      }
//    }
//  }
//  else {
//    //should never enter this state - immediately choose standing if it does
//    int state = standing;
//  }
  image = run_right[count];
  dataFile = SD.open(image, FILE_READ);
  dataFile.read((uint8_t *)&sonic, sizeof(sonic));
  sonicSprite.pushImage(20, 20, 70, 80, sonic, TFT_BLACK);
  sonicSprite.pushToSprite(&background, 20, 80);
  background.pushSprite(sonic_pos.x, sonic_pos.y);
  array_length = sizeof(run_right) / sizeof(run_right[0]);
  if (count >= array_length - 1) {
    count = 0;
  }
  else {
    count++;
  }
  delay(30);
////  printLocalTime();
//  Serial.println(state);
}
//
////jump animation currently using the fast images until the SD card is available
//int handle_jump (int count, bool rev) {
//  switch (count) {
//    case 0:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 49, 64, sonic_fast_1, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 49, 64, sonic_fast_1_rev, TFT_BLACK);
//      }
//      break;
//    case 1:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 50, 63, sonic_fast_2, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 50, 63, sonic_fast_2_rev, TFT_BLACK);
//      }
//      break;
//    case 2:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 50, 64, sonic_fast_3, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 50, 64, sonic_fast_3_rev, TFT_BLACK);
//      }
//      break;
//    case 3:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 49, 60, sonic_fast_4, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 49, 60, sonic_fast_4_rev, TFT_BLACK);
//      }
//      break;
//  }
//  count++;
//  if (count >= 4){
//    count = 0;
//  }
//  delay(5);
//  return count;
//}
//
//int handle_fast (int count, bool rev) {
//  switch (count) {
//    case 0:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 49, 64, sonic_fast_1, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 49, 64, sonic_fast_1_rev, TFT_BLACK);
//      }
//      break;
//    case 1:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 50, 63, sonic_fast_2, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 50, 63, sonic_fast_2_rev, TFT_BLACK);
//      }
//      break;
//    case 2:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 50, 64, sonic_fast_3, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 50, 64, sonic_fast_3_rev, TFT_BLACK);
//      }
//      break;
//    case 3:
//      if (rev == false) {
//        sonic.pushImage(20, 20, 49, 60, sonic_fast_4, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(20, 20, 49, 60, sonic_fast_4_rev, TFT_BLACK);
//      }
//      break;
//  }
//  count++;
//  if (count >= 4){
//    count = 0;
//  }
//  delay(5);
//  return count;
//}
//
//int handle_stand (int count, bool rev) {
//  switch (count) {
//    case 0:
//      sonic.pushImage(20, 20, 58, 66, sonic_stand_1, TFT_BLACK);
////      if (rev == false) {
////        sonic.pushImage(20, 20, 58, 66, sonic_stand_1, TFT_BLACK);
////      }
////      else {
////        sonic.pushImage(20, 20, 58, 66, sonic_stand_1_rev, TFT_BLACK);
////      }
//      break;
//    case 1:
//      sonic.pushImage(20, 20, 54, 66, sonic_stand_2, TFT_BLACK);
////      if (rev == false) {
////        sonic.pushImage(20, 20, 54, 66, sonic_stand_2, TFT_BLACK);
////      }
////      else {
////        sonic.pushImage(20, 20, 54, 66, sonic_stand_2_rev, TFT_BLACK);
////      }
//      break;
//    case 2:
//      sonic.pushImage(20, 20, 49, 66, sonic_stand_3, TFT_BLACK);
////      if (rev == false) {
////        sonic.pushImage(20, 20, 49, 66, sonic_stand_3, TFT_BLACK);
////      }
////      else {
////        sonic.pushImage(20, 20, 49, 66, sonic_stand_3_rev, TFT_BLACK);
////      }
//      break;
//    case 3:
//      sonic.pushImage(20, 20, 46, 65, sonic_stand_4, TFT_BLACK);
////      if (rev == false) {
////        sonic.pushImage(20, 20, 46, 65, sonic_stand_4, TFT_BLACK);
////      }
////      else {
////        sonic.pushImage(20, 20, 46, 65, sonic_stand_4_rev, TFT_BLACK);
////      }
//      break;
//    case 4:
//      sonic.pushImage(20, 20, 49, 66, sonic_stand_3, TFT_BLACK);
////      if (rev == false) {
////        sonic.pushImage(20, 20, 44, 67, sonic_stand_5, TFT_BLACK);
////      }
////      else {
////        sonic.pushImage(20, 20, 44, 67, sonic_stand_5_rev, TFT_BLACK);
////      }
//      break;
//    case 5:
//      sonic.pushImage(20, 20, 54, 66, sonic_stand_2, TFT_BLACK);
////      if (rev == false) {
////        sonic.pushImage(20, 20, 47, 66, sonic_stand_6, TFT_BLACK);
////      }
////      else {
////        sonic.pushImage(20, 20, 47, 66, sonic_stand_6_rev, TFT_BLACK);
////      }
//      break;
//  }
//  count++;
//  if (count >= 6){
//    count = 0;
//  }
//  delay(50);
//  return count;
//}
////
//////Handle_run not working as well as handle_stand and handle_fast
////int handle_run (int count, bool rev) {
////  switch (count) {
////    case 0:
////      redraw_sonic(sonic_run_1, 52, 66, rev);
////      break;
////    case 1:
////      redraw_sonic(sonic_run_2, 42, 68, rev);
////      break;
////    case 2:
////      redraw_sonic(sonic_run_3, 41, 68, rev);
////      break;
////    case 3:
////      redraw_sonic(sonic_run_4, 58, 65, rev);
////      break;
////    case 4:
////      redraw_sonic(sonic_run_5, 50, 69, rev);
////      break;
////    case 5:
////      redraw_sonic(sonic_run_6, 42, 68, rev);
////      break;
////    case 6:
////      redraw_sonic(sonic_run_7, 49, 70, rev);
////      break;
////    case 7:
////      redraw_sonic(sonic_run_8, 64, 68, rev);
////      break;
////  }
////  count++;
////  if (count >= 6){
////    count = 0;
////  }
////  delay(50);
////  return count;
////}
//
//void printLocalTime()
//{
//  struct tm timeinfo;
//  if(!getLocalTime(&timeinfo)){
//    Serial.println("Failed to obtain time");
//    return;
//  }
//  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
//}
