// Current Sensor code and MUX checker

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#define MUX_S0 12  // GPIO12 for S0 D6
#define MUX_S1 13  // GPIO13 for S1 D7
#define MUX_S2 14  // GPIO14 for S2 D5
#define VOLTAGE_PIN A0  // Analog pin connected to the MUX output

// Create an instance of the ADS1115 ADC
const int NUM_CHANNELS = 8; 

// ACS712 Parameters
const float ACS712_sensitivity = 0.185;  // Sensitivity for 5A module (0.185 V/A for ACS712-05B)
const float referenceVoltage = 2.5;      // Reference voltage is 2.5V at 0A for ACS712
const float vCCSensor = 3.3;
// Function to select a channel on the MUX
void selectChannel(int channel) {
  digitalWrite(MUX_S0, channel & 0x01); // Set S0
  digitalWrite(MUX_S1, (channel >> 1) & 0x01); // Set S1
  digitalWrite(MUX_S2, (channel >> 2) & 0x01); // Set S2
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  
}

void loop() {
  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    selectChannel(channel);
    delay(10); // Allow the MUX to settle
    if (channel == 0) {
      // Convert the raw ADC value to voltage
      int adcValue = analogRead(VOLTAGE_PIN);
      float voltage = adcValue * (1.0 / 1023.0);  // 0.125 mV per bit for GAIN_ONE (4.096V range)
      float vOut = voltage * vCCSensor;
      Serial.println("voltageinMUX: ");
      Serial.print(voltage, 4);
      Serial.println("voltageOut: ");
      Serial.print(vOut, 4);
      // Calculate the current based on the ACS712 sensitivity and reference voltage
      float current = (vOut - referenceVoltage) / ACS712_sensitivity;
      // Print the current to the Serial Monitor
      if (current > 0){
      Serial.print("Current: ");
      Serial.print(current, 4);  // Print with 4 decimal places
      Serial.println(" A");
      }
      else{
      Serial.print("Current: ");
      Serial.println("0A");
      }
    }
    else{
      Serial.print("Channel ");
      Serial.print(channel);
      delay(500); // Wait before switching to the next channel

    }
  // Read analog input from the ACS712 connected to A0 of ADS1115
  }

  // Delay for 1 second before taking another reading
  delay(1000);
}
