/*
Wiring Diagram:

    ESP8266 Pins:

        5V → VCC of LCD

        GND → GND of LCD

        SDA (D2 on NodeMCU) → SDA of LCD

        SCL (D1 on NodeMCU) → SCL of LCD

    16x2 LCD Connections (I2C):

        VCC → 5V

        GND → GND

        SDA → SDA (D2 on NodeMCU)

        SCL → SCL (D1 on NodeMCU)
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C address may vary, commonly 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address if necessary

// Timer variable
unsigned long previousMillis = 0;
const long interval = 10000; // 10 seconds
bool toggle = false; // To track message switching

void setup() {
  // Start Serial Monitor
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight

  // Display a startup message
  lcd.setCursor(0, 0); // Set cursor to the first column, first row
  lcd.print("Hello, World!");
  lcd.setCursor(0, 1); // Set cursor to the first column, second row
  lcd.print("LCD Test");

  Serial.println("LCD initialized.");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Reset timer

    lcd.clear(); // Clear previous message

    if (toggle) {
      lcd.setCursor(0, 0);
      lcd.print("Temp:31.60 C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity:78.40%");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Temp: 32.60 C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: 74.40%");
    }

    toggle = !toggle; // Switch message for next cycle
  }
}

//Checking the I2C address of LCD
//Upload this code, open the Serial Monitor, and check the output for the I2C address of your LCD module. 
//Update the LiquidCrystal_I2C lcd(0x27, 16, 2); line in the main code with the correct address.
/*
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("I2C Scanner started...");
}

void loop() {
  byte error, address;
  int devices = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();
      devices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (devices == 0) {
    Serial.println("No I2C devices found.");
  }

  delay(5000); // Wait 5 seconds before scanning again
}
*/