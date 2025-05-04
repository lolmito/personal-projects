#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Create an instance of the ADS1115 ADC
Adafruit_ADS1115 ads;

float vIn;                                // measured voltage (3.3V = max. 16.5V, 5V = max 25V)
float vOut;
float voltageSensorVal;                   // value on pin A3 (0 - 1023)
const float factor = 5.128;               // reduction factor of the Voltage Sensor shield
const float vCC = 3.00;                   // Arduino input voltage (measurable by voltmeter)

void setup() {
  Serial.begin(9600);

    // Initialize I2C communication with the ADS1115
  Wire.begin(4, 5);  // SDA = GPIO 4 (D2), SCL = GPIO 5 (D1) for ESP8266

  // Initialize the ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }
  Serial.println("ADS1115 Initialized.");
}

void loop() {
  int16_t adc3 = ads.readADC_SingleEnded(3);
  float voltageSensorVal = adc3 * 0.1875 / 1000.0;  // read the current sensor value (0 - 1023) 
  Serial.println(adc3);
  Serial.println(voltageSensorVal);  
  vOut = voltageSensorVal * vCC;             // convert the value to the real voltage on the analog pin    
  Serial.println(vOut);                          

  Serial.print("Voltage = ");             
  Serial.print(vOut);
  Serial.println("V");

  delay(1000);
}
