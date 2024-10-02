
#define L_speed_pin 9 
#define R_speed_pin 10 
#define L_forward A2  
#define L_backward A3 
#define R_forward A0  
#define R_backward A1                                                                          

void setup() {
  //Setting up pinmodes
  
  pinMode(L_speed_pin,OUTPUT);
  pinMode(R_speed_pin,OUTPUT);
  pinMode(L_forward,OUTPUT);
  pinMode(L_backward,OUTPUT);
  pinMode(R_forward,OUTPUT);
  pinMode(R_backward,OUTPUT);
 
}

void loop() {
  //Run pins motor forward
  digitalWrite(L_forward,HIGH);
  digitalWrite(L_backward,LOW);

  digitalWrite(R_forward,HIGH);
  digitalWrite(R_backward,LOW);
  
  
  analogWrite(L_speed_pin,255);
  analogWrite(R_speed_pin,255);
  //After running the code, if you motor operates in reverse, swap its pin
  //If all the pins are now correct, kindly change the same pins in the constants header file (constants.h)
}

