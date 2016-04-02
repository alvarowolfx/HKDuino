
#include <MySigningNone.h>
#include <MyTransportNRF24.h>
#include <MyHwATMega328.h>
#include <MySensor.h>
#include <DHT.h>

#define DHT_SENSOR_PIN 4
#define LIGHT_SENSOR_ANALOG_PIN 0
#define HUMIDITY_CHILD_ID  4
#define TEMP_CHILD_ID 3
#define LIGHT_CHILD_ID 1

unsigned long SLEEP_TIME_MS = 5*60*1000;

// NRFRF24L01 radio driver (set low transmit power by default)
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
// Select AtMega328 hardware profile
MyHwATMega328 hw;
// Construct MySensors library
MySensor gw(radio, hw);

MyMessage humMessage(HUMIDITY_CHILD_ID, V_HUM);
MyMessage tempMessage(TEMP_CHILD_ID, V_TEMP);
MyMessage lightMessage(LIGHT_CHILD_ID, V_LIGHT_LEVEL);

DHT dht(DHT_SENSOR_PIN, DHT11);

void incomingMessage(const MyMessage &message);
long getLightLevel();

void setup() {
    // Initialize library and add callback for incoming messages
    gw.begin(incomingMessage, AUTO, true);
    // Send the sketch version information to the gateway and Controller
    gw.sendSketchInfo("Estacao Tempo", "1.0");

    //Minimum read time
    gw.sleep(1000);

    gw.present(HUMIDITY_CHILD_ID, S_HUM);
    gw.send(humMessage.set(dht.readHumidity(),1));

    gw.present(TEMP_CHILD_ID, S_TEMP);
    gw.send(tempMessage.set(dht.readTemperature(),1));

    gw.present(LIGHT_CHILD_ID, S_LIGHT_LEVEL);
    gw.send(lightMessage.set(getLightLevel()));
}

long getLightLevel(){
    long lightLevel = map(1023-analogRead(LIGHT_SENSOR_ANALOG_PIN), 0, 1023, 0, 100);
    //int lightLevel = (1023-analogRead(LIGHT_SENSOR_ANALOG_PIN))/10.23;
    return lightLevel;
}


void loop() {
    // No need to receive msgs
    //gw.process();
    gw.sleep(SLEEP_TIME_MS);

    gw.send(humMessage.set(dht.readHumidity(),1));
    gw.send(tempMessage.set(dht.readTemperature(),1));
    gw.send(lightMessage.set(getLightLevel()));

}

void incomingMessage(const MyMessage &message) {
    //NOOP
}

