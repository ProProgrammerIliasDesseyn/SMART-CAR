// library
#include <IRremote.hpp>

// Pinnen beschrijven 
const int standby = 3;
const int PWM_rechts = 5;
const int richting_rechts = 7;
const int PWM_links = 6;
const int richting_links = 8;
const int IR_ontvanger = 9;

void setup() {
  // Pinnen als output beschrijven
  pinMode(standby, OUTPUT);
  pinMode(PWM_rechts, OUTPUT);
  pinMode(richting_rechts, OUTPUT);
  pinMode(PWM_links, OUTPUT);
  pinMode(richting_links, OUTPUT);

  // de IR-ontvanger zit op pin 9
  IrReceiver.begin(IR_ontvanger, ENABLE_LED_FEEDBACK);

  // Beginnen met rust toestand
  digitalWrite(standby, HIGH);
}

// functie maken voor vooruit (rechts en links laten vooruit draaien)
void vooruit() {
  // rechts
  digitalWrite(richting_rechts, HIGH);
  analogWrite(PWM_rechts, 150);
  // links
  digitalWrite(richting_links, HIGH);
  analogWrite(PWM_links, 150);
} 

// functie maken voor achteruit (rechts en links laten achteruit draaien)
void achteruit(){
  // rechts
  digitalWrite(richting_rechts, LOW);
  analogWrite(PWM_rechts, 150);

  // links
  digitalWrite(richting_links, LOW);
  analogWrite(PWM_links, 150);
}

// functie maken voor naar rechts te draaien
void rechts(){
  // rechter motor stoppen
  analogWrite(PWM_rechts, 0);
  // linker motor vooruit
  digitalWrite(richting_links, HIGH);
  analogWrite(PWM_links, 150);
}

// functie maken voor naar links te draaien
void links(){
  // linker motor stoppen
  analogWrite(PWM_links, 0);  
  // rechter motor vooruit
  digitalWrite(richting_rechts, HIGH);
  analogWrite(PWM_rechts, 150);
}

// functie voor het stoppen
void stop() {

  // rechter motor stoppen
  analogWrite(PWM_rechts, 0);
  // linker motor stoppen
  analogWrite(PWM_links, 0);
}

void loop() {

if (IrReceiver.decode()) {

    if (IrReceiver.decodedIRData.command == 0x46) {
      vooruit();
    }

    if (IrReceiver.decodedIRData.command == 0x15) {
      achteruit();
    }

    if (IrReceiver.decodedIRData.command == 0x44) {
      links();
    }

    if (IrReceiver.decodedIRData.command == 0x43) {
      rechts();
    }

    if (IrReceiver.decodedIRData.command == 0x40) {
      stop();
    }

    IrReceiver.resume();
  }
}

