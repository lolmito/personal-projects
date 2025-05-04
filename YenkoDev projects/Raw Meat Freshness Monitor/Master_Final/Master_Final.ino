
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

Adafruit_ADS1115 ads;

const char *ssid = ""; // Input SSID here
const char *password = ""; //Input password of wifi here

unsigned long channelID = 2844436;
const char *writeAPIKey = "WHYHACTNG5QEQLIK";

#define DHTPIN 13 //D7
#define DHTTYPE DHT22
#define VCC 5.0
#define RL 47
#define Ro 20
#define I2C_ADDR 0x27

//for MQ136
#define m_136 -1.3621
#define b_136 4.8843


//for MQ137
#define m_137 -0.9002
#define b_137 3.18



DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

bool testMode = true; // For testing purposes
unsigned long startTime1; // 5-minute interval sending of data
unsigned long startTime2; // 4-hour interval for sending of data

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Serial.println("Initializing...");
  connectToWiFi();
  ThingSpeak.begin(client);
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (true);
  }
  dht.begin();
  lcd.setCursor(0, 0);
  lcd.print("Initializing ");
  lcd.setCursor(0, 1);
  lcd.print("Sensors....");
  delay(50000);
  lcd.clear();
  startTime1 = millis();
  startTime2 = millis();
}

void connectToWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    lcd.setCursor(0, 1);
    lcd.print("WiFi connected!");
    Serial.println("\nWiFi connected!");
    delay(5000);
    lcd.clear();
}

int ppmConversion(float VRL, float m, float b) {
  float RS = ((VCC * RL) / VRL) - RL;
  return pow(10, ((log10(RS / Ro) - b) / m));
}



void displaySensorData(float ppm137, float ppm136, float temperature, float humidity) {

  if(testMode){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEST MODE");
    delay(5000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NH3: ");
  lcd.print(ppm137);
  lcd.setCursor(0, 1);
  lcd.print("H2S: ");
  lcd.print(ppm136);
  delay(10000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %");
  delay(10000);
}

void loop() {
  float voltage_MQ137 = (ads.readADC_SingleEnded(0) * 0.1875) / 1000.0;
  float voltage_MQ136 = (ads.readADC_SingleEnded(1) * 0.1875) / 1000.0;
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float ppm136 = ppmConversion(voltage_MQ136,m_136,b_136);
  float ppm137 = ppmConversion(voltage_MQ137,m_137,b_137);

  Serial.printf("MQ136 (ppm): %.2f\nMQ137 (ppm): %.2f\n", ppm136, ppm137);

  displaySensorData(ppm137, ppm136, temperature, humidity);

  if (testMode) {
    Serial.println("Test mode enabled: Data will not be sent to ThingSpeak");
    return;
  }
  if (millis() - startTime1 >= 300000) {  // Check if 5 minutes have passed
    Serial.println("5 minutes elapsed!");
    startTime1 = millis();  // Reset the timer

    //Send data to thingspeak
    ThingSpeak.setField(1, temperature); // Temperature (C)
    ThingSpeak.setField(2, humidity); //Humidity (%)
    ThingSpeak.setField(3, ppm136); //H2S (ppm)
    ThingSpeak.setField(4, ppm137); //NH3 (ppm)
    
    int response = ThingSpeak.writeFields(channelID, writeAPIKey);    
    Serial.println(response == 200 ? "Data sent to ThingSpeak successfully!" : "Failed to send data. HTTP Code: " + String(response));
  }
  if (millis() - startTime2 >= 14400000) {  // Check if 4 hours has passed
    Serial.println("4 hours elapsed!");
    startTime2 = millis();  // Reset the timer

    //Send data to thingspeak
    ThingSpeak.setField(5, temperature); // Temperature (C)
    ThingSpeak.setField(6, humidity); //Humidity (%)
    ThingSpeak.setField(7, ppm136); //H2S (ppm)
    ThingSpeak.setField(8, ppm137); //NH3 (ppm)
    
    int response = ThingSpeak.writeFields(channelID, writeAPIKey);    
    Serial.println(response == 200 ? "Data sent to ThingSpeak successfully!" : "Failed to send data. HTTP Code: " + String(response));
  }
  
}



/*
Wiring Diagram:

    ESP8266 Pins:

        3.3V → Power for sensors and LCD (if compatible)

        GND → Common ground for all components

        SDA (D2 on NodeMCU) → SDA of ADS1115 and LCD

        SCL (D1 on NodeMCU) → SCL of ADS1115 and LCD

        Digital Pins → For DHT22 (e.g., D4 on NodeMCU)

    ADS1115 Connections:

        VDD → 5V (depending on the ADS1115 module)

        GND → GND

        SCL → SCL (D1 on NodeMCU)

        SDA → SDA (D2 on NodeMCU)

        A0 → MQ137 Analog Output

        A1 → MQ136 Analog Output

    DHT22 Connections:

        VCC → 5V

        GND → GND

        Data → Digital Pin (D7 on NodeMCU)

    16x2 LCD Connections (I2C):

        VCC → 5V

        GND → GND

        SDA → SDA (D2 on NodeMCU)

        SCL → SCL (D1 on NodeMCU)

*/

