//CAN BE USED IN CHECKING WATER LEVEL SENSOR

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Initialize I2C (SDA = GPIO4 (D2), SCL = GPIO5 (D1) on ESP8266)

  Serial.println("Scanning for ADS1115...");

  if (!ads.begin()) {
    Serial.println("ADS1115 not found. Please check the wiring and connection.");
  } else {
    Serial.println("ADS1115 connected successfully!");
  }
}

void loop() {
  for (int channel = 0; channel < 4; channel++) {
    int16_t adcReading = ads.readADC_SingleEnded(channel); // Read from each channel
    float voltage = adcReading * 0.1875 / 1000.0;           // Convert to voltage, change accordingly (can be 0.125)

    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" | ADC Reading: ");
    Serial.print(adcReading);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 4);
    Serial.println(" V");
  }
  
  delay(1000);  // Wait 1 second between reads
}
