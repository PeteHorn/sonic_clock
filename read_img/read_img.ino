#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI

File dataFile;
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

uint16_t sonic[30000];

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
  dataFile = SD.open("/stand_01.bin", FILE_READ);
  Serial.println("SD opened");
  if (dataFile) {
    dataFile.read((uint8_t *)&sonic, sizeof(sonic));
  }
  else {
    Serial.println("read failed");
  }
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(20, 20, 150, 200, sonic, TFT_BLACK);
  dataFile.close();
}

void loop() {
}
