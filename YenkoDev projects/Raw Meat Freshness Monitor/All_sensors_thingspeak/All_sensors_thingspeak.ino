#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>

// WiFi credentials
const char *ssid = "Google";
const char *password = "trailjovic";

// ThingSpeak settings
unsigned long channelID = 2844436;
const char *writeAPIKey = "WHYHACTNG5QEQLIK";

// Sensor Pins
#define DHTPIN 13 // DHT22 sensor data pin
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ADS1115 settings
Adafruit_ADS1115 ads;  // Create an ADS1115 instance
#define VCC 5.0         // Sensor operating voltage
#define RL_MQ136 10000  // 10kΩ load resistor for MQ-136
#define RL_MQ137 47000  // 47kΩ load resistor for MQ-137

// MQ sensor analog pins
const int mq136Pin = A0;
const int mq137Pin = A1;

WiFiClient client;

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);
  Serial.println("Starting...");

  // Initialize DHT sensor
  dht.begin();

  // Initialize ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115!");
    while (1);
  }
  
  // Connect to WiFi
  connectToWiFi();
  
  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read DHT sensor values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read analog values from MQ sensors
  int rawMQ136 = ads.readADC_SingleEnded(0);  // Read MQ-136 sensor
  int rawMQ137 = ads.readADC_SingleEnded(1);  // Read MQ-137 sensor

  // Convert raw values to voltage
  float voltageMQ136 = rawMQ136 * 0.1875 / 1000.0;
  float voltageMQ137 = rawMQ137 * 0.1875 / 1000.0;

  // Convert voltage to resistance (Rs)
  float Rs_MQ136 = ((VCC - voltageMQ136) / voltageMQ136) * RL_MQ136;
  float Rs_MQ137 = ((VCC - voltageMQ137) / voltageMQ137) * RL_MQ137;

  // Convert Rs values to ppm using a simple formula (customize this for your sensors)
  float ppm_MQ136 = Rs_MQ136 * 10;  // Example formula for MQ-136
  float ppm_MQ137 = Rs_MQ137 * 10;  // Example formula for MQ-137

  // Display data in Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, MQ-136 (H2S) PPM: ");
  Serial.print(ppm_MQ136);
  Serial.print(", MQ-137 (NH3) PPM: ");
  Serial.println(ppm_MQ137);

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature); // Field 1: Temperature
  ThingSpeak.setField(2, humidity);    // Field 2: Humidity
  ThingSpeak.setField(3, ppm_MQ136);   // Field 3: MQ-136 (H2S)
  ThingSpeak.setField(4, ppm_MQ137);   // Field 4: MQ-137 (NH3)

  // Update ThingSpeak every 15 seconds
  int httpCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.print("Failed to send data. HTTP Code: ");
    Serial.println(httpCode);
  }

  // Wait for 15 seconds before sending the next data
  delay(15000);
}

// Function to connect to WiFi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}
