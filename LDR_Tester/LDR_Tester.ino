#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_MOSI 9
#define OLED_CLK 10
#define OLED_DC 11
#define OLED_CS 12
#define OLED_RESET 13

#define BX 0
#define BY 0
#define BW 40
#define BH 20

#define VX BX + 80
#define VY 0
#define VW 40
#define VH 20

#define EX 4
#define EY 21
#define EW 30
#define EH 10

#define RAW_COUNT 50
#define METER_PIN 5
#define CVALUE 250
#define CAPTURE_DELAY 10
#define DEFAULT_ASA 400

// These constants, define values needed for the LDR readings and ADC
#define MAX_ADC_READING 1023
#define ADC_REF_VOLTAGE 5.0
#define REF_RESISTANCE 9930  // measure this for best results
//#define LUX_CALC_SCALAR           12518931
//#define LUX_CALC_EXPONENT         -1.405

int asa = DEFAULT_ASA;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

double luxToEv(double lux) {
  if (lux <= 0) {
    return -INFINITY;  // Throw exception for negative numbers
  }
  return log(lux * asa / CVALUE) / log(2.0);
}

float rawToLdr(double ldrRaw) {
  float ldrResistance;
  float resistorVoltage = (float)ldrRaw / MAX_ADC_READING * ADC_REF_VOLTAGE;

  float ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

  return ldrResistance = ldrVoltage / resistorVoltage * REF_RESISTANCE;
}

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1)
      ;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {

  //float resistorVoltage, ldrVoltage;
 // float ldrResistance;
  float ldrLux;
  uint32_t ldrRaw = 0;

  for (int x = 0; x < RAW_COUNT; x++) {
    delay(CAPTURE_DELAY);
    ldrRaw += analogRead(METER_PIN);
  }
  ldrRaw /= RAW_COUNT;

//float rawToLdr(doulbe ldrRaw) {

  float ldrResistance = rawToLdr(ldrRaw);

  //float volt = a * max_v / 1023;
  //float lux = (volt * (-4000)) + 18000;

  ldrLux = 3.36899 * pow(10, 9) * (pow(ldrResistance, -1.56617));

  double ev = luxToEv(ldrLux);

  display.fillRect(BX, BY, BW, BH, SSD1306_BLACK);
  display.setCursor(BX, BY);
  display.println(ldrResistance);

  //display.setCursor(BX, BY + 12);
  //display.println(a);

  display.fillRect(VX, VY, VW, VH, SSD1306_BLACK);
  display.setCursor(VX, VY);
  display.println(ldrLux);

  Serial.print(ldrResistance);
  Serial.print('\t');
  Serial.println(ldrLux);

  //display.setCursor(VX, VY+ 12);
  //display.println(a);
  /* 
  display.fillRect(SX + 1, SY + 1, SW - 2, SH - 2, SSD1306_BLACK);
  display.setCursor(SX + 3, SY + 2);
  //display.println(speeds[count]);
  display.println(ev);

  display.setTextSize(1);
*/
  display.fillRect(EX, EY, EW, EH, SSD1306_BLACK);
  display.setCursor(EX, EY);
  display.println(ev);

  // Serial.print("EV: ");
  //Serial.print(ev);

  //Serial.print(" AD: ");
  //Serial.print(ad);

  //Serial.print(" LuxV: ");
  //Serial.print(luxVM);


  //Serial.print(" LuxB: ");
  //Serial.println(lux);


  display.display();
}