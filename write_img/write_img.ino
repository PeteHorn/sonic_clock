#include <SPI.h>
#include <SD.h>
#include "img.h"
#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI

File dataFile;
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

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
  delay(10000);
  dataFile = SD.open("/stand_01.bin", FILE_WRITE);
  Serial.println("SD opened");
  delay(10000);
  if (dataFile) {
    Serial.println(sizeof(sonic_stand_1));
    dataFile.write((const uint8_t *)&sonic_stand_1, sizeof(sonic_stand_1));
  }
  else {
    Serial.println("write failed");
  }
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(20, 20, 150, 200, sonic_stand_1, TFT_BLACK);
  dataFile.close();
}

void loop() {
}
