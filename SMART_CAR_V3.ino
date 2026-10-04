// library
#include <IRremote.hpp>

// pinnen beschrijven 
const int standby = 3;
const int PWM_rechts = 5;
const int richting_rechts = 7;
const int PWM_links = 6;
const int richting_links = 8;
const int IR_ontvanger = 9;

// godmode uit
bool godmode = false;

// tijd bijhouden voor wanneer de laatste knop werd ingedrukt
unsigned long tijd = 0;

// tijd bijhouden wanneer de knop werd ingedrukt
unsigned long startTijd = 0;

int huidigeKnop = 0;

// snelheid van de motoren
int snelheid = 150;

void setup() {

  // pinnen als output beschrijven
  pinMode(standby, OUTPUT);
  pinMode(PWM_rechts, OUTPUT);
  pinMode(richting_rechts, OUTPUT);
  pinMode(PWM_links, OUTPUT);
  pinMode(richting_links, OUTPUT);

  // de IR-ontvanger zit op pin 9
  IrReceiver.begin(IR_ontvanger, ENABLE_LED_FEEDBACK);

  // beginnen met rust toestand
  digitalWrite(standby, HIGH);
}

// functie voor vooruit (rechts en links laten vooruit draaien)
void vooruit() {

  // snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // godmode staat uit dus normale snelheid (bij ingedrukt houden zal hij sneller gaan naarmate de tijd)
  if (godmode == false) {
    analogWrite(PWM_rechts, snelheid);
    analogWrite(PWM_links, snelheid);
  }

  // godmode staat aan (als dit aanstaat rijd hij meteen sneller)
  if (godmode == true) {
    analogWrite(PWM_rechts, 255);
    analogWrite(PWM_links, 255);
  }

  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, HIGH);
}

// functie voor achteruit (rechts en links laten achteruit draaien)
void achteruit(){
  
  // snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // godmode staat uit dus normale snelheid (bij ingedrukt houden zal hij sneller gaan naarmate de tijd)
 if (godmode == false) {
    analogWrite(PWM_rechts, snelheid);
    analogWrite(PWM_links, snelheid);
  }

  // godmode staat aan (als dit aanstaat rijd hij meteen sneller)
  if (godmode == true) {
    analogWrite(PWM_rechts, 255);
    analogWrite(PWM_links, 255);
  }

  digitalWrite(richting_rechts, LOW);
  digitalWrite(richting_links, LOW);
}

// functie maken voor naar rechts te draaien
void rechts(){

  // snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // godmode staat uit dus normale snelheid
  if (godmode == false) {

    // rechter motor achteruit
    digitalWrite(richting_rechts, LOW);
    analogWrite(PWM_rechts, snelheid);

    // linker motor vooruit
    digitalWrite(richting_links, HIGH);
    analogWrite(PWM_links, snelheid);
  } 
  if (godmode == true) {

    // rechter motor achteruit
    digitalWrite(richting_rechts, LOW);
    analogWrite(PWM_rechts, 255);

    // linker motor vooruit
    digitalWrite(richting_links, HIGH);
    analogWrite(PWM_links, 255);
  }
}

// functie maken voor naar links te draaien
void links(){

  // snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // godmode staat uit dus normale snelheid
  if (godmode == false) {

    // rechter motor vooruit
    digitalWrite(richting_rechts, HIGH);
    analogWrite(PWM_rechts, snelheid);

    // linker motor achteruit
    digitalWrite(richting_links, LOW);
    analogWrite(PWM_links, snelheid);
  } 

  if (godmode == true) {

    // rechter motor vooruit
    digitalWrite(richting_rechts, HIGH);
    analogWrite(PWM_rechts, 255);

    // linker motor achteruit
    digitalWrite(richting_links, LOW);
    analogWrite(PWM_links, 255);
  }
}

// functie voor het stoppen
void stop() {

  // rechter motor stoppen
  analogWrite(PWM_rechts, 0);
  
  // linker motor stoppen
  analogWrite(PWM_links, 0);

  huidigeKnop = 0;
}

// functie voor wheelie 
void wheelie() {

  // Momentum opbouwen
  digitalWrite(richting_rechts, LOW);
  digitalWrite(richting_links, LOW);
  analogWrite(PWM_rechts, 150);
  analogWrite(PWM_links, 150);

  delay(600);

  // Korte pauze
  stop();
  delay(40);

  // Maximale korte acceleratie
  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, HIGH);
  analogWrite(PWM_rechts, 255);
  analogWrite(PWM_links, 255);

  delay(250);

  stop();
}

void loop() {

// knoppen linken aan de juiste knoppen via IR
if (IrReceiver.decode()) {

  // tijd van het laatste signaal bijhouden
  tijd = millis();

    if (IrReceiver.decodedIRData.command == 0x46) {

        if (huidigeKnop != 0x46) {
        startTijd = millis();
        huidigeKnop = 0x46;
        }
      
      vooruit();
    }

    if (IrReceiver.decodedIRData.command == 0x15) { 

      if (huidigeKnop != 0x15) {
      startTijd = millis();
      huidigeKnop = 0x15;
      }

      achteruit();
    }

    if (IrReceiver.decodedIRData.command == 0x44) {

      if (huidigeKnop != 0x44) {
      startTijd = millis();
      huidigeKnop = 0x44;
      }

      links();
    }

    if (IrReceiver.decodedIRData.command == 0x43) {

      if (huidigeKnop != 0x43) {
      startTijd = millis();
      huidigeKnop = 0x43;
      }

      rechts();
    }

    if (IrReceiver.decodedIRData.command == 0x40) {
      stop();

      // door op OK te drukken zal godmode uit staan
      godmode = false;
    }

    // godmode start door knop 1 en je kan het weten door de korte wheelie op het begin vanaf dan zit je in godmode
    // dit betekent dat je sneller kan draaien en sneller kan rijden
    if (IrReceiver.decodedIRData.command == 0x16) {
      godmode = true;
      wheelie();
    }

    IrReceiver.resume();
  }
    // stoppen als er te lang geen signaal meer komt
    if (millis() - tijd > 300) {
    stop();
  }
}

