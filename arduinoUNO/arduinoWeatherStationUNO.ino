/*******************************************
 * 
 * Arduino Weather Station
 * Board: UNO
 * Author: Zachary Kascak (@zkascak)
 * Date: 25 July 2020
 * 
 *******************************************/
#include <Wire.h>
#include <SparkFun_HIH4030.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>

// Global Variables
#define SeriesResistor 10000
#define Nominal_Resistance 10000
#define Nominal_Temperature 25
#define BCoefficient 3950
#define HIH4030_OUT A3
#define HIH4030_SUPPLY 5
#define VMA230_OUT A0
#define PHOTOCELL_OUT A1
byte mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
IPAddress ip=(192, 168, 1, 177);

// Library Variables
HIH4030 sensorSpecs(HIH4030_OUT, HIH4030_SUPPLY);

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

void setup() {
    Serial.begin(9600);
    Serial.println("Initialize Arduino");
    Serial.println("Initialize BMP180 Sensor I2C Connection");
    Serial.println();

    // Initialize BMP180 sensor
    if(!bmp.begin()) {
        // Initialization failure
        Serial.print("BMP180 failed to initialize");
        while(1);
    }
}

void loop() {
    // loop Variables
    float analogTempReading = 0;
    float resistance = 0.0;
    float steinhart = 0.0;
    float photocell = 0.0;
    float humiditySensor = 0.0;
    float bmpTemperature = 0.0;
    float sealevelPressure = 1019.8;

    // Collect Analog Temp Reading
    Serial.print("Analog Temp: ");
    analogTempReading = analogRead(VMA230_OUT);
    resistance = (1023 / analogTempReading) -1;
    resistance = SeriesResistor / resistance;
    steinhart = resistance / Nominal_Resistance;
    steinhart = log(steinhart);
    steinhart /= BCoefficient;
    steinhart += 1.0 / (Nominal_Temperature + 273.15);
    steinhart = 1.0 /steinhart;
    steinhart -= 273.15;
    Serial.print(steinhart);
    Serial.println(" C");

    // Collect data from the photocell
    photocell = analogRead(PHOTOCELL_OUT);
    Serial.print("Photocell Reading: ");
    Serial.println(photocell);

    // Collect data from humoidity sensor
    printHumidityData(sensorSpecs, steinhart);

    // Collect temperature and barometric pressure data from BMP180
    sensors_event_t event;
    bmp.getEvent(&event);

    // Display barometric pressure in hPa
    if (event.pressure) {
        Serial.println("Altitude Sensor:");
        Serial.print("\tBarometric Pressure: ");
        Serial.print(event.pressure);
        Serial.println(" hPa");
        bmp.getTemperature(&bmpTemperature);
        Serial.print("\tBMP180 Temperature: ");
        Serial.print(bmpTemperature);
        Serial.println(" C");
        Serial.print("Altitude: ");
        Serial.print(bmp.pressureToAltitude(sealevelPressure, event.pressure));
        Serial.println(" m");
    }
    else {
        Serial.println("BMP180 Error");
    }
    delay(1000);
}

void printHumidityData(HIH4030 sensor, float temperature){
    Serial.println("Humidity:");
    Serial.print("\tRH: ");
    Serial.print(sensor.getSensorRH());
    Serial.println("%");
    Serial.print("\tTH: ");
    Serial.print(sensor.getTrueRH(temperature));
    Serial.println("%");
    float relativeHumidity = 0.0;
    float dewPoint = 0.0;
    relativeHumidity = sensor.getTrueRH(temperature);
    dewPoint = temperature - ((100-relativeHumidity)/5);
    Serial.print("Dew Point: ");
    Serial.print(dewPoint);
    Serial.println("C");

}