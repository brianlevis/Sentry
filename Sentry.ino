#include <Servo.h>

Servo servo;

//  ---PIN LOCATIONS---
const int PIR = 2;
const int LED_RED = 2;
const int LED_GREEN = 3;
const int LED_GUNS = 4;
const int PING = 5;
const int SPEAKER = 6;
const int BUTTON_RESET = 7;
const int SERVO = 9;
//  -------------------

//  ---MOVEMENT AND DETECTION CONSTANTS---

const int VALUE_TOLERANCE = 50;
const int RANGE_TOLERANCE = 10;

const int ROTATION_DELAY = 20;

const int TOLERANCE_CM = 5;
const int TOLERANCE_DEGREES = 10;

const int TIME_BETWEEN_SHOTS = 75; // in milliseconds
const int GUN_FREQUENCY = 75;

//  --------------------------------------

int distances[170];

byte currentPosition;

boolean direction = true; // True = right
boolean alert = false;

String temp = "";

//bool check() { // returns true if distance is normal
//  Serial.print("Checking distance... ");
//  boolean normal = abs(distance[currentDirection] - getDistance()) < 5;
//  if(normal) Serial.println("normal.");
//  else Serial.println("alarm! Flagging state!");
//  return normal;
//}

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
  currentPosition = pos;
}

int getDistance() {
  pinMode(PING, OUTPUT);
  digitalWrite(PING, LOW);
  delayMicroseconds(2);
  digitalWrite(PING, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING, LOW);
  pinMode(PING, INPUT);
  int distance = pulseIn(PING, HIGH) / 58;//  Converts pulse duration in microseconds to centimeters
  return distance;
}

int getMotion() {
  if(digitalRead(PIR) == HIGH) return true;
  return false;
}

boolean locate() {
  Serial.print("Detected motion! Searching...");
  set(0);
  int flag = 0;
  for(int i = 0; i < 170; i++) {
    set(i);
    delay(ROTATION_DELAY);
    int difference = getDistance() - distances[i];
    if(difference > VALUE_TOLERANCE) flag++;
    else flag /= 2;
    if(flag > RANGE_TOLERANCE) {
      Serial.print(" FOUND DISTURBANCE AT POSITION ");
      Serial.print(currentPosition);
      Serial.println(": Tracking in progress!");
      return true;
    }
  }
  Serial.println("found nothing. Resetting!");
  return false;
}

void mapEnvironment() {
  Serial.println("Mapping environment... ");
  int tempMap[170];
  int error;
  set(-1);
  delay(500);
  for (int i = 0; i <= 170; i++) {
    set(i);
    delay(ROTATION_DELAY);
    tempMap[i] = getDistance();
  }
  for (int i = 170; i >= 0; i--) {
    set(i);
    delay(ROTATION_DELAY);
    distances[i] = getDistance();
  }
  set(85);
  Serial.print("done! Error: ");
  for (int i = 0; i < 170; i++) {
    int e = distances[i] - tempMap[i];
    error += abs(e);
    distances[i] = (distances[i] + tempMap[i]) / 2;
  }
  Serial.println(error);
  Serial.print("Read 1: ");
  Serial.print(distances[0]);
  for(int i = 1; i < 170; i++) {
    Serial.print(",");
    if(distances[i] < 100) Serial.print(0);
    Serial.print(distances[i]);
  }
  Serial.println("");
  Serial.print("Read 2: ");
  Serial.print(tempMap[0]);
  for(int i = 1; i < 170; i++) {
    Serial.print(",");
    if(tempMap[i] < 100) Serial.print(0);
    Serial.print(tempMap[i]);
  }
  Serial.println("");
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

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_GUNS, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(BUTTON_RESET, INPUT);
  pinMode(PIR, INPUT);
  //attack();
  servo.attach(SERVO);
  mapEnvironment();
}

void loop() {
  if (getMotion()) {
    locate();
    delay(2000);
    //track();
  }
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

