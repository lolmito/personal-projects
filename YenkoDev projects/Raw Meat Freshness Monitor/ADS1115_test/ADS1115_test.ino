 #include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads; // Create ADS1115 object

void setup() {
    Serial.begin(115200);
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115!");
        while (1);
    }
    ads.setGain(GAIN_TWOTHIRDS);  // ±6.144V range (ideal for 5V sensors)
}

void loop() {
    int16_t raw_A0 = ads.readADC_SingleEnded(0);
    int16_t raw_A1 = ads.readADC_SingleEnded(1);
    int16_t raw_A2 = ads.readADC_SingleEnded(2);
    int16_t raw_A3 = ads.readADC_SingleEnded(3);

    // Convert raw ADC value to voltage
    float voltage_A0 = raw_A0 * 0.1875 / 1000.0; // ADS1115 LSB (for ±6.144V range)
    float voltage_A1 = raw_A1 * 0.1875 / 1000.0;
    float voltage_A2 = raw_A2 * 0.1875 / 1000.0;
    float voltage_A3 = raw_A3 * 0.1875 / 1000.0;

    // Print voltage readings
    Serial.print("A0 Voltage: "); Serial.print(voltage_A0, 3); Serial.println("V");
    Serial.print("A1 Voltage: "); Serial.print(voltage_A1, 3); Serial.println("V");
    Serial.print("A2 Voltage: "); Serial.print(voltage_A2, 3); Serial.println("V");
    Serial.print("A3 Voltage: "); Serial.print(voltage_A3, 3); Serial.println("V");

    Serial.println("------------------------");
    delay(2000); // Wait 2 seconds before next reading
}
