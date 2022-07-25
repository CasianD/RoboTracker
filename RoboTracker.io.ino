#include <NewPing.h>
#include <SoftwareSerial.h>

#define TRIG_PIN 8
#define ECHO_PIN 7
#define CONST_DISTANCE 20
#define MAX_DISTANCE 30
#define light A0
#define LED 13

int Lvalue  = analogRead(light);
int mVolt = map(Lvalue,0,1023,0,5000);
float volt = (double)mVolt/1000;

int right_fr = 3;
int right_bk = 2;
int left_fr = 4;
int left_bk = 5;

NewPing sonar(TRIG_PIN,ECHO_PIN,MAX_DISTANCE);

int dist = 0;
int standby = 0;

void moveForward(){
  digitalWrite(right_fr,HIGH);
  digitalWrite(right_bk,LOW);
  digitalWrite(left_fr,HIGH);
  digitalWrite(left_bk,LOW);
}

void moveBackward(){
  digitalWrite(right_fr,LOW);
  digitalWrite(right_bk,HIGH);
  digitalWrite(left_fr,LOW);
  digitalWrite(left_bk,HIGH);
}

void moveRight(){
  digitalWrite(right_fr,LOW);
  digitalWrite(right_bk,HIGH);
  digitalWrite(left_fr,HIGH);
  digitalWrite(left_bk,LOW);
}

void moveLeft(){
  digitalWrite(right_fr,HIGH);
  digitalWrite(right_bk,LOW);
  digitalWrite(left_fr,LOW);
  digitalWrite(left_bk,HIGH);
}

void moveStop(){
  digitalWrite(right_fr,LOW);
  digitalWrite(right_bk,LOW);
  digitalWrite(left_fr,LOW);
  digitalWrite(left_bk,LOW);
}
////////////////////////////////////
SoftwareSerial hc06(9,10);

int scan(){
  return (sonar.ping_cm());
}

void setup() {

if (volt<1)
    digitalWrite(LED, HIGH);
    
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  hc06.begin(9600);
  if (hc06.available())
    Serial.write(hc06.read());
  if (Serial.available())
    hc06.write(Serial.read());
    ///////////////////
  pinMode(right_fr,OUTPUT);
  pinMode(right_bk,OUTPUT);
  pinMode(left_fr,OUTPUT);
  pinMode(left_bk,OUTPUT);
  digitalWrite(right_fr,LOW);
  digitalWrite(right_bk,LOW);
  digitalWrite(left_fr,LOW);
  digitalWrite(left_bk,LOW);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  
}

void loop() {

  dist = scan();
  if ((dist != 0) && ((dist > CONST_DISTANCE+2) || (dist < CONST_DISTANCE-2)))
  {
    digitalWrite(LED_BUILTIN,HIGH);
    standby = 0;
    if ((dist !=0) && (dist > CONST_DISTANCE))
    {
      moveForward();
    }
    if ((dist !=0) && (dist < CONST_DISTANCE))
    {
      moveBackward();
    }
  }
  else if (dist !=0)
  {
    moveStop();
    digitalWrite(LED_BUILTIN,LOW);
    standby = 0;
  }
  else
  {
    moveStop();
    if (standby == 0)
    {
      if (dist == 0)
      delay(1000);
      unsigned long initial_time = millis();
      unsigned long current_time = initial_time;
      while((current_time - initial_time < 1000) && (dist == 0))
      {
        moveLeft();
        current_time = millis();
        dist = scan();
        Serial.println(dist);
      }
      moveStop();
      if (dist == 0)
      delay(1000);
      initial_time = millis();
      current_time = initial_time;
      while((current_time - initial_time < 2000) && (dist == 0))
      {
        moveRight();
        current_time = millis();
        dist = scan();
        Serial.println(dist);
      }
      moveStop();
      if (dist == 0)
      delay(1000);
      initial_time = millis();
      current_time = initial_time;
      while((current_time - initial_time < 1000) && (dist == 0))
      {
        moveLeft();
        current_time = millis();
        dist = scan();
        Serial.println(dist);
      }
      moveStop();
    }
    if (dist == 0)
    {
      moveStop();
      standby = 1;
      dist = scan();
      digitalWrite(LED_BUILTIN,HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN,LOW);
      delay(1000);
    }
  }
}
