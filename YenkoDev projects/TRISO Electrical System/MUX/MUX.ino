#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Create an instance of the ADS1115 ADC
Adafruit_ADS1115 ads;

// Define digital pin numbers using #define
#define S0 D5  // Connect to S0 of the 74HC4051
#define S1 D6  // Connect to S1 of the 74HC4051
#define S2 D7  // Connect to S2 of the 74HC4051

// Function to select a channel on the 74HC4051 (0-7)
void selectChannel(int channel) {
  digitalWrite(S0, channel & 1);         // Set S0
  digitalWrite(S1, (channel >> 1) & 1);  // Set S1
  digitalWrite(S2, (channel >> 2) & 1);  // Set S2
}

void setup() {
  Serial.begin(115200);

  // Initialize the select pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  // Start I2C communication
  Wire.begin(D2, D1);  // SDA = D2 (GPIO 4), SCL = D1 (GPIO 5)

  // Initialize the ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }
  Serial.println("ADS1115 Initialized.");
}

void loop() {
  for (int channel = 0; channel < 8; channel++) {
    selectChannel(channel);  // Select the channel on the 74HC4051
    delay(10);               // Small delay for the multiplexer to stabilize

    // Read from ADS1115 channel 0 (where the multiplexer Z pin is connected)
    int16_t adcReading = ads.readADC_SingleEnded(0);

    // Convert the reading to voltage (assuming default gain of ±6.144V)
    float voltage = adcReading * 0.1875 / 1000.0;

    // Print the channel number and corresponding voltage
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.print(voltage);
    Serial.println(" V");
  }

  delay(1000);  // Wait 1 second before repeating the loop
}
