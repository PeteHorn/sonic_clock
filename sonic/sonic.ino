#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI
#include "sonic.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

TFT_eSprite spr = TFT_eSprite(&tft);

struct pos {
  int x;
  int y;
};

//variables
int count = 0;
unsigned long due = 0;
unsigned long currenttime = 0;
pos sonic_pos = {25, 240};
bool moving = false;
bool rev = false;

//constants
#define move_period 17000   //Constant to define how often Sonic should move to a new location (ms)
#define left_limit 25       //Furthest left sonic can move
#define right_limit 405     //Furthest right sonic can move

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");
  // Initialise the screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
//  char buf[32];
  spr.createSprite(80, 80);
}

//Currently it is going through the first move to the right correctly (where currenttime, due and rev are printed) but not then changing state to go back the other way
void loop() {
  currenttime = millis();
  if (moving == true) {
    Serial.println(rev);
    if (rev == true) {
      sonic_pos.x -= 10;
      if (sonic_pos.x <= left_limit) {
        moving = false;
      }
    }
    else {
      sonic_pos.x += 10;
      if (sonic_pos.x >= right_limit) {
        moving = false;
      }
    }
  }
  else {
    if (currenttime > due) {
      moving = true;
      rev != rev;
      due = currenttime + move_period;
      Serial.println(currenttime);
      Serial.println(due);
      Serial.println(rev);
    }
  }
  count = handle_fast (count, false);
  spr.pushSprite(sonic_pos.x, sonic_pos.y);
}

//move_data calc_move (move_data movedata) {
//  currenttime = millis();
//  if (currenttime > movedata.due) {
//    
//  }
//}

int handle_fast (int count, bool rev) {
  switch (count) {
    case 0:
      redraw_sonic(sonic_fast_1, 49, 64, rev);
      break;
    case 1:
      redraw_sonic(sonic_fast_2, 50, 63, rev);
      break;
    case 2:
      redraw_sonic(sonic_fast_3, 50, 64, rev);
      break;
    case 3:
      redraw_sonic(sonic_fast_4, 49, 60, rev);
      break;
  }
  count++;
  if (count >= 4){
    count = 0;
  }
  delay(50);
  return count;
}

int handle_stand (int count, bool rev) {
  switch (count) {
    case 0:
      redraw_sonic(sonic_stand_1, 58, 66, rev);
      break;
    case 1:
      redraw_sonic(sonic_stand_2, 54, 66, rev);
      break;
    case 2:
      redraw_sonic(sonic_stand_3, 49, 66, rev);
      break;
    case 3:
      redraw_sonic(sonic_stand_4, 46, 65, rev);
      break;
    case 4:
      redraw_sonic(sonic_stand_5, 44, 67, rev);
      break;
    case 5:
      redraw_sonic(sonic_stand_6, 47, 66, rev);
      break;
  }
  count++;
  if (count >= 6){
    count = 0;
  }
  delay(100);
  return count;
}

//Handle_run not working as well as handle_stand and handle_fast
int handle_run (int count, bool rev) {
  switch (count) {
    case 0:
      redraw_sonic(sonic_run_1, 52, 66, rev);
      break;
    case 1:
      redraw_sonic(sonic_run_2, 42, 68, rev);
      break;
    case 2:
      redraw_sonic(sonic_run_3, 41, 68, rev);
      break;
    case 3:
      redraw_sonic(sonic_run_4, 58, 65, rev);
      break;
    case 4:
      redraw_sonic(sonic_run_5, 50, 69, rev);
      break;
    case 5:
      redraw_sonic(sonic_run_6, 42, 68, rev);
      break;
    case 6:
      redraw_sonic(sonic_run_7, 49, 70, rev);
      break;
    case 7:
      redraw_sonic(sonic_run_8, 64, 68, rev);
      break;
  }
  count++;
  if (count >= 6){
    count = 0;
  }
  delay(75);
  return count;
}

//sonic_pos calc_move(sonic_pos input) {
//  current_time = millis();
//  if (current_time > input.due) {
//    
//  }
//  sonic_pos output;
//}

void redraw_sonic(int img[], int x_res, int y_res, bool rev) {
  int s = 0;      //Total elements of the array counter (as the image arrays are 1D)
  int x_pos = 0;  //Temp location to store the x pixel location, required in case the sprite is reversed
  for (int y=0; y<y_res; y++) {
    for (int x=0; x<x_res; x++) {
      if (rev) {
        x_pos = x_res - x;
      }
      else {
        x_pos = x;
      }
      spr.drawPixel(x_pos, y, img[s]);
      s++;
    }
  }
}
