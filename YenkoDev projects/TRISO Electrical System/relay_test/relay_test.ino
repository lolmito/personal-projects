// Pin Definition
#define RELAY_PIN 2 // GPIO pin connected to the relay control

void setup() {
    Serial.begin(115200);
    
    // Set the relay pin as an OUTPUT
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Initially turn off the relay (LED off)
}

void loop() {
    // Turn ON the relay to activate the LED
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("LED is ON");
    delay(5000); // Keep the LED ON for 5 seconds

    // Turn OFF the relay to deactivate the LED
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("LED is OFF");
    delay(5000); // Keep the LED OFF for 5 seconds
}
