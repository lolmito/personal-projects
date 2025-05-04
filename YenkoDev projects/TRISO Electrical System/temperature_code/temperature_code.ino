#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 4 (D2) on ESP8266
#define ONE_WIRE_BUS 15  // GPIO 4 (D2)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);   // Start serial communication at 115200 baud rate
  sensors.begin();        // Initialize the DallasTemperature library
}

void loop() {
  sensors.requestTemperatures();       // Send the command to get temperatures
  float temperatureC = sensors.getTempCByIndex(0);  // Get the temperature in Celsius from the first sensor
  
  // Print the temperature in one line
  Serial.println("Temperature: " + String(temperatureC) + " °C");

  delay(1000);  // Wait 1 second before taking the next reading
}
