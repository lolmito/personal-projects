#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <math.h>

Adafruit_ADS1115 ads;

// Constants from datasheets
#define A_H2S 70.0
#define B_H2S -1.2
#define A_NH3 116.602
#define B_NH3 -2.769

// Load Resistor Values
#define RL_MQ136 10000  // 10kΩ for MQ-136
#define RL_MQ137 47000  // 47kΩ for MQ-137

// Calibration values (Replace with your measured R₀ values)
#define R0_MQ136 367714 // Example value, replace with your own
#define R0_MQ137 141386  // Example value, replace with your own

#define VCC 5.0  // Sensor supply voltage

void setup() {
    Serial.begin(115200);
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115!");
        while (1);
    }
    ads.setGain(GAIN_TWOTHIRDS);
}

void loop() {
    int raw_MQ136 = ads.readADC_SingleEnded(0);
    int raw_MQ137 = ads.readADC_SingleEnded(1);
    float voltage_MQ136 = raw_MQ136 * 0.1875 / 1000.0;
    float voltage_MQ137 = raw_MQ137 * 0.1875 / 1000.0;

    const float Ro = 14248.0; // presumed sensor resistance in clean air
    const float slopeAlcohol = -0.552464627410705;
    const float scaleAlcohol = 10.2889924963885;

double ppm = 0;
ppm = 68.0551534226939 * pow (Rs/Ro, -1.80958209233455);

    // Print results
    Serial.print("MQ-136 (H2S): ");
    Serial.print(ppm_H2S, 2);
    Serial.println(" PPM");

    Serial.print("MQ-137 (NH3): ");
    Serial.print(ppm_NH3, 2);
    Serial.println(" PPM");

    Serial.println("-----------------------------");
    delay(2000);
}
