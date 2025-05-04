
//Dictate Dependencies/Libraries

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//Initialize Variables

//OneWire
#define ONE_WIRE_BUS 15//For temperature sensor
OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire device
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to DallasTemperature library

//Wi-Fi Credentials
WiFiClient client;// Initialize Wi-Fi client
const char* ssid = "Google";
const char* password = "trailjovic";

//Thingspeak credentials
unsigned long channelID = 2699314; // Your channel ID
const char* writeAPIKey = "8OUGJ0P9QUSINKIF"; // Your Write API Key

//Relays
#define RELAY_PIN 1     // Digital pin where the relay is connected (TX - Grid)
#define RELAY_PIN 2     // Digital pin where the relay is connected (D4 - Hydro)
#define RELAY_PIN 3     // Digital pin where the relay is connected (RX - Solar)
#define RELAY_PIN 0     // Digital pin where the relay is connected (D3 - Valve )

//ADS1115 initialization (Only activate if ADS1115 is present)
//Adafruit_ADS1115 ads;

/* Pins to control multiplexer (74HC4051)
const int s0 = D5;
const int s1 = D6;
const int s2 = D7;
*/

/* Create an instance of the ADS1115 ADC
Adafruit_ADS1115 ads;
WiFiClient client;
float voltage;
float pHValue;
float offset = 0.00;  // Adjust based on calibration
float sensorVoltageMax = 3.3;  // Use 1.0 if you are using a voltage divider to bring the sensor voltage with
*/

/*
// Pin definitions
const int voltagePin = A0;  // Analog pin where the voltage sensor module is connected
const int relayPin = 8;     // Digital pin where the relay is connected
*/

void setup() {
  Serial.begin(9600);

  //Setting Wi-Fi and ThingSpeak
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  ThingSpeak.begin(client);

  // Initialize I2C communication with the ADS1115
  Wire.begin(4, 5);  // SDA = D2 (GPIO 4), SCL = D1 (GPIO 5)
  
  //Only activate if ADS1115 is present
  /*
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }
  Serial.println("ADS1115 Initialized.");
  */

  // Start the DS18B20 sensor
  sensors.begin();
  Serial.println("DS18B20 Initialized.");
}

  /* Initialize MUX control pins
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  


*/

//Temperature Function
float DS18B20_Temperature(){
  sensors.requestTemperatures();  // Request temperature from DS18B20
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.println("Temperature: " + String(temperatureC) + " °C");
  return temperatureC;
}

void loop() {
  //Get temperature statistic
  float thingTemp = DS18B20_Temperature();
  if (thingTemp != -127.00){
      ThingSpeak.setField(1, thingTemp);// Set field 1 with the actual temperature value
  }
  int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);// Write the fields to ThingSpeak
  // Check if the write was successful
  if (responseCode == 200) {
      Serial.println("Data sent successfully:");
      Serial.print("Temperature: " + String(thingTemp)+ " °C");
    } 
  else {
    Serial.print("Error sending data: ");
    Serial.println(responseCode);
    }

    delay(16000); // Wait 20 seconds before sending the next data
}



//Void loop test for thingspeak
/*
String statusMessage1 = "Temperature is above 5°C"; // Message for above threshold
String statusMessage2 = "Temperature is below 5°C";  // Message for below threshold
const int tempThreshold = 50;
void loop (){
    // Simulated temperature reading (replace with your actual sensor reading)
    float temp = random(0, 100); // Random temperature between 0 and 9
    // Check temperature and set status message accordingly
    if (temp < tempThreshold) {
        ThingSpeak.setField(2, statusMessage2);  // Set field 2 with status message 2
    } else {
        ThingSpeak.setField(2, statusMessage1);  // Set field 2 with status message 1   
    }


    // Set field 1 with the actual temperature value
    ThingSpeak.setField(1, temp); // Set field 1 with temperature value

    // Write the fields to ThingSpeak
    int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);
    
    // Check if the write was successful
    if (responseCode == 200) {
        Serial.println("Data sent successfully:");
        Serial.print("Temperature: ");
        Serial.println(temp);
        Serial.print("Status Message: ");
        Serial.println(temp < tempThreshold ? statusMessage2 : statusMessage1);
    } else {
        Serial.print("Error sending data: ");
        Serial.println(responseCode);
    }

    delay(10000); // Wait 20 seconds before sending the next data
}

*/

