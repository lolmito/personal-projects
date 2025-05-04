#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define RL_MQ136 10000 // 10kΩ load resistor for MQ-136
#define RL_MQ137 10000  // 10kΩ load resistor for MQ-137
#define VCC 5.0         // Sensor operating voltage
#define RL 47  //The value of resistor RL is 47K
#define m -0.263 //Enter calculated Slope 
#define b 0.42 //Enter calculated intercept
#define Ro 20 //Enter found Ro value
#define MQ_sensor A0 //Sensor is connected to A4

void setup() {
    Serial.begin(115200);
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115!");
        while (1);
    }
    ads.setGain(GAIN_TWOTHIRDS);  // Gain = 2/3, allows up to 6.144V range
}

void loop() {
    int raw_MQ136 = ads.readADC_SingleEnded(0);  // MQ-136 on A0
    int raw_MQ137 = ads.readADC_SingleEnded(1);  // MQ-137 on A1
    
    float voltage_MQ136 = raw_MQ136 * 0.1875 / 1000.0;  // Convert ADC to voltage
    float voltage_MQ137 = raw_MQ137 * 0.1875 / 1000.0;

    // Calculate Rs (Sensor Resistance)
    float Rs_MQ136 = ((VCC * RL_MQ136) / voltage_MQ136) - (RL_MQ136);
    float Rs_MQ137 = ((VCC * RL_MQ137) / voltage_MQ137) - (RL_MQ137);

    Serial.print("MQ-136 Rs: ");
    Serial.println(Rs_MQ136, 3);
    Serial.print("MQ-137 Rs: ");
    Serial.println(Rs_MQ137, 3);
    Serial.println(voltage_MQ136, 3);
    Serial.println(voltage_MQ137, 3);


    float VRL; //Voltage drop across the MQ sensor
    float Rs; //Sensor resistance at gas concentration 
    float ratio; //Define variable for ratio
    VRL = voltage_MQ136; //Measure the voltage drop and convert to 0-5V
    Rs = ((5.0*RL)/VRL)-RL; //Use formula to get Rs value
    ratio = Rs/Ro;  // find ratio Rs/Ro
    float ppm = pow(10, ((log10(ratio)-b)/m)); //use formula to calculate ppm
    Serial.println("MQ136 (ppm) = "); //Display a ammonia in ppm
    Serial.println(ppm);
    Serial.println("Voltage = "); //Display a intro message 
    Serial.println(VRL);

    VRL = voltage_MQ137; //Measure the voltage drop and convert to 0-5V
    Rs = ((5.0*RL)/VRL)-RL; //Use formula to get Rs value
    ratio = Rs/Ro;  // find ratio Rs/Ro
    ppm = pow(10, ((log10(ratio)-b)/m)); //use formula to calculate ppm
    Serial.println("MQ137 (ppm) = "); //Display a ammonia in ppm
    Serial.println(ppm);
    Serial.println("Voltage = "); //Display a intro message 
    Serial.println(VRL);

    delay(5000);
}

