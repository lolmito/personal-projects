// ---------------WORKING PH SENSOR INDIVIDUAL TESTING AND CALIBRATION--------------------------
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
//NOTES: 2 Ground pins must be connected to ground
// Create an instance of the ADS1115 ADC
Adafruit_ADS1115 ads;
float voltage;
float pHValue;
float offset = 0;  // Adjust based on calibration
float sensorVoltageMax = 1;  // Use 1.0 if you are using a voltage divider to bring the sensor voltage within 0-1V
unsigned long int avgval;
float calibration_value = 15.82 + 5.72;
int buffer_arr[10], temp;
void setup() {
  Serial.begin(9600);
  
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
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = adc0;
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++){
    avgval += buffer_arr[i];
  }
  // Convert the analog reading to voltage (ESP8266 ADC reads from 0-1V or divided value)
  float voltage = adc0 * 0.1875 / 1000.0;  // Adjust sensorVoltageMax based on your voltage divider
  float voltage3 =  -5.70 * voltage;
  pHValue = -5.70 * voltage + calibration_value;
  // Convert the voltage to pH value (adjust the formula based on calibration) // Modify this formula based on your pH sensor calibration
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print("Voltage with 3.5 ");
  Serial.print(voltage3, 4);
  Serial.print(" V, pH: ");
  Serial.println(pHValue);

  delay(1000); // Wait for 1 second
}
//will have to put code that converts voltage to mm



