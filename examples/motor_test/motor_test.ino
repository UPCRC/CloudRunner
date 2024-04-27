
#define L_speed_pin 5 //D5
#define R_speed_pin 6 //D6
#define L_forward 14  //A0
#define L_backward 15 //A1
#define R_forward 16  //A2
#define R_backward 17 //A3
#define constant_speed 100

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
  
  analogWrite(L_speed_pin,100);
  analogWrite(R_speed_pin,100);
}


  /*
  //Beep Motors for Three seconds
  for(int i = 0 ; i < 3; i++){
   test_beep();
   delay(250);
   test_beep();
   
   delay(1000); 
  }

    test_speed();
  */

//This is the Referrence code for the test_beep() function
/*
 * void playSound(uint16_t freq, uint16_t duration_msec, uint8_t  amplitude)
{
  uint32_t n,k,period_usec,duration_usec;

  period_usec = 1000000L / freq;
  duration_usec = 1000 * (uint32_t) duration_msec;
  k = duration_usec / period_usec;

  motorSpeed (amplitude, amplitude);

  for (n = 0; n < k; n++)
  {
    motorDirection (DIR_FORWARD, DIR_FORWARD);
    delayMicroseconds(period_usec/2);
    motorDirection (DIR_REVERSE, DIR_REVERSE);
    delayMicroseconds(period_usec/2);
  }
  motorSpeed (0, 0);
}
 */

//--------------------------Function to use DC motors as mini buzzers-------------------
//-> Use this function to provide feedback and make debugging easier
void test_beep(){
  
  int amplitude = 255;      //PWM value controls amplitude of beep
  int oscillation_delay_us = 500;
  analogWrite(L_speed_pin,amplitude);
  analogWrite(R_speed_pin,amplitude);  

  //oscillate motors to achieve beep
  for( int i = 0; i < 20; i++){
    
    
       //Run pins motor forward
    digitalWrite(L_forward,HIGH);
    digitalWrite(L_backward,LOW);
    digitalWrite(R_forward,HIGH);
    digitalWrite(R_backward,LOW);
    delayMicroseconds(oscillation_delay_us);
      //Run pins motor backwards
    digitalWrite(L_forward,LOW);
    digitalWrite(L_backward,HIGH);
    digitalWrite(R_forward,LOW);
    digitalWrite(R_backward,HIGH);
    delayMicroseconds(oscillation_delay_us);
  }

  analogWrite(L_speed_pin,0);
  analogWrite(R_speed_pin,0);
}


//---------------Function for speed and direction--------------
//-> Use to test motor speed and direction control
void test_speed() {
   
   //Run pins motor forward
  digitalWrite(L_forward,HIGH);
  digitalWrite(L_backward,LOW);
  digitalWrite(R_forward,HIGH);
  digitalWrite(R_backward,LOW);


  for(int i = 0; i<255; i++){
    analogWrite(L_speed_pin,i);
    analogWrite(R_speed_pin,i);
    delay(10);
  }

  //Stop motors for 5 seconds
  digitalWrite(L_forward,HIGH);
  digitalWrite(L_backward,HIGH);
  digitalWrite(R_forward,HIGH);
  digitalWrite(R_backward,HIGH);
  delay(10);
  
  
   //Run pins motor backwards
  digitalWrite(L_forward,LOW);
  digitalWrite(L_backward,HIGH);
  digitalWrite(R_forward,LOW);
  digitalWrite(R_backward,HIGH);

  for(int i = 0; i<255; i++){
    analogWrite(L_speed_pin,i);
    analogWrite(R_speed_pin,i);
    delay(10);
  }

  
  //Stop motors for 5 seconds
  digitalWrite(L_forward,HIGH);
  digitalWrite(L_backward,HIGH);
  digitalWrite(R_forward,HIGH);
  digitalWrite(R_backward,HIGH);
  delay(10);
  
}
