// Pin definitions
//const int voltagePin = A0;  // Analog pin where the voltage sensor module is connected
#define RELAY_PIN 2     // Digital pin where the relay is connected (D4)

// Constants
//const float voltageThreshold = 11.0;  // Voltage threshold to control the relay
//const float sensorMaxVoltage = 12.0;  // Maximum input voltage the sensor module can measure
const float referenceVoltage = 5.0;   // Arduino reference voltage (typically 5V for 5V boards)
//const int analogResolution = 1023;    // Analog resolution for 10-bit ADC (0-1023)

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set the relay pin as output
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);  // Ensure the relay is initially off
}

void loop() {
  // Read the analog input from the voltage sensor module
  //int sensorValue = analogRead(voltagePin);
  
  // Map the sensor output (0-5V) to the actual voltage the module is sensing (e.g., 0-25V)
  //float measuredVoltage = (sensorValue * sensorMaxVoltage) / analogResolution;

  // Print the measured voltage for debugging
  //Serial.print("Measured Voltage: ");
  //Serial.println(measuredVoltage);
  int measuredVoltage = 10;
  int randNumber=random(0, 20);
  Serial.println(randNumber);

  // Control the relay based on the voltage threshold
  if (measuredVoltage <= voltageThreshold) { //If less than threshold, then normally open
    // Turn off the relay if the voltage is below the threshold
    digitalWrite(RELAY_PIN, LOW); //Right is normally closed, left is normally opened
    Serial.println("Relay ON");
  } 
  if (voltageThreshold < measuredVoltage && measuredVoltage < sensorMaxVoltage)) {
    

  }
  else{
    // Turn on the relay if the voltage is above the threshold
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relay OFF");
  }
  
  // Small delay before next measurement
  delay(2000);
}
