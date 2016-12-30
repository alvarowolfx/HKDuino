
// NRFRF24L01 radio driver (set low transmit power by default)
// #define MY_DEBUG
#define MY_RADIO_NRF24
#define MY_NODE_ID 3
#include <MySensors.h>

#define RELAY_1  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 3 // Total number of attached relays
#define RELAY_ON 0  // GPIO value to write to turn on attached relay
#define RELAY_OFF 1 // GPIO value to write to turn off attached relay

MyMessage lightMsgs[NUMBER_OF_RELAYS] = {
        MyMessage(RELAY_1, V_STATUS),
        MyMessage(RELAY_1+1, V_STATUS),
        MyMessage(RELAY_1+2, V_STATUS)
};

bool initialValueSent = false;

MyMessage getLightMessage(int sensor){
    return lightMsgs[sensor-RELAY_1];
}

void setup() {
  // Fetch relay status
  for (int i=0, pin=RELAY_1; i<NUMBER_OF_RELAYS;i++, pin++) {
      // Then set relay pins in output mode
      Serial.print("Setting sensor :");
      Serial.println(pin);
      pinMode(pin, OUTPUT);
  }
}

void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Sala-Jantar", "1.1");

  // Fetch relay status
  for (int i=0, pin=RELAY_1; i<NUMBER_OF_RELAYS;i++, pin++) {
      // Register all sensors to gw (they will be created as child devices)
      Serial.print("Presenting sensor :");
      Serial.println(pin);
      present(pin, S_BINARY, "Interruptor");
      wait(50);
  }
}


void loop() {
  if (!initialValueSent) {
    Serial.println("Requesting initial value");
    for (int i=0, pin=RELAY_1; i<NUMBER_OF_RELAYS;i++, pin++) {
        // Then set relay pins in output mode
        Serial.print("Request initial sensor data :");
        Serial.println(pin);

        int value = loadState(pin);
        send(getLightMessage(pin).set(value == RELAY_OFF ? false : true));
        wait(50);
        request(pin, V_STATUS);
        wait(50);
    }
    wait(2000);
  }
  sendHeartbeat();
}

void receive(const MyMessage &message) {

    if(message.isAck()){
      Serial.println("This is an ack from gateway");
    }
    // We only expect one type of message from controller. But we better check anyway.
    if (message.type == V_STATUS) {

        if (!initialValueSent) {
          Serial.println("Receiving initial value from controller");
          initialValueSent = true;
        }

        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", With value: ");
        Serial.println(message.getBool());

        int value = message.getBool() ? RELAY_ON: RELAY_OFF;
        // Change relay state
        digitalWrite(message.sensor, value);
        // Store state in eeprom
        saveState(message.sensor, value);

        // Write some debug info
        Serial.print(", New status: ");
        Serial.println(message.getBool());

        //ACK
        send(getLightMessage(message.sensor).set(message.getBool()));
    }
}
