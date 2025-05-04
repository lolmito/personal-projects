//Dictate Dependencies/Libraries

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//Initialize Variables

//Wi-Fi Credentials
WiFiClient client;// Initialize Wi-Fi client
const char* ssid = "Google";
const char* password = "trailjovic";

//Thingspeak credentials
unsigned long channelID = 2699314; // Your channel ID
const char* writeAPIKey = "8OUGJ0P9QUSINKIF"; // Your Write API Key

//Relays
#define HYDRO_PIN 2     // GPIO 2  - Hydro relay (indiv tested)
#define SOLAR_PIN 15     // GPIO 15 - Solar relay
#define GRID_PIN 0      // GPIO 0 - Grid relay
#define VALVE_1 13      // GPIO 13 - Valve 1
#define VALVE_2 14      // GPIO 14 - Valve 2

// Define digital pin numbers for the multiplexer
#define S0 12  // Connect to S0 of the 74HC4051 (GPIO 12 - D6) / Tempearture Sensor
#define S1 13  // Connect to S1 of the 74HC4051 (GPIO 13 - D7) / PH Sensor
#define S2 14  // Connect to S2 of the 74HC4051 (GPIO 14 - D5)

// Analog pin
#define ANALOG_PIN A0 //Analog pin in ESP8266 that is used by MUX

//OneWire Temperature Sensor
//#define ONE_WIRE_BUS 16              // GPIO 16 (D0)
OneWire* oneWire = nullptr;          // Pointer to OneWire object
DallasTemperature* sensors = nullptr; // Pointer to DallasTemperature object

//PH Sensor
float offset = 0.00;

//General Variables
bool initialStart = true; //for initial startup
bool testFlag = true; // For testing purposes
//const float vCCSensor1 = 5;  // Voltage for Relays and other sensors
float voltage;        // Placeholder for voltage values
int muxValue;         // Placeholder for mux raw values
int16_t adcReading;    // Placeholder for adc raw values

// For TRISO variables:
float solarVoltage;
float solarCurrent;
float hydroVoltage;
float hydroCurrent;
float gridCurrent;
float gridVoltage = 12;
//float* maxVoltage = &gridVoltage; //comparison is grid voltage since it is always constant
float batteryVoltage;

//Voltage and Current (ACS712) Sensors
const float ACS712_sensitivity = 0.185;  // Sensitivity for 5A module (0.185 V/A for ACS712-05B)
const float referenceVoltage = 2.5;      // Reference voltage is 2.5V at 0A for ACS712
const float vCCSensor = 3.3;             //Voltage used for all the current and voltage sensors

//Water level voltage reference values:
//NOTE: at no water level sensor registers a value of abt 1V even if it isnt being cleaned. After 5 seconds it returns to ~0.20V. No need to consider 3V or 5V input
const int waterLevel1 = 2.8; //3/4 of the sensor
const int waterLevel2 = 2.8; ///3/4 of the sensor
const int waterLevel3 = 2.8; //3/4 of the sensor

//thingSpeak data
float thingTemp;
float thingPH;
float thingSolar;
float thingGrid;
float thingHydro;

//ADS1115 initialization (Only activate if ADS1115 is present)
Adafruit_ADS1115 ads;
/*
float pHValue;
  // Adjust based on calibration
float sensorVoltageMax = 3.3;  // Use 1.0 if you are using a voltage divider to bring the sensor voltage with
*/

void ADScheckError (){

  //Error Checking for ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }
  Serial.println("ADS1115 Initialized.");
}

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
  ADScheckError(); //Check for errors in initialization
  ads.setGain(GAIN_ONE);// 0.125 mV per bit for GAIN_ONE (4.096V range)
/*
  //Initialize pins of the MUX
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
*/
  // Initialize all relay pins as OUTPUT and turn them off (LOW)
  pinMode(HYDRO_PIN, OUTPUT);
  pinMode(SOLAR_PIN, OUTPUT);
  pinMode(GRID_PIN, OUTPUT);
  pinMode(VALVE_1, OUTPUT);
  pinMode(VALVE_2, OUTPUT);
  
}

// ------------------------Temperature Sensor Classes------------------------

