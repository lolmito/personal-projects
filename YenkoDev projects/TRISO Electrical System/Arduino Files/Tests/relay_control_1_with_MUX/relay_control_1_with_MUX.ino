// Pin definitions for relays
#define HYDRO_PIN 2     // GPIO 16 - Hydro relay
#define SOLAR_PIN 15     // GPIO 4 - Solar relay
#define GRID_PIN 0      // GPIO 0 - Grid relay

// Constants
const float voltageThreshold = 11.0;  // Voltage threshold to control the relay
const float sensorMaxVoltage = 12.0;  // Max voltage sensor can measure
const float vCCSensors = 3.00; // don't change for continuity (except relay)
const float vCCMUX = 5.00; // don't change for continuity
float batteryVoltage;
float measuredVoltage;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  // Initialize MUX 
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  // Initialize all relay pins as OUTPUT and turn them off (LOW)
  pinMode(HYDRO_PIN, OUTPUT);
  pinMode(SOLAR_PIN, OUTPUT);
  pinMode(GRID_PIN, OUTPUT);
  
  resetRelays();
}

void resetRelays() {
  // Set all relays to LOW (turn off)
  digitalWrite(HYDRO_PIN, LOW);
  digitalWrite(SOLAR_PIN, LOW);
  digitalWrite(GRID_PIN, LOW);
}

float readVoltagefromGridCurrentSensor(){

}

void changeRelayTo(const char* source) {
  resetRelays();  // Reset all relays before switching
  
  if (strcmp(source, "Solar") == 0) {
    digitalWrite(SOLAR_PIN, HIGH); // Turn on solar relay
    Serial.println("Switching to Solar power");
  } else if (strcmp(source, "Hydro") == 0) {
    digitalWrite(HYDRO_PIN, HIGH); // Turn on hydro relay
    Serial.println("Switching to Hydro power");
  } else if (strcmp(source, "Grid") == 0) {
    digitalWrite(GRID_PIN, HIGH);  // Turn on grid relay
    Serial.println("Switching to Grid power");
  }
  else if (strcmp(source, "Off") == 0){
    resetRelays();
  }
}

void loop() {
  for (int channel = 0; channel < NUM_CHANNELS; channel++) {
    selectChannel(channel);
    delay(10); // Allow the MUX to settle

    if (channel == 0) {
      float gridCurrentSensor = readVoltagefromGridCurrentSensor();

    voltage = vCCMUX * voltageSensor;
    Serial.print("Channel ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.print(voltage);
    Serial.println(" V");
    delay(500); // Wait before switching to the next channel
    }
    else if (channel == 1){
      Serial.print("Channel ");
      Serial.print(channel);
      delay(500); // Wait before switching to the next channel

    }
  }
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
  delay(2000);
}
