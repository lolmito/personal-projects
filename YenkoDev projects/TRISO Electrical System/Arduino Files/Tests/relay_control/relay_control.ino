// Pin definitions for relays
#define HYDRO_PIN 2     // GPIO 2 - Hydro relay
#define SOLAR_PIN 15    // GPIO 15 - Solar relay
#define GRID_PIN 0      // GPIO 0 - Grid relay

// Constants
const float voltageThreshold = 11.0;  // Voltage threshold to control the relay
const float sensorMaxVoltage = 12.0;  // Max voltage sensor can measure

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize all relay pins as OUTPUT and turn them off (LOW)
  pinMode(HYDRO_PIN, OUTPUT);
  pinMode(SOLAR_PIN, OUTPUT);
  pinMode(GRID_PIN, OUTPUT);
  resetRelays();
  Serial.println("Relays Initialized");
}

void resetRelays() {
  // Set all relays to LOW (turn off)
  digitalWrite(HYDRO_PIN, HIGH);
  digitalWrite(SOLAR_PIN, HIGH);
  digitalWrite(GRID_PIN, HIGH);
  Serial.println("All relays reset to LOW");
}

void changeRelayTo(const char* source) {
  Serial.print("Changing relay to: ");
  Serial.println(source);
  resetRelays();  // Reset all relays before switching

  // Activate the selected relay
  if (strcmp(source, "Solar") == 0) {
    digitalWrite(SOLAR_PIN, LOW); // Turn on solar relay
    Serial.println("Solar relay activated");
  } else if (strcmp(source, "Hydro") == 0) {
    digitalWrite(HYDRO_PIN, LOW); // Turn on hydro relay
    Serial.println("Hydro relay activated");
  } else if (strcmp(source, "Grid") == 0) {
    digitalWrite(GRID_PIN, LOW);  // Turn on grid relay
    Serial.println("Grid relay activated");
  }
}

void loop() {
  // Simulated voltage readings
  int randNumberSolarVoltage = random(0, 13);  // 0 to 12V
  int randNumberHydroVoltage = random(0, 13);  // 0 to 12V
  
  // Print simulated voltages
  Serial.print("Solar Voltage: ");
  Serial.println(randNumberSolarVoltage);
  
  Serial.print("Hydro Voltage: ");
  Serial.println(randNumberHydroVoltage);

  // Decision-making based on voltage values
  if (randNumberHydroVoltage < voltageThreshold && randNumberSolarVoltage < voltageThreshold) {
    changeRelayTo("Grid");  // Both are below threshold, use Grid
  } else if (randNumberSolarVoltage >= voltageThreshold && randNumberHydroVoltage >= voltageThreshold) {
    // Both are within range, choose the one with the higher voltage
    changeRelayTo((randNumberSolarVoltage >= randNumberHydroVoltage) ? "Solar" : "Hydro");
  } else if (randNumberSolarVoltage >= voltageThreshold) {
    changeRelayTo("Solar");  // Solar is within range, hydro is not
  } else if (randNumberHydroVoltage >= voltageThreshold) {
    changeRelayTo("Hydro");  // Hydro is within range, solar is not
  }

  // Small delay before next measurement
  delay(5000);
}
