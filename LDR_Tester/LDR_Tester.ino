#include <SPI.h>
#include <Wire.h>
//#include <math.h>
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

#define ADC_COUNT 50
#define METER_PIN 5
#define CVALUE 250
#define CAPTURE_DELAY 10
#define DEFAULT_ASA 400.0
#define MAX_ADC_READING 1023
#define ADC_REF_VOLTAGE 5.0
#define REF_RESISTANCE 9930
#define LUX_CALC_SCALAR 3.36899 * pow(10, 9)
#define LUX_CALC_EXPONENT -1.56617

int asa = DEFAULT_ASA;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

float luxToEv(float lux) {
  if (lux <= 0) {
    return -INFINITY;  // Throw exception for negative numbers
  }
  return log(lux * asa / CVALUE) / log(2.0);
}

float ldrRawToLdrRes(float ldrRaw) {

  float resV = (float)ldrRaw / MAX_ADC_READING * ADC_REF_VOLTAGE;
  float ldrV = ADC_REF_VOLTAGE - resV;
  float ldrRes = ldrV / resV * REF_RESISTANCE;
  return ldrRes;
}

float ldrResToLdrLux(float ldrResistance) {
  return LUX_CALC_SCALAR * (pow(ldrResistance, LUX_CALC_EXPONENT));
}

float evToShutterSpeed(float ev, float aperture) {
    float shutterSpeed = (aperture * aperture) / pow(2.0, ev);
    return 1/shutterSpeed;
}
/*
float shutterSpeed(float ev, float aperture_n, float iso_s) {
  // First, find EV100
  //float ev100 = ev - log2(iso_s / 100.0);
  float ev100 = ev - log(iso_s / 100.0) / log(2);
  //return log(x) / log(2.0);

  float t = (aperture_n * aperture_n) / pow(2.0, ev100);
  return t;
}*/

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

  //float lux;
  uint32_t ldrRaw = 0;

  // Get average value for conversion from LDR
  for (int x = 0; x < ADC_COUNT; x++) {
    delay(CAPTURE_DELAY);
    ldrRaw += analogRead(METER_PIN);
  }
  ldrRaw /= ADC_COUNT;

  // Convert the raw LDR reading to a resistor value for the LDR
  float ldrRes = ldrRawToLdrRes((float)ldrRaw);

  // Convert the LDR resistance value to lux
  float lux = ldrResToLdrLux(ldrRes);

  // Conver the lux value to EV
  float ev = luxToEv(lux);

  //Solve for \(t\): \(t=N^{2}/2^{EV}\) 

  float speed = evToShutterSpeed(ev, 2.8);

  display.fillRect(BX, BY, BW, BH, SSD1306_BLACK);
  display.setCursor(BX, BY);
  display.println(speed);

  //display.setCursor(BX, BY + 12);
  //display.println(a);

  display.fillRect(VX, VY, VW, VH, SSD1306_BLACK);
  display.setCursor(VX, VY);
  display.println(lux);

  Serial.print(ldrRes);
  Serial.print('\t');
  Serial.println(lux);

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