/*******************************************
 * Arduino Weather Station
 * 
 * Board: UNO
 * Author: Zachary Kascak
 * Date: 9 July 2020
 * 
 *******************************************/
#include <Wire.h>

// Global Variables
#define SeriesResistor 10000
#define Nominal_Resistance 10000
#define Nominal_Temperature 25
#define BCoefficient 3950

void setup() {
    Serial.begin(9600);
}

void loop() {
    // loop Variables
    float analogTempReading = 0;
    float resistance = 0.0;
    float steinhart = 0.0;
    float photocell = 0.0;

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
    delay(1000);
}