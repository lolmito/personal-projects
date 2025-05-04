#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2      // DHT22 data pin (GPIO2 on ESP8266) D4
#define DHTTYPE DHT22  // Define sensor type

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

void setup() {
    Serial.begin(115200);
    Serial.println("DHT22 Test - ESP8266");
    dht.begin();  // Start the sensor
}

void loop() {
    // Read temperature & humidity
    float temperature = dht.readTemperature(); // Celsius
    float humidity = dht.readHumidity();

    // Check if the readings are valid
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT22 sensor!");
    } else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("°C | Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
    }

    delay(2000); // Wait 2 seconds before next reading
}
