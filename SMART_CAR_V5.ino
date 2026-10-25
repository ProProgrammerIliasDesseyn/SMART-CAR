// Library
#include <IRremote.hpp>
#include <Servo.h>

// Pinnen beschrijven 
const int standby = 3;
const int PWM_rechts = 5;
const int richting_rechts = 7;
const int PWM_links = 6;
const int richting_links = 8;
const int IR_ontvanger = 9;
const int servoPin = 10;
const int echoPin = 12;
const int trigPin = 13;

const int sensorLinks = A2;
const int sensorMidden = A1;
const int sensorRechts = A0;


// Servo uit libary met naam ultasoonServo als naam
Servo ultrasoonServo;

// Lijnvolgen uit
bool lijnVolgen = false;


// Godmode uit
bool godmode = false;

// Tijd bijhouden voor wanneer de laatste knop werd ingedrukt
unsigned long tijd = 0;

// Tijd bijhouden wanneer de knop werd ingedrukt
unsigned long startTijd = 0;

// Onthouden wnr welke knop momenteel ingedrukt is
int huidigeKnop = 0;

// Snelheid van de motoren
int snelheid = 150;

// Variabelen voor afstand
long duur;
int afstand;

// Richting die de camera doorgeeft: 'L', 'R' of 'S' (rechtdoor)
char richting = 'S';

void setup() {

  // Pinnen als output/input beschrijven
  pinMode(standby, OUTPUT);
  pinMode(PWM_rechts, OUTPUT);
  pinMode(richting_rechts, OUTPUT);
  pinMode(PWM_links, OUTPUT);
  pinMode(richting_links, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensorLinks, INPUT);
  pinMode(sensorMidden, INPUT);
  pinMode(sensorRechts, INPUT);


  // De IR-ontvanger zit op pin 9
  IrReceiver.begin(IR_ontvanger, ENABLE_LED_FEEDBACK);

  // Beginnen met rust toestand
  digitalWrite(standby, HIGH);

  // Analoge meting toegangbaar
  Serial.begin(9600);

  // Installeren van de servo
  ultrasoonServo.attach(servoPin);
  ultrasoonServo.write(90);
  delay(500);

}

// Functie voor vooruit (rechts en links laten vooruit draaien)
void vooruit() {

  // Snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // Maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // Godmode staat uit dus normale snelheid (bij ingedrukt houden zal hij sneller gaan naarmate de tijd)
  if (godmode == false) {
    analogWrite(PWM_rechts, snelheid);
    analogWrite(PWM_links, snelheid);
  }

  // Godmode staat aan (als dit aanstaat rijd hij meteen sneller)
  if (godmode == true) {
    analogWrite(PWM_rechts, 255);
    analogWrite(PWM_links, 255);
  }

  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, HIGH);
}

// Functie voor achteruit (rechts en links laten achteruit draaien)
void achteruit(){
  
  // Snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // Maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // Godmode staat uit dus normale snelheid (bij ingedrukt houden zal hij sneller gaan naarmate de tijd)
 if (godmode == false) {
    analogWrite(PWM_rechts, snelheid);
    analogWrite(PWM_links, snelheid);
  }

  // Godmode staat aan (als dit aanstaat rijd hij meteen sneller)
  if (godmode == true) {
    analogWrite(PWM_rechts, 255);
    analogWrite(PWM_links, 255);
  }

  digitalWrite(richting_rechts, LOW);
  digitalWrite(richting_links, LOW);
}

// Functie maken voor naar rechts te draaien
void rechts(){

  // Snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // Maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // Godmode staat uit dus normale snelheid
  if (godmode == false) {

    // Rechter motor achteruit
    digitalWrite(richting_rechts, LOW);
    analogWrite(PWM_rechts, snelheid);

    // Linker motor vooruit
    digitalWrite(richting_links, HIGH);
    analogWrite(PWM_links, snelheid);
  } 
  if (godmode == true) {

    // Rechter motor achteruit
    digitalWrite(richting_rechts, LOW);
    analogWrite(PWM_rechts, 255);

    // Linker motor vooruit
    digitalWrite(richting_links, HIGH);
    analogWrite(PWM_links, 255);
  }
}

