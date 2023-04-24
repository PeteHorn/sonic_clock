#include <SPI.h>
#include <SD.h>
#include "img.h"
#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI

File dataFile1;
File dataFile2;
File dataFile3;
File dataFile4;
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
uint16_t sonic1[5600];
uint16_t sonic2[5600];
uint16_t sonic3[5600];
uint16_t sonic4[5600];

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
  delay(1000);
  if (SD.exists("/sonic_stop_1.bin")) {
    dataFile1 = SD.open("/sonic_stop_1.bin", FILE_WRITE);
    if (dataFile1) {
    dataFile1.write((const uint8_t *)&sonic_stop_1, sizeof(sonic_stop_1));
    }
    else {
      Serial.println("sonic_stop_1 exists already");
    }
    dataFile1.close();
  }
  if (SD.exists("/sonic_stop_1_rev.bin")) {
    dataFile2 = SD.open("/sonic_stop_1_rev.bin", FILE_WRITE);
    if (dataFile2) {
    dataFile2.write((const uint8_t *)&sonic_stop_1_rev, sizeof(sonic_stop_1_rev));
    }
    else {
      Serial.println("sonic_stop_1_rev exists already");
    }
    dataFile2.close();
  }
  if (SD.exists("/sonic_stand_6.bin")) {
    dataFile3 = SD.open("/sonic_stand_6.bin", FILE_WRITE);
    if (dataFile3) {
    dataFile3.write((const uint8_t *)&sonic_stand_6, sizeof(sonic_stand_6));
    } 
    else {
      Serial.println("sonic_stand_6 exists already");
    }
    dataFile3.close();
  }
  if (SD.exists("/sonic_stand_6_rev.bin")) {
    dataFile4 = SD.open("/sonic_stand_6_rev.bin", FILE_WRITE);
    if (dataFile4) {
      dataFile4.write((const uint8_t *)&sonic_stand_6_rev, sizeof(sonic_stand_6_rev));
    }
    else {
      Serial.println("sonic_stand_6_rev exists already");
    }
    dataFile4.close();
  }
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  delay(500);
  dataFile1 = SD.open("/sonic_stop_1.bin", FILE_READ);
  dataFile2 = SD.open("/sonic_stop_1_rev.bin", FILE_READ);
  dataFile3 = SD.open("/sonic_stand_6.bin", FILE_READ);
  dataFile4 = SD.open("/sonic_stand_6_rev.bin", FILE_READ);
  Serial.println("SD opened");
  delay(1000);
  if (dataFile1) {
    dataFile1.read((uint8_t *)&sonic1, sizeof(sonic1));
  }
  dataFile1.close();
  tft.pushImage(20, 20, 70, 80, sonic1, TFT_BLACK);
  if (dataFile2) {
    dataFile2.read((uint8_t *)&sonic2, sizeof(sonic2));
  }
  dataFile2.close();
  tft.pushImage(220, 20, 70, 80, sonic2, TFT_BLACK);
  if (dataFile3) {
    dataFile3.read((uint8_t *)&sonic3, sizeof(sonic3));
  }
  dataFile3.close();
  tft.pushImage(20, 220, 70, 80, sonic3, TFT_BLACK);
  if (dataFile4) {
    dataFile4.read((uint8_t *)&sonic4, sizeof(sonic4));
  }
  tft.pushImage(220, 220, 70, 80, sonic4, TFT_BLACK);
  dataFile4.close();
}

void loop() {
}
