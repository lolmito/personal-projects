#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// WiFi Credentials
const char *ssid = "Google";       // 🔹 Replace with your WiFi Name
const char *password = "trailjovic"; // 🔹 Replace with your WiFi Password

// ThingSpeak Credentials
unsigned long channelID = 2844436; // 🔹 Replace with your ThingSpeak Channel ID
const char *writeAPIKey = "WHYHACTNG5QEQLIK"; // 🔹 Replace with your ThingSpeak Write API Key

// DHT22 Sensor Setup
#define DHTPIN 13  // GPIO Pin where DHT22 Data pin is connected D7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// WiFi Client
WiFiClient client;

void setup() {
    Serial.begin(115200);
    Serial.println("Initializing...");

    // Start DHT sensor
    dht.begin();

    // Connect to WiFi
    connectToWiFi();

    // Initialize ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    // Read Temperature & Humidity from DHT22
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Celsius

    // Check if readings are valid
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print values to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Send data to ThingSpeak
    ThingSpeak.setField(1, temperature); // Field 1: Temperature
    ThingSpeak.setField(2, humidity);    // Field 2: Humidity

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);
    
    if (response == 200) {
        Serial.println("Data sent to ThingSpeak successfully!");
    } else {
        Serial.print("Failed to send data. HTTP Code: ");
        Serial.println(response);
    }

    // Wait 15 seconds before next update
    delay(15000);
}

// 📡 Connect to WiFi Function
void connectToWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi connected!");
}
