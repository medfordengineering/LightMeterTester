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

#define VX BX + 60
#define VY 0
#define VW 40
#define VH 20

#define EX 4
#define EY 21
#define EW 30
#define EH 10

#define METER 5

// These constants, define values needed for the LDR readings and ADC
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0
#define REF_RESISTANCE            9930  // measure this for best results
//#define LUX_CALC_SCALAR           12518931
//#define LUX_CALC_EXPONENT         -1.405

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {

  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float ldrLux;
  uint32_t ldrRaw = 0;

  for (int x = 0; x < 20; x++)
    ldrRaw += analogRead(METER);

  ldrRaw /= 20;

 // RESISTOR VOLTAGE_CONVERSION
  // Convert the raw digital data back to the voltage that was measured on the analog pin
  resistorVoltage = (float)ldrRaw / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // voltage across the LDR is the 5V supply minus the 5k resistor voltage
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  
  // LDR_RESISTANCE_CONVERSION
  // resistance that the LDR would have for that voltage  
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  
  //float volt = a * max_v / 1023;
  //float lux = (volt * (-4000)) + 18000;

 //y= 3.36899\times10^{9}\right)\cdot x^{-1.56617}

  display.fillRect(BX, BY, BW, BH, SSD1306_BLACK);
  display.setCursor(BX, BY);
  display.println(ldrResistance);

  //display.setCursor(BX, BY + 12);
  //display.println(a);

  //display.fillRect(VX, VY, VW, VH, SSD1306_BLACK);
  //display.setCursor(VX, VY);
  //display.println(lux);

  //display.setCursor(VX, VY+ 12);
  //display.println(a);
  /*
  display.fillRect(SX + 1, SY + 1, SW - 2, SH - 2, SSD1306_BLACK);
  display.setCursor(SX + 3, SY + 2);
  //display.println(speeds[count]);
  display.println(ev_v);

  display.setTextSize(1);

  display.fillRect(EX, EY, EW, EH, SSD1306_BLACK);
  display.setCursor(EX, EY);
  //display.println(ev);

  // Serial.print("EV: ");
  //Serial.print(ev);

  //Serial.print(" AD: ");
  //Serial.print(ad);

  Serial.print(" LuxV: ");
  //Serial.print(luxVM);


  Serial.print(" LuxB: ");
  Serial.println(lux);

*/
  display.display();
}