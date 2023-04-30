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
File dataFile1, dataFile2, dataFile3, dataFile4;
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
uint16_t sonic_1[5600];
uint16_t sonic_2[5600];
uint16_t sonic_3[5600];
uint16_t sonic_4[5600];
int set_size;

String image;
int array_length;

int load_images(String image_set[]) {
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
  int setsize = sizeof(image_set);
  return setsize;  
}

int draw_sonic(int intcount, int setsize) {
  if (intcount < setsize - 1) {
    intcount++;
  }
  else {
    intcount = 0;
  }
  switch (count) {
    case 0:
      sonicSprite.pushImage(20, 20, 70, 80, sonic_1, TFT_BLACK);
    case 1:
      sonicSprite.pushImage(20, 20, 70, 80, sonic_2, TFT_BLACK);
    case 2:
      sonicSprite.pushImage(20, 20, 70, 80, sonic_3, TFT_BLACK);
    case 3:
      sonicSprite.pushImage(20, 20, 70, 80, sonic_4, TFT_BLACK);
  }
  return intcount;
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
  set_size = load_images(run_left);
}

void loop() {
  count = draw_sonic(count, set_size);
  sonicSprite.pushToSprite(&background, 20, 80);
  background.pushSprite(sonic_pos.x, sonic_pos.y);
  delay(3000);
}
