const byte valuePin = A3; // potentiometer input
const byte controlPin = 13; // triac output
const byte interruptPin = 2; // phase sensing
const int range = 256;

const int valueFactor = 4; // 1024 (10 bit) / 256 (range)

volatile int value = 0;
volatile float increment = 0.0;

volatile float a = 0.0;
volatile bool skip = false;

void setup() {
  pinMode(controlPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), calculateSkip, CHANGE);
}

void calculateSkip() {
  if (digitalRead(interruptPin) == LOW) {
    a += increment;
  } else {
    skip = true;
    return;
  }

  if (a >= 1.0) {
    a -= 1.0;
    skip = true;
  } else {
    skip = false;
  }

  if (a > 1.0) {
    a = 0.0;
    skip = false;
  }
}

void updateIncrement() {
  increment = value / range;
  a = 0.0;
}

void updateValue() {
  int readValue = analogRead(valuePin);
  int oldValue = value * valueFactor;

  if (readValue > (oldValue + valueFactor) || readValue < (oldValue - valueFactor)) { // add some hysteresis to filter out noise
    value = readValue / valueFactor;
    updateIncrement();
  }
}

void updateControl() {
  if (skip) {
    digitalWrite(controlPin, LOW);
  } else {
    digitalWrite(controlPin, HIGH);
  }
}

void loop() {
  updateValue();
  updateControl();
}
