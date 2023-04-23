#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h> // Master copy here: https://github.com/Bodmer/TFT_eSPI

File dataFile;
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

uint16_t sonic[5600];
int count = 0;
int img_count = 50;
String images[50] = {
  "/sonic_stop_1_rev.bin",
  "/sonic_stop_1.bin",
  "/sonic_fast_1_rev.bin",
  "/sonic_fast_1.bin",
  "/sonic_fast_2_rev.bin",
  "/sonic_fast_2.bin",
  "/sonic_fast_3_rev.bin",
  "/sonic_fast_3.bin",
  "/sonic_fast_4_rev.bin",
  "/sonic_fast_4.bin",
  "/sonic_roll_1_rev.bin",
  "/sonic_roll_1.bin",
  "/sonic_roll_2_rev.bin",
  "/sonic_roll_2.bin",
  "/sonic_roll_3_rev.bin",
  "/sonic_roll_3.bin",
  "/sonic_roll_4_rev.bin",
  "/sonic_roll_4.bin",
  "/sonic_run_1_rev.bin",
  "/sonic_run_1.bin",
  "/sonic_run_2_rev.bin",
  "/sonic_run_2.bin",
  "/sonic_run_3_rev.bin",
  "/sonic_run_3.bin",
  "/sonic_run_4_rev.bin",
  "/sonic_run_4.bin",
  "/sonic_run_5_rev.bin",
  "/sonic_run_5.bin",
  "/sonic_run_6_rev.bin",
  "/sonic_run_6.bin",
  "/sonic_run_7_rev.bin",
  "/sonic_run_7.bin",
  "/sonic_run_8_rev.bin",
  "/sonic_run_8.bin",
  "/sonic_stand_1_rev.bin",
  "/sonic_stand_1.bin",
  "/sonic_stand_2_rev.bin",
  "/sonic_stand_2.bin",
  "/sonic_stand_3_rev.bin",
  "/sonic_stand_3.bin",
  "/sonic_stand_4_rev.bin",
  "/sonic_stand_4.bin",
  "/sonic_stand_5_rev.bin",
  "/sonic_stand_5.bin",
  "/sonic_stand_6_rev.bin",
  "/sonic_stand_6.bin",
  "/sonic_star_1.bin",
  "/sonic_star_2.bin",
  "/sonic_star_3.bin",
  "/sonic_star_4.bin",
};
String image = "nothing";

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
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  image = images[count];
  dataFile = SD.open(image, FILE_READ);
  Serial.println(image);
  Serial.println("SD opened");
  if (dataFile) {
    dataFile.read((uint8_t *)&sonic, sizeof(sonic));
    tft.fillScreen(TFT_BLACK);
    tft.pushImage(20, 20, 70, 80, sonic, TFT_BLACK);
  }
  else {
    Serial.println("read failed");
  }
  dataFile.close();
  if (count >= img_count - 1) {
    count = 0;
  }
  else {
    count++;
  }
  delay(1000);
}