//initialize Temperature sensor
void initializeTemperatureSensor() {
  oneWire = new OneWire(S0);
  sensors = new DallasTemperature(oneWire);
  sensors->begin();
}
//Temperature Function
float DS18B20_Temperature(){
  sensors->requestTemperatures();  // Request temperature from DS18B20
  float temperatureC = sensors->getTempCByIndex(0);
  Serial.println("Temperature: " + String(temperatureC) + " °C");
  return temperatureC;
}

//deinitialize Temperature sensor
void deinitializeTemperatureSensor() {
  if (sensors) {
    delete sensors;  // Delete DallasTemperature object
    sensors = nullptr;
  }
  if (oneWire) {
    delete oneWire;  // Delete OneWire object
    oneWire = nullptr;
  }
}
// ------------------------Multiplexer Classes------------------------

//Initialize MUX
void initializeMux() {
  pinMode(S0, OUTPUT);  // Set GPIO 12 as output for MUX control
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
}

// Function to select a channel on the 74HC4051 (0-7)
void selectChannel(int channel) {
  digitalWrite(S0, channel & 1);         // Set S0
  digitalWrite(S1, (channel >> 1) & 1);  // Set S1
  digitalWrite(S2, (channel >> 2) & 1);  // Set S2
}

//Deinitalize the MUX
void deinitializeMux() {
  pinMode(S0, INPUT);  // Reset GPIO 12 to INPUT for reuse (Temperature)
  pinMode(S1, INPUT);  // Reset GPIO 13 to INPUT for reuse (PH Sensor)
  pinMode(S2, INPUT);
}

// ------------------------PH Sensor Classes------------------------

//PH Sensor Function
void PHSensor(float voltage){
  thingPH = 3.5 * voltage + offset;
}

// ------------------------TRISO Sensors and Relays ----------------------------

//Grid Current Sensor
void GridCurrentSensor(float voltage){
 float vGOut = voltage * vCCSensor;
 float current = (vGOut - referenceVoltage) / ACS712_sensitivity;
 gridCurrent = abs(current); //absolute value of current sent
 Serial.print("Grid Current: ");
 Serial.print(gridCurrent, 4);  // Print with 4 decimal places
 Serial.println(" A");
}

//Hydro Voltage Sensor
void HydroVoltageSensor(float voltage){
  hydroVoltage = voltage * vCCSensor;
  Serial.print("Hydro Voltage: ");
  Serial.print(hydroVoltage, 4);  // Print with 4 decimal places
  Serial.println(" V");
}

//Hydro Current Sensor
void HydroCurrentSensor(float voltage){
 float vOut = voltage * vCCSensor;
 float current = (vOut - referenceVoltage) / ACS712_sensitivity;
 hydroCurrent = abs(current);
 Serial.print("Hydro Current: ");
 Serial.print(hydroCurrent, 4);  // Print with 4 decimal places
 Serial.println(" A");
}

//Solar Voltage Sensor
void SolarVoltageSensor(float voltage){
  solarVoltage = voltage * vCCSensor;
  Serial.print("Solar Voltage: ");
  Serial.print(solarVoltage, 4);  // Print with 4 decimal places
  Serial.println(" V");
}

//Solar Current Sensor
void SolarCurrentSensor(float voltage){
  float vOut = voltage * vCCSensor;
  float current = (vOut - referenceVoltage) / ACS712_sensitivity;
  solarCurrent = abs(current);
  Serial.print("Solar Current: ");
  Serial.print(solarCurrent, 4);  // Print with 4 decimal places
  Serial.println(" A");
}

//Battery Voltage Sensor

void BatteryVoltageSensor(float voltage){
  batteryVoltage = voltage * vCCSensor;
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 4);  // Print with 4 decimal places
  Serial.println(" V");
}

// TRISO Reset Relays
void resetTRISORelays() {
  // Set all relays to LOW (turn off)
  Serial.println("All TRISO Relays are off");
  digitalWrite(HYDRO_PIN, HIGH);
  digitalWrite(SOLAR_PIN, HIGH);
  digitalWrite(GRID_PIN, HIGH);
}

