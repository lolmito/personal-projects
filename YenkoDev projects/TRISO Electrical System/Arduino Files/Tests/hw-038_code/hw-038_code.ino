#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Create an instance of the ADS1115 ADC
Adafruit_ADS1115 ads;
float voltage;
float pHValue;
float offset = 0.00;  // Adjust based on calibration
float sensorVoltageMax = 3.3;  // Use 1.0 if you are using a voltage divider to bring the sensor voltage within 0-1V

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C communication with the ADS1115
  Wire.begin(4, 5);  // SDA = D2 (GPIO 4), SCL = D1 (GPIO 5)

  // Initialize the ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }
  Serial.println("ADS1115 Initialized.");
}

void loop() {
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);

  // Convert the analog reading to voltage (ESP8266 ADC reads from 0-1V or divided value)
  float voltage = adc0 * (sensorVoltageMax / 1023.0);  // Adjust sensorVoltageMax based on your voltage divider

  // Convert the voltage to pH value (adjust the formula based on calibration)
  pHValue = 3.5 * voltage + offset;  // Modify this formula based on your pH sensor calibration

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, pH: ");
  Serial.println(pHValue);

  delay(1000); // Wait for 1 second
}
//will have to put code that converts voltage to mm