// Functie maken voor naar links te draaien
void links(){

  // Snelheid verhogen zolang de knop ingedrukt blijft
  snelheid = 150 + (millis() - startTijd) / 20;

  // Maximale snelheid
  if (snelheid > 255) {
    snelheid = 255;
  }

  // Godmode staat uit dus normale snelheid
  if (godmode == false) {

    // Rechter motor vooruit
    digitalWrite(richting_rechts, HIGH);
    analogWrite(PWM_rechts, snelheid);

    // Linker motor achteruit
    digitalWrite(richting_links, LOW);
    analogWrite(PWM_links, snelheid);
  } 

  if (godmode == true) {

    // Rechter motor vooruit
    digitalWrite(richting_rechts, HIGH);
    analogWrite(PWM_rechts, 255);

    // Linker motor achteruit
    digitalWrite(richting_links, LOW);
    analogWrite(PWM_links, 255);
  }
}

// Functie voor het stoppen
void stop() {

  // Rechter motor stoppen
  analogWrite(PWM_rechts, 0);
  
  // Linker motor stoppen
  analogWrite(PWM_links, 0);

  huidigeKnop = 0;
}

// Functie voor wheelie 
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

void meetAfstand(){

  // Ultrasone puls sturen
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Meten
  duur = pulseIn(echoPin, HIGH, 30000);

  // Omzetten naar cm
  if (duur == 0) {
    afstand = -1;
  } 
  else {
    afstand = duur * 0.0343 / 2;
  }
}

void linksKijken(){


  ultrasoonServo.write(150);
  delay(500);

  meetAfstand();
}

void rechtsKijken(){
  
  ultrasoonServo.write(30);
  delay(500);

  meetAfstand();
}

void vooruitKijken(){

  ultrasoonServo.write(90);
  delay(500);
}

void vooruitTijd(int tijdRijden){

  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, HIGH);

  analogWrite(PWM_rechts, 90);
  analogWrite(PWM_links, 90);

  delay(tijdRijden);
  stop();
}

void rechtsTijd(int tijdDraaien){

  digitalWrite(richting_rechts, LOW);
  digitalWrite(richting_links, HIGH);

  analogWrite(PWM_rechts, 90);
  analogWrite(PWM_links, 90);

  delay(tijdDraaien);
  stop();
}

void linksTijd(int tijdDraaien){

  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, LOW);

  analogWrite(PWM_rechts, 90);
  analogWrite(PWM_links, 90);

  delay(tijdDraaien);
  stop();
}


// Vooruit rijden langs het obstakel, de sensor kijkt opzij
void rijLangsObstakel(int kijkHoek, int extraTijd) {

  // Sensor opzij draaien
  ultrasoonServo.write(kijkHoek);
  delay(300);

  // Vooruit rijden
  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, HIGH);
  analogWrite(PWM_rechts, 90);
  analogWrite(PWM_links, 90);

  unsigned long beginRijden = millis();

  // Deel 1: rijden tot hij het obstakel naast zich ziet (max 2 sec)
  meetAfstand();
  while ((afstand == -1 || afstand > 40) && millis() - beginRijden < 2000) {
    meetAfstand();
    delay(20);
  }

  // Deel 2: rijden tot hij het obstakel niet meer ziet (max 2 sec)
  while (afstand > 0 && afstand <= 40 && millis() - beginRijden < 2000) {
    meetAfstand();
    delay(20);
  }

  // Nog wat doorrijden zodat de hele auto voorbij is
  delay(extraTijd);
  stop();
  delay(150);

  // Sensor terug vooruit
  vooruitKijken();
}