void resetValveRelays() {
  Serial.println("All valve Relays are off");
  // Set all relays to LOW (turn off)
  digitalWrite(VALVE_1, HIGH);
  digitalWrite(VALVE_2, HIGH);
}

// TRISO Relays Switching
void changeRelayTo(const char* source) {
  resetTRISORelays();
  if (strcmp(source, "Solar") == 0) {
    digitalWrite(SOLAR_PIN, LOW);
    Serial.println("Switching to Solar power");
  } else if (strcmp(source, "Hydro") == 0) {
    digitalWrite(HYDRO_PIN, LOW);
    Serial.println("Switching to Hydro power");
  } else if (strcmp(source, "Grid") == 0) {
    digitalWrite(GRID_PIN, LOW);
    Serial.println("Switching to Grid power");
  }else if (strcmp(source, "Off") == 0){
    resetTRISORelays();
  }
}

// Testing
void forTesting(){
  //MUX Testing
  Serial.println("MUX Testing: ");
  for (int channel = 0; channel < 8; channel++) { //Check sources and pick the most appropriate source:
  selectChannel(channel);  // Select the channel on the 74HC4051
  delay(10);               // Small delay for the multiplexer to stabilize
  muxValue = analogRead(ANALOG_PIN);
  voltage = muxValue * (1.0 / 1023.0);  // 0.125 mV per bit for GAIN_ONE (4.096V range)
  switch (channel) {
    case 0: //Grid
    Serial.println("------------ GRID CURRENT SENSOR ------------ ");
    Serial.print("Voltage Y0= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    case 1:  // Hydro Voltage
    Serial.println("------------ HYDRO VOLTAGE SENSOR ------------ ");
    Serial.print("Voltage A1= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;      
    case 2:
    Serial.println("------------ HYDRO CURRENT SENSOR ------------ ");
    Serial.print("Voltage Y2= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    case 3:
    Serial.println("------------ SOLAR VOLTAGE SENSOR ------------ ");
    Serial.print("Voltage Y3= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    case 4:
    Serial.println("------------ SOLAR CURRENT SENSOR ------------ ");
    Serial.print("Voltage Y4= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    case 5:
    Serial.println("------------ BATTERY VOLTAGE SENSOR ------------ ");
    Serial.print("Voltage Y5= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    case 6:
    Serial.println("------------ ABSENT ------------ ");
    Serial.print("Voltage Y6= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    case 7:
    Serial.println("------------ ABSENT ------------ ");
    Serial.print("Voltage Y7= ");             
    Serial.print(voltage);
    Serial.println("V");
    Serial.print("Voltage with Vcc= ");             
    Serial.print(voltage * vCCSensor);
    Serial.println("V");
    break;
    }
    delay(5000);
    }
    Serial.println("MUX Testing - DONE ");
    deinitializeMux();
    Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
    Serial.println(" ");
    
  //ADS-1115 Test
  Serial.println("ADS-1115 Testing: ");
  for (int channel = 0; channel < 4; channel++) {
  adcReading = ads.readADC_SingleEnded(channel); // Read from each channel
  voltage = adcReading * 0.125 / 1000.0;           // Given this is GAIN ONE we use 0.125 (if default +-2.048 use 0.1875)
  switch (channel) {
    case 0:
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" | Raw ADC Reading: ");
    Serial.print(adcReading);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 4);
    Serial.println(" V");
    Serial.println("=========PH SENSOR (PH-4502C) TESTING: =========");
    PHSensor(voltage);
    Serial.println("pH: ");
    Serial.print(thingPH);
    break;
    case 1:
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" | Raw ADC Reading: ");
    Serial.print(adcReading);
    Serial.print(" | Water Level 1 Voltage: ");
    Serial.print(voltage, 4);
    Serial.println(" V");
    break;
    case 2:
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" | Raw ADC Reading: ");
    Serial.print(adcReading);
    Serial.print(" | Water Level 2 Voltage: ");
    Serial.print(voltage, 4);
    Serial.println(" V");
    break;
    case 3:
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(" | Raw ADC Reading: ");
    Serial.print(adcReading);
    Serial.print(" | Water Level 3 Voltage: ");
    Serial.print(voltage, 4);
    Serial.println(" V");
    break;
  }
  delay(5000);
  }
  Serial.println("ADS-1115 Testing - DONE ");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println(" ");

  //Relay Test:
  //TRISO
  Serial.print("Testing TRISO Relays");
  Serial.println("Turning OFF");
  changeRelayTo("Off");
  delay(5000);
  Serial.println("Turning OFF - DONE");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("Turning ON Grid");
  changeRelayTo("Grid");
  delay(5000);
  Serial.println("Turning ON Grid - DONE");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("Turning ON Hydro");
  changeRelayTo("Hydro");
  delay(5000);
  Serial.println("Turning ON Hydro - DONE");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("Turning ON Solar");
  changeRelayTo("Solar"); 
  delay(5000);
  Serial.println("Turning ON Solar - DONE");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("TRISO TEST DONE - TURNING OFF RELAYS");
  changeRelayTo("Off");
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println(" ");

  //Valves
  Serial.print("Testing Valve Relays");
  Serial.println("Turning OFF Relays");
  digitalWrite(VALVE_1, HIGH);
  digitalWrite(VALVE_2, HIGH);
  delay(5000);
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("Turning ON Valve 1");
  digitalWrite(VALVE_1, LOW);
  delay(5000);
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("Turning ON Valve 2");
  digitalWrite(VALVE_2, LOW);
  delay(5000);
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("VALVE TEST DONE - TURNING OFF");
  digitalWrite(VALVE_1, HIGH);
  digitalWrite(VALVE_2, HIGH);
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println(" ");

  //Sensor Testing
  Serial.println("TEMPERATURE SENSOR (DS18B20) TESTING: ");
  initializeTemperatureSensor();
  float thingTemp = DS18B20_Temperature();
  Serial.println("Temperature: " + String(thingTemp) + " °C");
  deinitializeTemperatureSensor();
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println("VALVE TEST DONE - REPEATING TESTING in 3 SECS"); 
  Serial.println("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
  Serial.println(" ");
  delay(3000); 

}

void loop() {
  //During startup
  if(initialStart == true){
    Serial.println("Initial Startup....");
    // Reset all valves and deinitialize all modules
    resetValveRelays();
    resetTRISORelays();
    deinitializeTemperatureSensor();
    deinitializeMux();
    Serial.print("Done");
    initialStart = false;
  }
  //TRISO Section
  initializeMux(); //initialize the MUX first
  while (testFlag == true){
    forTesting();
    initializeMux(); //re-initializing MUX
  }
  for (int channel = 0; channel < 8; channel++) { //Check sources and pick the most appropriate source:
    selectChannel(channel);  // Select the channel on the 74HC4051
    delay(10);               // Small delay for the multiplexer to stabilize
    muxValue = analogRead(ANALOG_PIN);
    voltage = muxValue * (1.0 / 1023.0);        
    switch (channel) {
    case 0:
    GridCurrentSensor(voltage);
    break;
    case 1:  // Hydro Voltage
    HydroVoltageSensor(voltage);
    break;
    case 2:
    HydroCurrentSensor(voltage);
    break;
    case 3:
    SolarVoltageSensor(voltage);
    break;
    case 4:
    SolarCurrentSensor(voltage);
    break;
    case 5:
    BatteryVoltageSensor(voltage);
    break;
  }
  delay(1000);
  }
  deinitializeMux(); //deinitialize the MUX after use (inputs for PH and temperature sensor)

  // Simplified relay logic with a single function for changing relay
  if (batteryVoltage == 12){
    changeRelayTo("Off"); // If battery voltage is 12V (can be changed) turn off all relays
  }
  else if (hydroVoltage < 11 && solarVoltage < 11) {
    changeRelayTo("Grid");    // Both are below 11V, use Grid
  } else if (solarVoltage >= 11 && hydroVoltage >= 11) {
    // Both are within range, choose the one with the higher voltage
    changeRelayTo((solarVoltage >= hydroVoltage) ? "Solar" : "Hydro");
  } else if (solarVoltage >= 11) {
    changeRelayTo("Solar");  // Solar is within range, hydro is not
  } else if (hydroVoltage >= 11) {
    changeRelayTo("Hydro");  // Hydro is within range, solar is not
  }

  //Power Calculations (Will have to discuss tomorrow)
  thingSolar = solarVoltage * solarCurrent;
  Serial.print("Solar Power: ");
  Serial.print(thingSolar,4);
  Serial.print("W");
  thingHydro = hydroVoltage * hydroCurrent;
  Serial.print("Hydro Power: ");
  Serial.print(thingHydro,4);
  Serial.print("W");
  thingGrid= gridVoltage * gridCurrent;
  Serial.print("Grid Power: ");
  Serial.print(thingGrid,4);
  Serial.print("W");

  //Send latest statistics to thingspeak
  ThingSpeak.setField(2, thingSolar); //Solar power
  ThingSpeak.setField(3, thingHydro); //Hydro power
  ThingSpeak.setField(4, thingGrid); //grid power
  ThingSpeak.setField(5, batteryVoltage); //Battery Voltage
  //ADS-1115 
  for (int channel = 0; channel < 4; channel++) {
    adcReading = ads.readADC_SingleEnded(channel);// Read from each channel
    int ADSvoltage = adcReading * 0.125 / 1000.0;// Given this is GAIN ONE we use 0.125 (if default +-2.048 use 0.1875)
    Serial.println("Raw ADC Reading: ");
    Serial.print(adcReading);
    switch (channel) {
      case 0:
      PHSensor(ADSvoltage);
      Serial.println("pH Level: ");
      Serial.println(thingPH);
      while(thingPH >= 9){ // not yet tested individually
        digitalWrite(VALVE_1, LOW);
        delay(2000); //WATER RELEASE DELAY VALVE 1: 2 seconds (can be changed)
        digitalWrite(VALVE_1, HIGH);
        delay(3000); //Wait for 3 seconds
        }
      break;
      case 1:
      Serial.println("Water Level 1 Voltage: ");
      Serial.print(ADSvoltage, 4);
      Serial.println(" V");
      while(voltage >= waterLevel1){ // not yet tested individually
        digitalWrite(VALVE_1, LOW);
        delay(2000); //WATER RELEASE DELAY VALVE 1: 2 seconds (can be changed)
        digitalWrite(VALVE_1, HIGH);
        delay(2000); //Wait for 3 seconds
      }
      break;
      case 2:
      Serial.println(" Water Level 2 Voltage: ");
      Serial.print(ADSvoltage, 4);
      Serial.println(" V");
      while(voltage >= waterLevel2){ // not yet tested individually
        digitalWrite(VALVE_2, LOW);
        delay(2000); //WATER RELEASE DELAY VALVE 1: 2 seconds (can be changed)
        digitalWrite(VALVE_2, HIGH);
        delay(2000); //Wait for 3 seconds
      }
      break;
      case 3:
      Serial.println("Water Level 3 Voltage: ");
      Serial.print(ADSvoltage, 4);
      Serial.println(" V");
      while(voltage >= waterLevel3){ // not yet tested individually
        digitalWrite(VALVE_1, LOW); //TOCHECK:will have to check which valve to close or open
        delay(2000); //WATER RELEASE DELAY VALVE 1: 2 seconds (can be changed)
        digitalWrite(VALVE_1, HIGH); //TOCHECK:will have to check which valve to close or open
        delay(2000); //Wait for 3 seconds
      }
      break;
    }
    delay(1000);
  }
  // Temperature Sensor
  //Get temperature statistic
  initializeTemperatureSensor();
  thingTemp = DS18B20_Temperature();
  Serial.println("Temperature: " + String(thingTemp)+ " °C"); 
  if (thingTemp != -127.00){ // -127.00 is value used for error
    Serial.println("Temperature measured non-erroneous. Sending data to Thingspeak"); 
      ThingSpeak.setField(1, thingTemp);// Set field 1 with the actual temperature value 
  }
  deinitializeTemperatureSensor();

  int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);// Write the fields to ThingSpeak
  // Check if the write was successful
  if (responseCode == 200) {
      Serial.println("Data sent to Thingspeak successfully:");
    } 
  else {
    Serial.print("Error sending data: ");
    Serial.println(responseCode);
    }

    delay(16000); // Wait 20 seconds before sending the next data
}
