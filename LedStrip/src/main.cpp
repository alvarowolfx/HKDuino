
#include <MySigningNone.h>
#include <MyTransportNRF24.h>
#include <MyHwATMega328.h>
#include <MySensor.h>

//Define RGB pins
#define R_PIN  3  //R
#define G_PIN  5  //G
#define B_PIN  6  //B
#define LED_STRIP_CHILD_ID 1

// NRFRF24L01 radio driver (set low transmit power by default)
MyTransportNRF24 radio(RF24_CE_PIN, RF24_CS_PIN, RF24_PA_LEVEL_GW);
// Select AtMega328 hardware profile
MyHwATMega328 hw;
// Construct MySensors library
MySensor gw(radio, hw);

MyMessage rgbMsg(LED_STRIP_CHILD_ID, V_RGB);

void incomingMessage(const MyMessage &message);
String toHexFromRGB(int r, int g, int b);

void setup() {
    // Initialize library and add callback for incoming messages
    gw.begin(incomingMessage, AUTO, true);
    // Send the sketch version information to the gateway and Controller
    gw.sendSketchInfo("Fita Led RGB", "1.0");

    gw.present(LED_STRIP_CHILD_ID, S_RGB_LIGHT);

    pinMode(R_PIN, OUTPUT);
    pinMode(G_PIN, OUTPUT);
    pinMode(B_PIN, OUTPUT);

    // Set led strip colors to last known state (using eeprom storage)
    int rState = 100;//gw.loadState(R_PIN);
    int gState = 100;//gw.loadState(G_PIN);
    int bState = 100;//gw.loadState(B_PIN);
    analogWrite(R_PIN, rState);
    analogWrite(G_PIN, gState);
    analogWrite(B_PIN, bState);

    Serial.println(String(rState, HEX));
    Serial.println(rState);
    Serial.println(String(gState, HEX));
    Serial.println(gState);
    Serial.println(String(bState, HEX));
    Serial.println(bState);

    Serial.println("Getting stored value to hex");
    String hex = toHexFromRGB(rState, gState, bState);
    Serial.println("Initial value for rgb : ");
    Serial.println(hex);
    gw.send(rgbMsg.set(hex.c_str()));
}

String toHexFromRGB(int r, int g, int b){
    return (String(r, HEX) + String(g, HEX) + String(b, HEX)).c_str();
}

void parseHexToRGB(const char* hex, int* r, int* g, int* b){
    // Get rid of '#' and convert it to integer
    long number = strtol( &hex[0], NULL, 16);

    // Split them up into r, g, b values
    *r = number >> 16;
    *g = number >> 8 & 0xFF;
    *b = number & 0xFF;
}

void loop() {
    // Alway process incoming messages whenever possible
    gw.process();
}

void incomingMessage(const MyMessage &message) {
    // We only expect one type of message from controller. But we better check anyway.
    if (message.type==V_RGB) {
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", With value: ");
        Serial.println(message.getString());

        int r,g,b = 0;
        parseHexToRGB(message.getString(), &r, &g, &b);
        Serial.println(String(r, HEX));
        Serial.println(r);
        Serial.println(String(g, HEX));
        Serial.println(g);
        Serial.println(String(b, HEX));
        Serial.println(b);
        // Change led strip state
        analogWrite(R_PIN, r);
        analogWrite(G_PIN, g);
        analogWrite(B_PIN, b);
        // Store state in eeprom
        gw.saveState(R_PIN, r);
        gw.saveState(G_PIN, g);
        gw.saveState(B_PIN, b);

        // Write some debug info
        Serial.println("New status writed ");

        //ACK
        gw.send(rgbMsg.set(message.getString()));
    }
}

