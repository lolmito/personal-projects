#include <Arduino.h>

float minValue = 100.0;     // Starting value
float maxValue = 500.0;     // Target maximum value
float currentValue = minValue; // rInitialize current value to minValue

void setup() {
  Serial.begin(9600);  // Start serial communication
  randomSeed(analogRead(0));  // Seed random for more randomness
}

void loop() {
  // If currentValue is below maxValue, increase it by a random small incrementr
  if (currentValue < maxValue) {
    float increment = random(5, 20) / 10.0;  // Random increment between 0.5 and 2.0
    currentValue += increment;

    // Cap currentValue at maxValue
    if (currentValue > maxValue) {
      currentValue = maxValue;
    }
  }
  // After reaching maxValue, allow slight random variation
  else {
    float variation = random(-5, 6) / 10.0;  // Small variation between -0.5 and +0.5
    currentValue = maxValue + variation;
  }

  Serial.println(currentValue);  // Print the current value to Serial Monitor

  delay(500);  // Delay for half a second before next increment
}
