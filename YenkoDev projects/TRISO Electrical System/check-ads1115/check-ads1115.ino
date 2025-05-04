#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Create an instance of the ADS1115 ADC
Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C communication with the ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }
  
  Serial.println("ADS1115 Initialized.");
}

void loop() {
  // Read analog input from A0 of the ADS1115
  int16_t adcValue = ads.readADC_SingleEnded(3);  // Reads from A0
  
  // Convert the ADC value to voltage (ADS1115 default gain is +/- 6.144V)
  float voltage = adcValue * 0.1875 / 1000.0;  // 0.1875mV per bit

  // Print the voltage to the Serial Monitor
  Serial.print("Voltage on A3: ");
  Serial.print(voltage, 6);  // Print with 6 decimal places
  Serial.println(" V");

  delay(1000);  // Wait 1 second between readings
}
