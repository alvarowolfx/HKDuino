
#include <MySigningNone.h>
#include <MyTransportNRF24.h>
#include <MyHwATMega328.h>
#include <MySensor.h>

#define RELAY_1  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 3 // Total number of attached relays
#define RELAY_ON 0  // GPIO value to write to turn on attached relay
#define RELAY_OFF 1 // GPIO value to write to turn off attached relay

// NRFRF24L01 radio driver (set low transmit power by default)
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
// Select AtMega328 hardware profile
MyHwATMega328 hw;
// Construct MySensors library
MySensor gw(radio, hw);

MyMessage lightMsgs[NUMBER_OF_RELAYS] = {
        MyMessage(RELAY_1, V_LIGHT),
        MyMessage(RELAY_1+1, V_LIGHT),
        MyMessage(RELAY_1+2, V_LIGHT)
};

MyMessage getLightMessage(int sensor){
    return lightMsgs[sensor-RELAY_1];
}

void incomingMessage(const MyMessage &message);

void setup() {
    // Initialize library and add callback for incoming messages
    gw.begin(incomingMessage, AUTO, true);
    // Send the sketch version information to the gateway and Controller
    gw.sendSketchInfo("Sala Jantar", "1.0");

    // Fetch relay status
    for (int i=0, pin=RELAY_1; i<NUMBER_OF_RELAYS;i++, pin++) {
        // Register all sensors to gw (they will be created as child devices)
        Serial.print("Presenting sensor :");
        Serial.println(pin);
        gw.present(pin, S_LIGHT);
        // Then set relay pins in output mode
        pinMode(pin, OUTPUT);
        // Set relay to last known state (using eeprom storage)
        digitalWrite(pin, gw.loadState(pin)?RELAY_ON:RELAY_OFF);

        Serial.print("Sending initial value:");
        Serial.println(pin);
        gw.send(getLightMessage(pin).set(gw.loadState(pin)));

    }
}


void loop() {
    // Alway process incoming messages whenever possible
    gw.process();
}

void incomingMessage(const MyMessage &message) {
    // We only expect one type of message from controller. But we better check anyway.
    if (message.type==V_LIGHT) {
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", With value: ");
        Serial.println(message.getBool());

        // Change relay state
        digitalWrite(message.sensor, message.getBool()?RELAY_ON:RELAY_OFF);
        // Store state in eeprom
        gw.saveState(message.sensor, message.getBool());

        // Write some debug info
        Serial.print(", New status: ");
        Serial.println(message.getBool());

        //ACK
        gw.send(getLightMessage(message.sensor).set(message.getBool()));
    }
}