// Traag rechtdoor rijden tot een sensor de lijn ziet (max 5 sec)
void rijNaarLijn() {

  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, HIGH);
  analogWrite(PWM_rechts, 70);
  analogWrite(PWM_links, 70);

  unsigned long beginZoeken = millis();

  while (analogRead(sensorLinks) < 300 && analogRead(sensorMidden) < 300 && analogRead(sensorRechts) < 300 && millis() - beginZoeken < 5000) {
    // blijven rijden
  }

  // Klein stukje doorrijden
  delay(120);
  stop();
  delay(150);
}

// Links draaien tot de middelste sensor op de lijn staat
void draaiLinksTotLijn() {

  digitalWrite(richting_rechts, HIGH);
  digitalWrite(richting_links, LOW);
  analogWrite(PWM_rechts, 85);
  analogWrite(PWM_links, 85);

  while (analogRead(sensorMidden) < 300) {
    // blijven draaien
  }
}

// Rechts draaien tot de middelste sensor op de lijn staat
void draaiRechtsTotLijn() {

  digitalWrite(richting_rechts, LOW);
  digitalWrite(richting_links, HIGH);
  analogWrite(PWM_rechts, 85);
  analogWrite(PWM_links, 85);

  while (analogRead(sensorMidden) < 300) {
    // blijven draaien
  }
}
// Berichten van de camera lezen: {L}, {R} of {S}
void leesCamera() {

  while (Serial.available() > 0) {

    char letter = Serial.read();

    if (letter == 'L' || letter == 'R' || letter == 'S') {
      richting = letter;
    }
  }
}

// Wat doen op een kruispunt
void kruispunt() {

  // Stoppen op het kruispunt
  stop();

  // Oude berichten van de camera weggooien
  while (Serial.available() > 0) {
    Serial.read();
  }

  // Maximaal 3 seconden wachten op een pijl van de camera
  char richting = 'N';                     // geen pijl gezien
  unsigned long beginWachten = millis();

  while (millis() - beginWachten < 3000) {

    if (Serial.available() > 0) {
      char letter = Serial.read();

      if (letter == 'L' || letter == 'R' || letter == 'S') {
        richting = letter;
        break;                             // pijl gevonden: stoppen met wachten
      }
    }
  }

  // Geen pijl ontvangen binnen 3 sec: blijven staan
  if (richting == 'N') {
    stop();
    lijnVolgen = false;
    return;
  }

  // Stukje vooruit zodat het midden van de auto op het kruispunt staat
  vooruitTijd(300);
  delay(150);

  if (richting == 'R') {
    rechtsTijd(200);         // eerst van de rechte lijn af draaien
    draaiRechtsTotLijn();    // dan draaien tot hij de lijn naar rechts ziet
  }
  else if (richting == 'L') {
    linksTijd(200);
    draaiLinksTotLijn();
  }

  // Bij 'S' (rechtdoor) moet hij niets extra doen
}

// Links rond het obstakel (obstakel staat dan rechts van de auto: sensor op 10)
void ontwijkLinks() {

  linksTijd(400);              // 1: 90 graden naar links
  delay(150);

  rijLangsObstakel(10, 1200);   // 2: opzij tot voorbij de rand

  rechtsTijd(400);             // 3: terug rechtdoor zetten
  delay(150);

  rijLangsObstakel(10, 1100);   // 4: rechtdoor tot voorbij het obstakel

  rechtsTijd(220);             // 5: schuin naar de lijn (lijn ligt rechts)
  delay(150);

  rijNaarLijn();               // 6: rijden tot hij de lijn ziet
  draaiLinksTotLijn();         // 7: recht op de lijn gaan staan
}

