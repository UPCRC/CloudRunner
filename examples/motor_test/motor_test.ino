
#define L_SPEED_PIN 10 //D10
#define R_SPEED_PIN 9 //D9
#define L_FORWARD A0  //A0
#define L_BACKWARD A1 //A1
#define R_FORWARD A2 //A2
#define R_BACKWARD A3 //A3                                                                      

void setup() {
  //Setting up pinmodes
  pinMode(L_SPEED_PIN,OUTPUT);
  pinMode(R_SPEED_PIN,OUTPUT);
  pinMode(L_FORWARD,OUTPUT);
  pinMode(L_BACKWARD,OUTPUT);
  pinMode(R_FORWARD,OUTPUT);
  pinMode(R_BACKWARD,OUTPUT);
}

void loop() {
  //Run pins motor forward
  digitalWrite(L_FORWARD,HIGH);
  digitalWrite(L_BACKWARD,LOW);

  digitalWrite(R_FORWARD,HIGH);
  digitalWrite(R_BACKWARD,LOW);
  
  
  analogWrite(L_SPEED_PIN,255);
  analogWrite(R_SPEED_PIN,255);
  //After running the code, if you motor operates in reverse, swap its pin
  //If all the pins are now correct, kindly change the same pins in the constants header file (constants.h)
}

