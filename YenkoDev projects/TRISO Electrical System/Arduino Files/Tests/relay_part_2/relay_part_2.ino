// Pin definitions for the multiplexer and relays
#define S0_PIN 12      // GPIO 12 - S0 for 74HC4051
#define S1_PIN 13      // GPIO 13 - S1 for 74HC4051 and Relay later
#define S2_PIN 14      // GPIO 14 - S2 for 74HC4051 and Relay later

#define RELAY1_PIN 13  // GPIO 13 - Relay 1
#define RELAY2_PIN 14  // GPIO 14 - Relay 2

void setup() {
  Serial.begin(9600);
  
  // Initialize 74HC4051 (Multiplexer) pins
  initMultiplexer();
  
  // Simulate using the multiplexer
  //selectMultiplexerChannel(0);  // Select channel 0 for analog reading
  
  // After using the multiplexer, deinitialize the multiplexer pins
  deinitMultiplexer();
  
  // Reinitialize GPIO 13 and 14 for relay control
  initRelays();
  
  // Activate relays for testing
  activateRelay(RELAY1_PIN);
  delay(5000);
  activateRelay(RELAY2_PIN);
  delay(5000);
  deactivateRelays();
}

void initMultiplexer() {
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  
  Serial.println("Multiplexer initialized");
}

void deinitMultiplexer() {
  // Set multiplexer control pins back to INPUT (deinitialize)
  pinMode(S0_PIN, INPUT);
  pinMode(S1_PIN, INPUT);
  pinMode(S2_PIN, INPUT);
  
  Serial.println("Multiplexer deinitialized");
}

void selectMultiplexerChannel(int channel) {
  // Assuming we're selecting one of 8 channels (3-bit selection)
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));
  
  Serial.print("Multiplexer channel selected: ");
  Serial.println(channel);
}

void initRelays() {
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  
  // Ensure relays are initially off
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  
  Serial.println("Relays initialized");
}

void activateRelay(int relayPin) {
  digitalWrite(relayPin, LOW);  // Turn on relay
  Serial.print("Relay on GPIO ");
  Serial.print(relayPin);
  Serial.println(" activated");
}

void deactivateRelays() {
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  
  Serial.println("All relays deactivated");
}

void loop() {
  // Add any repeating tasks if necessary
}
