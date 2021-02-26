
#include <Arduino.h>
#include <M5Stack.h>

#define KEYBOARD_I2C_ADDR     0X08
#define KEYBOARD_INT          5

void text();

String sku_input;
String file_name;

String sku, tana;
bool sku_get = false;
bool tana_get = false;
bool tana_bool = false;
unsigned long count = 1;
unsigned int ccnt = 1;


void tanaprint(int check_sku) {
  bool ok = false;
  File dataFile = SD.open("/stock_csv.csv");
  if (dataFile) {
    if (check_sku >= 505)dataFile.seek(50000);//495
    if (check_sku >= 1095)dataFile.seek(100000);//1085
    if (check_sku >= 1509)dataFile.seek(150000);//1489
    if (check_sku >= 1995)dataFile.seek(200000);//1985
    if (check_sku >= 2534)dataFile.seek(250000);//2524
    if (check_sku >= 3037)dataFile.seek(300000);//3027
    if (check_sku >= 3570)dataFile.seek(350000);//3560
    if (check_sku >= 4095)dataFile.seek(400000);//4085
    if (check_sku >= 5535)dataFile.seek(450000);//5525
    if (check_sku >= 6131)dataFile.seek(500000);//6121
    if (check_sku >= 6679)dataFile.seek(550000);//6669

    while (dataFile.available()) {
      char buff = dataFile.read();
      if (buff == '\n') {
        sku_get = true;
        ccnt = 0;
      }
      if (sku_get == true) {
        if (buff == ',') {
          if (sku.toInt() == check_sku) {
            Serial.print(sku);
            tana_bool = true;
          }
          sku = "";
          sku_get = false;
        } else {
          sku.concat(buff);
        }
      }
      if (buff == ',') {
        ccnt++;
        if (ccnt == 5) {
          tana_get = true;
        }
      }
      if (tana_get == true) {
        tana.concat(buff);
        if (ccnt == 6) {
          tana_get = false;
          if (tana_bool) {
            Serial.print(tana);
            M5.Lcd.clear(BLACK);
            text();
            M5.Lcd.setCursor(100, 100);
            M5.Lcd.setTextSize(2);

            if (tana == ",,") {
              M5.Lcd.print("not set");
            } else {
              tana.replace(",", "");
              M5.Lcd.print(tana);
            }
            text();
            tana_bool = false;
            tana = "";
            ok = true;
            break;
          }
          tana = "";

        }
      }
    }
  } else {
    Serial.println(F("SDcard_error"));
  }
  if (ok == false) {
    M5.Lcd.clear(BLACK);
    text();
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.setTextSize(3);
    M5.Lcd.print("???");

    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(30, 10);
    M5.Lcd.print("SKU:");
  }
  Serial.print("end");
}

void text() {
  M5.Lcd.setCursor(50, 200);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("DEL");

  M5.Lcd.setCursor(120, 200);
  M5.Lcd.printf("SEARCH");
  M5.Lcd.setCursor(30, 10);
  M5.Lcd.print("SKU:");
}

void setup()
{
  M5.begin();
  Wire.begin();
  M5.Lcd.setTextFont(4);
  pinMode(KEYBOARD_INT, INPUT_PULLUP);
  text();
}

void loop()
{
  M5.update();
  if (M5.BtnA.wasReleased()) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
    sku_input = "";
    text();
  } else if (M5.BtnB.wasReleased()) {

    M5.Lcd.setCursor(95, 100);
    M5.Lcd.setTextSize(0.1);
    M5.Lcd.clear(BLACK);
    text();
    M5.Lcd.setCursor(95, 100);
    M5.Lcd.print("wait..");

    unsigned int skui = sku_input.toInt();
    tanaprint(skui);
    sku_input = "";
  } else if (M5.BtnC.wasReleased()) {
  }
  if (digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);
    while (Wire.available()) {
      uint8_t key_val = Wire.read();
      if (key_val != 0) {
        if (key_val > 47 && key_val < 58) {
          sku_input.concat((char)key_val);
          Serial.print((char)key_val);
          M5.Lcd.print((char)key_val);
        }
      }
    }
  }
}
