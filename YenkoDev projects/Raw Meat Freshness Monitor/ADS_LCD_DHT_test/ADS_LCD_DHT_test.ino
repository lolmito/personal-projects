#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 13        // Pin where DHT22 is connected GPI013
#define DHTTYPE DHT22    // DHT22 sensor
#define I2C_ADDR 0x27    // LCD I2C address

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Initialize the LCD
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2);

// Initialize the ADS1115
Adafruit_ADS1115 ads;

// Pin for ADS1115 (connected to A0)
int adcPin = 0;

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Start the DHT sensor
  dht.begin();
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("DHT22 & ADS1115");
  delay(3000);
  lcd.clear();

  // Initialize the ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115");
    while (1);
  }
  
  // Set the gain for ADS1115 (for example, 1x gain)
  ads.setGain(GAIN_TWOTHIRDS); // The range is +/-4.096V (1 bit = 0.000125V)
}

void loop() {
  // Read temperature and humidity from DHT22
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Read ADC value from ADS1115 (single-ended)
  int16_t adcValue = ads.readADC_SingleEnded(adcPin);
  float voltage = (adcValue / 32768.0) * 6.144; // Convert ADC value to voltage (for two-thirds setting)

  // Display on Serial Monitor
  Serial.print("Temp: "); Serial.print(t); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(h); Serial.println(" %");
  Serial.print("ADC Voltage: "); Serial.print(voltage); Serial.println(" V");
  
  // Display on LCD - Clear the LCD screen before updating with new data

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print(" %");
  
  delay(3000); // Update every 3 seconds
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ADC Voltage:");
  lcd.setCursor(0, 1);
  lcd.print(voltage);
  lcd.print(" V");

  delay(3000); // Update every 3 seconds
}
