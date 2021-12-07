const byte valuePin = A3; // potentiometer input
const byte controlPin = 13; // triac output
const byte interruptPin = 3; // phase sensing

const unsigned int range = 127;
const unsigned int valueFactor = 4; // precalculated constant 1024 [10 bit ADC resolution] / 128 [range + 1] / 2

volatile unsigned int value = 0;

volatile unsigned int a = 0;
volatile bool skip = false;

void setup() {
  pinMode(controlPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), calculateSkip, RISING); // low-to-high transition = start of negative half-wave
}

void calculateSkip() {
  a += value;

  if (a >= range) {
    a -= range;
    skip = false;
  } else {
    skip = true;
  }

  if (a > range) {
    a = 0;
    skip = false;
  }

  updateControl();
}

void updateControl() {
  if (skip) {
    digitalWrite(controlPin, LOW);
  } else {
    digitalWrite(controlPin, HIGH);
  }
}

void updateValue() {
  int readValue = analogRead(valuePin);
  int oldValue = value * valueFactor * 2;

  if (readValue > (oldValue + valueFactor * 3) || (readValue + valueFactor) < oldValue) { // add some hysteresis to filter out noise
    value = readValue / valueFactor / 2;
    a = 0;
  }
}

void loop() {
  updateValue();
  delay(500);
}
