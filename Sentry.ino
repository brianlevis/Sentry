#include <Servo.h>

Servo servo;

//  ---MOVEMENT AND DETECTION CONSTANTS---

const long ROTATION_CONSTANT = 1275; // 100% power

const int TOLERANCE_CM = 5;
const int TOLERANCE_DEGREES = 10;

const int TIME_BETWEEN_SHOTS = 75; // in milliseconds
const int GUN_FREQUENCY = 75;

//  --------------------------------------

//  ---PIN LOCATIONS---
const int PIR = 1;
const int LED_RED = 2;
const int LED_GREEN = 3;
const int LED_GUNS = 4;
const int PING = 5;
const int SPEAKER = 6;
const int BUTTON_RESET = 7;
const int SERVO = 9;

//  -------------------

int distances[170];
int currentDirection = 16;

String temp = "";

//bool check() { // returns true if distance is normal
//  Serial.print("Checking distance... ");
//  boolean normal = abs(distance[currentDirection] - getDistance()) < 5;
//  if(normal) Serial.println("normal.");
//  else Serial.println("alarm! Flagging state!");
//  return normal;
//}

int getDistance() {
  pinMode(PING, OUTPUT);
  digitalWrite(PING, LOW);
  delayMicroseconds(2);
  digitalWrite(PING, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING, LOW);
  pinMode(PING, INPUT);
  int distance = pulseIn(PING, HIGH) / 58;//  Converts pulse duration in microseconds to centimeters
  //Serial.println(distance);
  return distance;
}

void wait(long time) {
  long start = millis();
  while(millis() - start < time);
}

//void rotate(int degree) {
//  Serial.print("Rotating "); Serial.print(degree); Serial.print(" degrees in "); Serial.print(ROTATION_CONSTANT * abs(degree) / 360); Serial.println(" milliseconds");
//  currentDirection += degree / 10;
//  setServo(100 * abs(degree) / degree);
//  wait(ROTATION_CONSTANT * abs(degree) / 360);
//  setServo(0);
//}

void set(int pos) {
  servo.write(pos + 5);
}

void mapEnvironment() {
  Serial.println("Mapping environment... ");
  int tempMap[170];
  int error;
  set(-1);
  delay(500);
  for (int i = 0; i <= 170; i++) {
    set(i);
    delay(15);
    tempMap[i] = getDistance();
    Serial.print(",");
    Serial.print(tempMap[i]);
  }
  Serial.println("");
  for (int i = 170; i >= 0; i--) {
    set(i);
    delay(15);
    distances[i] = getDistance();
    Serial.print(",");
    Serial.print(distances[i]);
  }
  set(85);
  Serial.print("done! Error: ");
  for (int i = 0; i < 170; i++) {
    int e = distances[i] - tempMap[i];
    Serial.println(e);
    error += abs(e);
    Serial.println(error);
    distances[i] = (distances[i] + tempMap[i]) / 2;
  }
  Serial.println(error);
}

void attack() {
  Serial.println("Unauthorized entity detected: Attacking!");
  bool continuePursuit = true;
  int pursuingDirection = 1;
  long start = millis();
  long now = millis();
  digitalWrite(LED_RED, HIGH);
  while(continuePursuit) {
    if(now - start >= TIME_BETWEEN_SHOTS / 2) {
      start = millis();
      if(digitalRead(LED_GUNS) == HIGH) digitalWrite(LED_GUNS, LOW);
	else {
          digitalWrite(LED_GUNS, HIGH);
          tone(SPEAKER, GUN_FREQUENCY, 10);
        }
    }
    Serial.println("1");
    now = millis();
  }
  digitalWrite(LED_RED, LOW);
}

void updateLEDS(String LED) {
	
}

void disp(String segments[]) {
  String temp = "";
  for(int i = 0; i < sizeof(segments); i++) temp += segments[i];
  Serial.println(temp);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_GUNS, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(BUTTON_RESET, INPUT);
  //attack();
  servo.attach(SERVO);
  mapEnvironment();
}

void loop() {
  /*
  int flagCount = 0;
  long ledControl = millis();
  while(true){ // loop starts at position 0
    for(int i = 0; i < 31; i += 5) {
      Serial.print("Checking position "); Serial.println(i); Serial.print("Flag count is "); Serial.println(flagCount);
      goTo(i);
      if(flagCount >= 3) {
        flagCount = 0;
        attack();
      }
      if(!check()) flagCount++;
      else if(flagCount > 0) flagCount--;
    }
    for(int i = 30; i >= 0; i -= 5) {
      Serial.print("Checking position "); Serial.println(i); Serial.print("Flag count is "); Serial.println(flagCount);
      goTo(i);
      if(flagCount >= 3) {
        flagCount = 0;
        attack();
      }
      if(!check()) flagCount++;
      else if(flagCount > 0) flagCount--;
    }
  }
  */
}
