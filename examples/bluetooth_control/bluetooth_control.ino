/*
USing SriTu hobby Controller App
Connect to the bluetooth module 
*/


#include<SoftwareSerial.h>

// Define Motor Pins
#define L_SPEED_PIN 10 //D9
#define R_SPEED_PIN 9 //D10
#define L_FORWARD A0 //A1
#define L_BACKWARD A1 //A0
#define R_FORWARD A2//A2
#define R_BACKWARD A3  //A3                                                                      
 
/* Create object named bt of the class SoftwareSerial */ 
SoftwareSerial BTSerial(12,13); /* (Rx,Tx) */	

//Define Motor Speed
int INITSpeed = 40;
int Speed_add = 70;

int R_offset = 0;
int L_offset = 10;

int R_Speed = INITSpeed + R_offset;
int L_Speed = INITSpeed + L_offset;

void setup() {
  
  pinMode(L_SPEED_PIN,OUTPUT);
  pinMode(R_SPEED_PIN,OUTPUT);
  pinMode(L_FORWARD,OUTPUT);
  pinMode(L_BACKWARD,OUTPUT);
  pinMode(R_FORWARD,OUTPUT);
  pinMode(R_BACKWARD,OUTPUT);
  BTSerial.begin(9600); // Default baud rate for HC-05/HC-06
  Serial.begin(9600);   // Debugging via Serial Monitor
}

void loop() {
  if (BTSerial.available() > 0) {
    char value = BTSerial.read();
    Serial.println(value);
    if (value == 'U') {
      Forward();
    } 
    else if (value == 'D') {
      Backward();
    } 
    else if (value == 'S') {
      Stop();
    } 
    else if (value == 'L') {
      Left();
    } 
    else if (value == 'R') {
      Right();
    }
    else if (value == 'T') {
      UpLeft();
    }
    else if (value == 'F') {
      UpRight();
    }
    else if (value == 'H') {
      DownLeft();
    }
    else if (value == 'G') {
      DownRight();
    }
  }
  delay(15);

}

void Forward() {
  analogWrite(L_SPEED_PIN, L_Speed);
  analogWrite(R_SPEED_PIN, R_Speed);
      digitalWrite(L_FORWARD,HIGH);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,HIGH);
      digitalWrite(R_BACKWARD,LOW);
}
void Backward() {
  analogWrite(L_SPEED_PIN, L_Speed);
  analogWrite(R_SPEED_PIN, R_Speed);
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,HIGH);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,HIGH);
}
void Stop() {

      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,LOW);
}
void Left() {
  analogWrite(L_SPEED_PIN, L_Speed);
  analogWrite(R_SPEED_PIN, R_Speed);
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,HIGH);
      digitalWrite(R_FORWARD,HIGH);
      digitalWrite(R_BACKWARD,LOW);

}
void Right() {
  analogWrite(L_SPEED_PIN, L_Speed);
  analogWrite(R_SPEED_PIN, R_Speed);
      digitalWrite(L_FORWARD,HIGH);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,HIGH);
}

void UpLeft() {
  analogWrite(L_SPEED_PIN, L_Speed);
  analogWrite(R_SPEED_PIN, R_Speed + Speed_add);
      digitalWrite(L_FORWARD,HIGH);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,HIGH);
      digitalWrite(R_BACKWARD,LOW);
}

void UpRight() {
  analogWrite(L_SPEED_PIN, L_Speed + Speed_add);
  analogWrite(R_SPEED_PIN, R_Speed);
      digitalWrite(L_FORWARD,HIGH);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,HIGH);
      digitalWrite(R_BACKWARD,LOW);
}

void DownLeft() {
  analogWrite(L_SPEED_PIN, L_Speed);
  analogWrite(R_SPEED_PIN, R_Speed + Speed_add);
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,HIGH);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,HIGH);
}

void DownRight() {
  analogWrite(L_SPEED_PIN, L_Speed + Speed_add);
  analogWrite(R_SPEED_PIN, R_Speed);
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,HIGH);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,HIGH);
}