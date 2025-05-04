/*
Wiring Diagram:

    ESP8266 Pins:

        3.3V → Power for sensors and LCD (if compatible)

        GND → Common ground for all components

        SDA (D2 on NodeMCU) → SDA of ADS1115 and LCD

        SCL (D1 on NodeMCU) → SCL of ADS1115 and LCD

        Analog Pin (A0) → Not used directly (ADS1115 handles analog signals)

        Digital Pins → For DHT22 (e.g., D4 on NodeMCU)

    ADS1115 Connections:

        VDD → 3.3V or 5V (depending on the ADS1115 module)

        GND → GND

        SCL → SCL (D1 on NodeMCU)

        SDA → SDA (D2 on NodeMCU)

        A0 → MQ137 Analog Output

        A1 → MQ136 Analog Output

    DHT22 Connections:

        VCC → 3.3V

        GND → GND

        Data → Digital Pin (e.g., D4 on NodeMCU)

    16x2 LCD Connections (I2C):

        VCC → 3.3V or 5V

        GND → GND

        SDA → SDA (D2 on NodeMCU)

        SCL → SCL (D1 on NodeMCU)

*/

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// Initialize ADS1115
Adafruit_ADS1115 ads;

// Initialize DHT22
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Initialize LCD (I2C address may vary, commonly 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Initialize ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115.");
    while (1);
  }

  // Initialize DHT22
  dht.begin();

  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("Initializing...");

  delay(2000); // Allow sensors to stabilize
}

void loop() {
  // Read MQ137 (Ammonia Sensor)
  int16_t adc0 = ads.readADC_SingleEnded(0);
  float ammonia = (adc0 * 0.1875) / 1000.0; // Convert to voltage

  // Read MQ136 (Hydrogen Sulfide Sensor)
  int16_t adc1 = ads.readADC_SingleEnded(1);
  float h2s = (adc1 * 0.1875) / 1000.0; // Convert to voltage

  // Read DHT22 (Temperature and Humidity)
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Display on Serial Monitor
  Serial.print("Ammonia: ");
  Serial.print(ammonia);
  Serial.print(" V, H2S: ");
  Serial.print(h2s);
  Serial.print(" V, Temp: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NH3: ");
  lcd.print(ammonia);
  lcd.print(" V");

  lcd.setCursor(0, 1);
  lcd.print("H2S: ");
  lcd.print(h2s);
  lcd.print(" V");

  delay(2000); // Wait before next reading

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print(" %");

  delay(2000); // Wait before next reading
}