// Rechts rond het obstakel (obstakel staat dan links van de auto: sensor op 170)
void ontwijkRechts() {

  rechtsTijd(400);             // 1: 90 graden naar rechts
  delay(150);

  rijLangsObstakel(170, 1200);  // 2: opzij tot voorbij de rand

  linksTijd(400);              // 3: terug rechtdoor zetten
  delay(150);

  rijLangsObstakel(170, 1100);  // 4: rechtdoor tot voorbij het obstakel

  linksTijd(220);              // 5: schuin naar de lijn (lijn ligt links)
  delay(150);

  rijNaarLijn();               // 6: rijden tot hij de lijn ziet
  draaiRechtsTotLijn();        // 7: recht op de lijn gaan staan
}

void volgLijn() {

  // Afstand tot een obstakel meten
  meetAfstand();

  // Stoppen als er een obstakel binnen 15 cm staat
  if (afstand > 0 && afstand <= 15) {

    stop();
    delay(150);

    // Links en rechts kijken
    linksKijken();
    int afstandLinks = afstand;

    rechtsKijken();
    int afstandRechts = afstand;

    vooruitKijken();

    // Links vrij: links ontwijken
    if (afstandLinks > 25 || afstandLinks == -1) {
      ontwijkLinks();
    }

    // Anders rechts vrij: rechts ontwijken
    else if (afstandRechts > 25 || afstandRechts == -1) {
      ontwijkRechts();
    }

    // Beide kanten dicht: blijven staan
    else {
      stop();
    }

    return;
  }

  // Waarden van de lijnsensoren uitlezen
  int linksWaarde = analogRead(sensorLinks);
  int middenWaarde = analogRead(sensorMidden);
  int rechtsWaarde = analogRead(sensorRechts);

  // Alle 3 sensoren zwart = kruispunt
  if (linksWaarde > 300 && middenWaarde > 300 && rechtsWaarde > 300) {
    kruispunt();
    return;
  }

  // Zwarte lijn onder de middelste sensor
  if (middenWaarde > 300) {

    digitalWrite(richting_rechts, HIGH);
    digitalWrite(richting_links, HIGH);

    analogWrite(PWM_rechts, 65);
    analogWrite(PWM_links, 65);
  }

  // Zwarte lijn links: naar links sturen
  else if (linksWaarde > 300) {

    digitalWrite(richting_rechts, HIGH);
    digitalWrite(richting_links, HIGH);

    analogWrite(PWM_rechts, 75);
    analogWrite(PWM_links, 0);
  }

  // Zwarte lijn rechts: naar rechts sturen
  else if (rechtsWaarde > 300) {

    digitalWrite(richting_rechts, HIGH);
    digitalWrite(richting_links, HIGH);

    analogWrite(PWM_rechts, 0);
    analogWrite(PWM_links, 75);
  }

  // Geen sensor ziet de lijn
  else {
    stop();
  }
}


void lijnvolgen();


void loop() {

// Altijd luisteren naar de camera
leesCamera();

// Knoppen linken aan de juiste knoppen via IR
if (IrReceiver.decode()) {

  // Tijd van het laatste signaal bijhouden
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
      // Door op OK te drukken zal lijnvolgen uit staan
      lijnVolgen = false;

      // Door op OK te drukken zal godmode uit staan
      godmode = false;
    }

    // Godmode start door knop 1 en je kan het weten door de korte wheelie op het begin vanaf dan zit je in godmode
    // dit betekent dat je sneller kan draaien en sneller kan rijden
    if (IrReceiver.decodedIRData.command == 0x16) {
      godmode = true;
      wheelie();
    }

    // Knop 2 start lijnvolgen
    if (IrReceiver.decodedIRData.command == 0x19) {
      lijnVolgen = true;
    }

    IrReceiver.resume();
  }
  // Stoppen als er geen IR-signaal meer komt
  if (millis() - tijd > 300 && lijnVolgen == false) {
    stop();
    huidigeKnop = 0;
  }

  // Automatisch de zwarte lijn volgen
  if (lijnVolgen == true) {
  volgLijn();
  }

}