#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI
#include "sonic.h"

#include <WiFi.h>
#include "time.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

TFT_eSprite sonic = TFT_eSprite(&tft);
TFT_eSprite background= TFT_eSprite(&tft);
TFT_eSprite txtSprite= TFT_eSprite(&tft);

struct pos {
  int x;
  int y;
};

//variables
int count = 0;
unsigned long due = 0;
unsigned long currenttime = 0;
pos sonic_pos = {25, 220};
bool moving = true;
bool rev = false;

const char* ssid       = "Horn_Home";
const char* password   = "hJ4tq549zVN5";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

//constants
#define move_period 10000   //Constant to define how often Sonic should move to a new location (ms)
#define left_limit 5       //Furthest left sonic can move
#define right_limit 385     //Furthest right sonic can move

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");
  // Initialise the screen
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  background.createSprite(120, 120);
  background.setSwapBytes(true);
  background.fillSprite(TFT_BLACK);
  sonic.createSprite(80, 80);
  sonic.setSwapBytes(true);
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
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

//Currently it is going through the first move to the right correctly (where currenttime, due and rev are printed) but not then changing state to go back the other way
void loop() {
  currenttime = millis();
  if (moving == true) {
    Serial.println(sonic_pos.x);
    if (sonic_pos.x < left_limit || sonic_pos.x > right_limit) {
      rev = !rev;
      due = currenttime + move_period;
      moving = false;
    }
    if (rev == false) {
      sonic_pos.x += 10;
    }
    else {
      sonic_pos.x -= 10;
    }
    count = handle_fast (count, rev);
  }
  else {
    if (currenttime > due) {
      moving = true;
    }
    else {
      count = handle_stand (count, rev);
    }
  }
//  background.fillSprite(TFT_BLACK);
  sonic.pushToSprite(&background, 20, 20);
  background.pushSprite(sonic_pos.x, sonic_pos.y);
  printLocalTime();
}

int handle_fast (int count, bool rev) {
  switch (count) {
    case 0:
      if (rev == false) {
        sonic.pushImage(0, 0, 49, 64, sonic_fast_1, TFT_BLACK);
      }
      else {
        sonic.pushImage(0, 0, 49, 64, sonic_fast_1_rev, TFT_BLACK);
      }
      break;
    case 1:
      if (rev == false) {
        sonic.pushImage(0, 0, 50, 63, sonic_fast_2, TFT_BLACK);
      }
      else {
        sonic.pushImage(0, 0, 50, 63, sonic_fast_2_rev, TFT_BLACK);
      }
      break;
    case 2:
      if (rev == false) {
        sonic.pushImage(0, 0, 50, 64, sonic_fast_3, TFT_BLACK);
      }
      else {
        sonic.pushImage(0, 0, 50, 64, sonic_fast_3_rev, TFT_BLACK);
      }
      break;
    case 3:
      if (rev == false) {
        sonic.pushImage(0, 0, 49, 60, sonic_fast_4, TFT_BLACK);
      }
      else {
        sonic.pushImage(0, 0, 49, 60, sonic_fast_4_rev, TFT_BLACK);
      }
      break;
  }
  count++;
  if (count >= 4){
    count = 0;
  }
  delay(20);
  return count;
}

int handle_stand (int count, bool rev) {
  switch (count) {
    case 0:
      sonic.pushImage(0, 0, 58, 66, sonic_stand_1, TFT_BLACK);
//      if (rev == false) {
//        sonic.pushImage(0, 0, 58, 66, sonic_stand_1, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(0, 0, 58, 66, sonic_stand_1_rev, TFT_BLACK);
//      }
      break;
    case 1:
      sonic.pushImage(0, 0, 54, 66, sonic_stand_2, TFT_BLACK);
//      if (rev == false) {
//        sonic.pushImage(0, 0, 54, 66, sonic_stand_2, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(0, 0, 54, 66, sonic_stand_2_rev, TFT_BLACK);
//      }
      break;
    case 2:
      sonic.pushImage(0, 0, 49, 66, sonic_stand_3, TFT_BLACK);
//      if (rev == false) {
//        sonic.pushImage(0, 0, 49, 66, sonic_stand_3, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(0, 0, 49, 66, sonic_stand_3_rev, TFT_BLACK);
//      }
      break;
    case 3:
      sonic.pushImage(0, 0, 46, 65, sonic_stand_4, TFT_BLACK);
//      if (rev == false) {
//        sonic.pushImage(0, 0, 46, 65, sonic_stand_4, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(0, 0, 46, 65, sonic_stand_4_rev, TFT_BLACK);
//      }
      break;
    case 4:
      sonic.pushImage(0, 0, 49, 66, sonic_stand_3, TFT_BLACK);
//      if (rev == false) {
//        sonic.pushImage(0, 0, 44, 67, sonic_stand_5, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(0, 0, 44, 67, sonic_stand_5_rev, TFT_BLACK);
//      }
      break;
    case 5:
      sonic.pushImage(0, 0, 54, 66, sonic_stand_2, TFT_BLACK);
//      if (rev == false) {
//        sonic.pushImage(0, 0, 47, 66, sonic_stand_6, TFT_BLACK);
//      }
//      else {
//        sonic.pushImage(0, 0, 47, 66, sonic_stand_6_rev, TFT_BLACK);
//      }
      break;
  }
  count++;
  if (count >= 6){
    count = 0;
  }
  delay(100);
  return count;
}
//
////Handle_run not working as well as handle_stand and handle_fast
//int handle_run (int count, bool rev) {
//  switch (count) {
//    case 0:
//      redraw_sonic(sonic_run_1, 52, 66, rev);
//      break;
//    case 1:
//      redraw_sonic(sonic_run_2, 42, 68, rev);
//      break;
//    case 2:
//      redraw_sonic(sonic_run_3, 41, 68, rev);
//      break;
//    case 3:
//      redraw_sonic(sonic_run_4, 58, 65, rev);
//      break;
//    case 4:
//      redraw_sonic(sonic_run_5, 50, 69, rev);
//      break;
//    case 5:
//      redraw_sonic(sonic_run_6, 42, 68, rev);
//      break;
//    case 6:
//      redraw_sonic(sonic_run_7, 49, 70, rev);
//      break;
//    case 7:
//      redraw_sonic(sonic_run_8, 64, 68, rev);
//      break;
//  }
//  count++;
//  if (count >= 6){
//    count = 0;
//  }
//  delay(50);
//  return count;
//}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
