const byte valuePin = A3; // potentiometer input
const byte controlPin = 13; // triac output
const byte interruptPin = 3; // phase sensing

const unsigned int range = 127;
const unsigned int valueFactor = 8; // 1024 (10 bit) / 128 (range + 1)

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
    skip = true;
  } else {
    skip = false;
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
  int oldValue = value * valueFactor;

  if (readValue > (oldValue + valueFactor) || readValue < (oldValue - valueFactor)) { // add some hysteresis to filter out noise
    value = readValue / valueFactor;
    a = 0;
  }
}

void loop() {
  updateValue();
  delay(500);
}
