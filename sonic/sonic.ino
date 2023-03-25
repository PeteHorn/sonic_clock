#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI
#include "sonic.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

TFT_eSprite spr = TFT_eSprite(&tft);

int count = 0;

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

void loop() {
  switch (count) {
    case 0:
      redraw_sonic(sonic_stand_1, 58, 66);
      break;
    case 1:
      redraw_sonic(sonic_stand_2, 54, 66);
      break;
    case 2:
      redraw_sonic(sonic_stand_3, 49, 66);
      break;
    case 3:
      redraw_sonic(sonic_stand_4, 46, 65);
      break;
    case 4:
      redraw_sonic(sonic_stand_5, 44, 67);
      break;
    case 5:
      redraw_sonic(sonic_stand_6, 47, 66);
      break;
  }
  count++;
  if (count == 3){
    count = 0;
  }
  spr.pushSprite(50, 50);
  delay(1000);
}

void redraw_sonic(int img[], int x_res, int y_res) {
  int s = 0;
  for (int y=0; y<y_res; y++) {
    for (int x=0; x<x_res; x++) {
      spr.drawPixel(x, y, img[s]);
      s++;
    }
  }
}
