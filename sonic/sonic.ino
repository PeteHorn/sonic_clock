#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI

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
String fast_right[4] = {
  "/sonic_fast_1.bin",
  "/sonic_fast_2.bin",
  "/sonic_fast_3.bin",
  "/sonic_fast_4.bin"
};
String fast_left[4] = {
  "/sonic_fast_1_rev.bin",
  "/sonic_fast_2_rev.bin",
  "/sonic_fast_3_rev.bin",
  "/sonic_fast_4_rev.bin",
};
String roll_right[4] = {
  "/sonic_roll_1.bin",
  "/sonic_roll_2.bin",
  "/sonic_roll_3.bin",
  "/sonic_roll_4.bin"
//  "/sonic_roll_5.bin"
};
String roll_left[4] = {
  "/sonic_roll_1_rev.bin",
  "/sonic_roll_2_rev.bin",
  "/sonic_roll_3_rev.bin",
  "/sonic_roll_4_rev.bin"
//  "/sonic_roll_5_rev.bin"
};
String run_right[4] = {
  "/sonic_run_1.bin",
  "/sonic_run_2.bin",
  "/sonic_run_3.bin",
  "/sonic_run-4.bin"
//  "/sonic_run_5.bin",
//  "/sonic_run_6.bin",
//  "/sonic_run-7.bin",
//  "/sonic_run_8.bin"
};
String run_left[4] = {
  "/sonic_run_1_rev.bin",
  "/sonic_run_2_rev.bin",
  "/sonic_run_3_rev.bin",
  "/sonic_run_4_rev.bin"
//  "/sonic_run_5_rev.bin",
//  "/sonic_run_6_rev.bin",
//  "/sonic_run_7_rev.bin",
//  "/sonic_run_8_rev.bin"
};
String stand_right[4] = {
  "/sonic_stand_1.bin",
  "/sonic_stand_2.bin",
  "/sonic_stand_3.bin",
  "/sonic_stand-4.bin"
//  "/sonic_stand_5.bin",
//  "/sonic_stand_6.bin"
};
String stand_left[4] = {
  "/sonic_stand_1_rev.bin",
  "/sonic_stand_2_rev.bin",
  "/sonic_stand_3_rev.bin",
  "/sonic_stand_4_rev.bin"
//  "/sonic_stand_5_rev.bin",
//  "/sonic_stand_6_rev.bin"
};
String star[4] = {
  "/sonic_star_1.bin",
  "/sonic_star_2.bin",
  "/sonic_star_3.bin",
  "/sonic_star_4.bin"
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
File dataFile1, dataFile2, dataFile3, dataFile4;
int count = 0;
int setsize = 4;
unsigned long due = 0;
unsigned long currenttime = 0;
pos sonic_pos = {init_sonic_x, init_sonic_y};
bool moving = true;
int state = running;
bool rev = false;
int jump_ani_count = 0;
int jump_height_count = 0;
float jumpheight = 0;
uint16_t sonic_1[5600], sonic_2[5600], sonic_3[5600], sonic_4[5600];
unsigned long lastupdate;
int imgset;

String image;
int array_length;

void load_images(String image_set[]) {
//  setsize = set_size;
  dataFile1 = SD.open(image_set[0], FILE_READ);
  dataFile1.read((uint8_t *)&sonic_1, sizeof(sonic_1));
  dataFile1.close();
  dataFile2 = SD.open(image_set[1], FILE_READ);
  dataFile2.read((uint8_t *)&sonic_2, sizeof(sonic_2));
  dataFile2.close();
  dataFile3 = SD.open(image_set[2], FILE_READ);
  dataFile3.read((uint8_t *)&sonic_3, sizeof(sonic_3));
  dataFile3.close();
  dataFile4 = SD.open(image_set[3], FILE_READ);
  dataFile4.read((uint8_t *)&sonic_4, sizeof(sonic_4));
  dataFile4.close();
//  dataFile5 = SD.open(image_set[4], FILE_READ);
//  dataFile5.read((uint8_t *)&sonic_5, sizeof(sonic_5));
//  dataFile5.close();
//  dataFile6 = SD.open(image_set[5], FILE_READ);
//  dataFile6.read((uint8_t *)&sonic_6, sizeof(sonic_6));
//  dataFile6.close();
//  dataFile7 = SD.open(image_set[6], FILE_READ);
//  dataFile7.read((uint8_t *)&sonic_7, sizeof(sonic_7));
//  dataFile7.close();
//  dataFile8 = SD.open(image_set[7], FILE_READ);
//  dataFile8.read((uint8_t *)&sonic_8, sizeof(sonic_8));
//  dataFile8.close();
}

void draw_sonic() {
  currenttime = millis();
  if (currenttime > due) {
    if (count < setsize - 1) {
    count++;
    }
    else {
      count = 0;
    }
    Serial.println(setsize);
    if (count == 0) {
      sonicSprite.pushImage(20, 20, 70, 80, sonic_1, TFT_BLACK);
    }
    if (count == 1) {
      sonicSprite.pushImage(20, 20, 70, 80, sonic_2, TFT_BLACK);
    }
    if (count == 2) {
      sonicSprite.pushImage(20, 20, 70, 80, sonic_3, TFT_BLACK);
    }
    if (count == 3) {
      sonicSprite.pushImage(20, 20, 70, 80, sonic_4, TFT_BLACK);
    }
    sonicSprite.pushToSprite(&background, 20, 80);
    due = due + 30;
  }
  delay(10);
}

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
  load_images(roll_right);
}

void loop() {
  if (millis() > lastupdate + 10000) {
    lastupdate = millis();
    imgset++;
    if (imgset == 0) {
      load_images(run_right);
      Serial.println("run right");
    }
    if (imgset == 1) {
      load_images(run_left);
      Serial.println("run left");
    }
    if (imgset == 2) {
      load_images(roll_right);
      Serial.println("roll right");
    }
    if (imgset == 3) {
      load_images(roll_left);
      Serial.println("roll left");
    }
    if (imgset == 4) {
      load_images(fast_right);
      Serial.println("fast right");
    }
    if (imgset == 5) {
      load_images(fast_left);
      Serial.println("fast left");
    }
    if (imgset == 6) {
      load_images(stand_right);
      Serial.println("stand right");
    }
    if (imgset == 7) {
      load_images(stand_left);
      Serial.println("stand left");
    }
    if (imgset == 8) {
      load_images(star);
      Serial.println("star");
      imgset = -1;
    }
  }
  draw_sonic();
  background.pushSprite(sonic_pos.x, sonic_pos.y);
}
