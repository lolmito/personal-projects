#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define RL_MQ136 10000  // 10kΩ load resistor
#define RL_MQ137 10000  // 10kΩ load resistor
#define VCC 5.0         // Sensor supply voltage

void setup() {
    Serial.begin(115200);
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115!");
        while (1);
    }
    ads.setGain(GAIN_TWOTHIRDS);  // ±6.144V range
}

void loop() {
    int16_t raw_MQ136 = ads.readADC_SingleEnded(0); 
    int16_t raw_MQ137 = ads.readADC_SingleEnded(1); 

    float voltage_MQ136 = raw_MQ136 * (6.144 / 32767.0);  // Convert ADC to voltage (VRL)
    float voltage_MQ137 = raw_MQ137 * (6.144 / 32767.0);  // Convert ADC to voltage (VRL)

    if (voltage_MQ136 <= 0.1 || voltage_MQ137 <= 0.1) {  // Check if sensor is connected properly
        Serial.println("⚠️ Sensor reading too low! Check wiring.");
    } else {
        float Rs_MQ136 = ((VCC - voltage_MQ136) / voltage_MQ136) * RL_MQ136;
        float Rs_MQ137 = ((VCC - voltage_MQ137) / voltage_MQ137) * RL_MQ137;

        float Ro_MQ136 = Rs_MQ136/3.6;
        float Ro_MQ137 = Rs_MQ137/3.6;

        Serial.print("raw value MQ136: ");
        Serial.println(raw_MQ136);
        Serial.print("raw value MQ137: ");
        Serial.println(raw_MQ137);

        Serial.print("RS MQ-136: ");
        Serial.println(Rs_MQ136, 3);
        Serial.print("RS MQ-137: ");
        Serial.println(Rs_MQ137, 3);

        Serial.print("Ro MQ-136: ");
        Serial.println(Ro_MQ136, 3);
        Serial.print("Ro MQ-137: ");
        Serial.println(Ro_MQ137, 3);
    }

    delay(5000);
}
