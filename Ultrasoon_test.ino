const int trigPin = 13;
const int echoPin = 12;

long duur;
int afstand;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Ultrasone puls versturen
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Echo meten
  duur = pulseIn(echoPin, HIGH, 30000);

  // Omrekenen naar centimeters
  if (duur == 0) {
    Serial.println("Geen meting");
  } else {
    afstand = duur * 0.0343 / 2;

    Serial.print("Afstand: ");
    Serial.print(afstand);
    Serial.println(" cm");
  }

  delay(300);
}
