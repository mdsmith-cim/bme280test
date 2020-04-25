/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

unsigned long delayTime;
#include <OneBitDisplay.h>


static uint8_t ucBackBuffer[1024];

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are the pin numbers for the M5Stack Atom Grove port I2C (reversed SDA/SCL for straight through wiring)
#define SDA_PIN -1
#define SCL_PIN -1
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let OneBitDisplay figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 1

// Change these if you're using a different OLED display
#define MY_OLED OLED_128x64
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

OBDISP obd;


void setup() {
    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned status;
    
    status = bme.begin(BME280_ADDRESS_ALTERNATE, &Wire);

    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    }

    delayTime = 1000;
    int rc;

    rc = obdI2CInit(&obd, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 800000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND)
      {
        char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D",(char *)"SH1106 @ 0x3C",(char *)"SH1106 @ 0x3D"};
        obdFill(&obd, 0, 1);
        obdWriteString(&obd, 0,0,0,msgs[rc], FONT_NORMAL, 0, 1);
        obdSetBackBuffer(&obd, ucBackBuffer);
        delay(2000);
  }

  
    obdFill(&obd, 0x0, 1);
    obdWriteString(&obd, 0,0,0,(char *)"Temp:", FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,0,1,(char *)"Pres:", FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,0,2,(char *)"Altd:", FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,0,3,(char *)"Humd:", FONT_NORMAL, 0, 1);

}


void loop() { 
    printValues();
    delay(delayTime);
}


void printValues() {
  
    float temp = bme.readTemperature();

    char result[8]; // Buffer big enough for 7-character float
    dtostrf(temp, 6, 2, result); // Leave room for too large numbers!
    obdWriteString(&obd, 0,5*8,0, result, FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,13*8,0, (char *)"*C", FONT_NORMAL, 0, 1);

    float pressure = bme.readPressure() / 100.0F;
    dtostrf(pressure, 6, 2, result); // Leave room for too large numbers!
    obdWriteString(&obd, 0,5*8,1, result, FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,13*8,1, (char *)"hPA", FONT_NORMAL, 0, 1);
    
    float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    dtostrf(alt, 6, 2, result); // Leave room for too large numbers!
    obdWriteString(&obd, 0,5*8,2, result, FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,13*8,2, (char *)"m", FONT_NORMAL, 0, 1);
    
    float humid = bme.readHumidity();
    dtostrf(humid, 6, 2, result); // Leave room for too large numbers!
    obdWriteString(&obd, 0,5*8,3, result, FONT_NORMAL, 0, 1);
    obdWriteString(&obd, 0,13*8,3, (char *)"%", FONT_NORMAL, 0, 1);
/
}
