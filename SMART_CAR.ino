// Pinnen beschrijven 
const int standby = 3;
const int PWM_rechts = 5;
const int richting_rechts = 7;
const int PWM_links = 6;
const int richting_links = 8;

void setup() {
  // Pinnen als output beschrijven
  pinMode(standby, OUTPUT);
  pinMode(PWM_rechts, OUTPUT);
  pinMode(richting_rechts, OUTPUT);
  pinMode(PWM_links, OUTPUT);
  pinMode(richting_links, OUTPUT);

  // Beginnen met rust toestand
  digitalWrite(standby, HIGH);
}

void loop() {
  digitalWrite(richting_rechts, HIGH);
  analogWrite(PWM_rechts, 150);
}
