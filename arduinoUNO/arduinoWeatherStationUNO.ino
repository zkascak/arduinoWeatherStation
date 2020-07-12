/*******************************************
 * Arduino Weather Station
 * 
 * Board: UNO
 * Author: Zachary Kascak
 * Date: 9 July 2020
 * 
 *******************************************/
#include <Wire.h>
#include <SparkFun_HIH4030.h>

// Global Variables
#define SeriesResistor 10000
#define Nominal_Resistance 10000
#define Nominal_Temperature 25
#define BCoefficient 3950
#define HIH4030_OUT A2
#define HIH4030_SUPPLY 5

// Library Variables
HIH4030 sensorSpecs(HIH4030_OUT, HIH4030_SUPPLY);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // loop Variables
    float analogTempReading = 0;
    float resistance = 0.0;
    float steinhart = 0.0;
    float photocell = 0.0;
    float humiditySensor = 0.0;

    // Collect Analog Temp Reading
    Serial.print("Analog Temp: ");
    analogTempReading = analogRead(A0);
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
    photocell = analogRead(A1);
    Serial.print("Photocell Reading: ");
    Serial.println(photocell);

    // Collect data from humoidity sensor
    printHumidityData(sensorSpecs, steinhart);

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