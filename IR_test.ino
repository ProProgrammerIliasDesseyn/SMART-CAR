#include <IRremote.hpp>

const int IR_ontvanger = 9;

void setup() {
  
  Serial.begin(9600);
  IrReceiver.begin(IR_ontvanger, ENABLE_LED_FEEDBACK);

}

void loop() {
  
    if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
    }

    if (IrReceiver.decodedIRData.command == 0x46) {
    Serial.println("VOORUIT");
    }

    if (IrReceiver.decodedIRData.command == 0x43) {
    Serial.println("RECHTS");
    }

    if (IrReceiver.decodedIRData.command == 0x15) {
    Serial.println("ACHTERUIT");
    }

    if (IrReceiver.decodedIRData.command == 0x44) {
    Serial.println("LINKS");
    }

    if (IrReceiver.decodedIRData.command == 0x40) {
    Serial.println("STOP");
    }

}
