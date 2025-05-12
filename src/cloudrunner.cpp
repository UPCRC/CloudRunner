#include "CloudRunner.h"
#include "constants.h"
//======================BOARD INITIALIZING FUNCTIONS======================
//-------------------------Class constructor-----------------------------
CloudRunner::CloudRunner(){
  return;
}
//-------------------------Initialize Motor output-----------------------------
//-> this function initializes all the motor related pins
void CloudRunner::begin(){
  pinMode(L_SPEED_PIN,OUTPUT);
  pinMode(R_SPEED_PIN,OUTPUT);
  pinMode(L_FORWARD,OUTPUT);
  pinMode(L_BACKWARD,OUTPUT);
  pinMode(R_FORWARD,OUTPUT);
  pinMode(R_BACKWARD,OUTPUT);


  // set the number of sensors operating for PID value
  if(SENSOR_NUM ==5)  pin = fivepin;
  if(SENSOR_NUM ==3)  pin = threepin;
}


//======================SENSOR UTILITY FUNCTIONS======================
//-------------------------Sensor Reading code-----------------------------
//This code works for sensor circuit based on 3pi robot by Pololu 
//For circuit explanation Refer to : https://www.youtube.com/watch?v=9XjSJV5MPc0&t=543s
int CloudRunner::read_sensor(int p_sensor_pin){  
  pinMode(p_sensor_pin,OUTPUT);
  digitalWrite(p_sensor_pin,HIGH);
  
  unsigned long start = micros(); 
  
  delayMicroseconds(10);
  pinMode(p_sensor_pin,INPUT);

  //Wait for the voltage to fall bellow threshold of HIGH, if this takes too long 
  //then output maximum value
  while(digitalRead(p_sensor_pin) == HIGH && micros() - start < MAX_LIMIT_SENSE);
    int val = micros() - start;
    
  return val;
}

void CloudRunner::test_read_sensor(){  
  int pin2 = read_sensor(2)/32;
  int pin4 = read_sensor(4)/32;
  int pin5 = read_sensor(5)/32;
  int pin7 = read_sensor(7)/32;
  int pin8 = read_sensor(8)/32;
  
  //printing Raw Sensor Values
  Serial.print("Pin 2: ");
  Serial.print(pin2);
  Serial.print(" Pin 4: ");
  Serial.print(pin4);
  Serial.print(" Pin 5: ");
  Serial.print(pin5);
  Serial.print(" Pin 7: ");
  Serial.print(pin7);
  Serial.print(" Pin 8: ");
  Serial.println(pin8);
  
}


//------------------Calibration function for PID sensors----------------------
//-->Records lowest and highest values of sensor values during initiation phase (only for PID sensors)
// Note: This sets the lowest and highest values for each sensor, since they are not uniform
void CloudRunner::calibrate_PID_sensors(){
  for (int i = 0; i < SENSOR_NUM; i++) {
    LB_vals[i] = 1000;
    UB_vals[i] = 0;
  }
  
  unsigned int start = millis(); 

  while (millis() - start < 5000) {
    drive_motor(TURNRIGHT);
    for(int i=0, raw_val=0; i < SENSOR_NUM;i++){
      raw_val = read_sensor(pin[i])/32;   //read sensors for raw data     

      //Check if raw value is less than Lower bound for this sensor, set new lower bound if yes
      if(LB_vals[i] > raw_val) LB_vals[i] = raw_val;
      //Check if raw value is more than Upper bound for this sensor, set new upper bound if yes
      if(UB_vals[i] < raw_val) UB_vals[i] = raw_val;

    }
  }
  drive_motor(STOP_MOTORS);
    Serial.print("Lower bound: ");
    for (int i = 0; i < SENSOR_NUM; i++) {
      Serial.print(String(LB_vals[i]) + " ");
  }
    Serial.print("Upper bound: ");
    for (int i = 0; i < SENSOR_NUM; i++) {
      Serial.print(String(UB_vals[i]) + " ");
  }


}



//--------------Calibration function for turn sensors-----------------------
//-> separate calibration function for turn sensors
//->this function sets new thresh values as calibration is underway
void CloudRunner::calibrate_turn_sensors(){

  R_onblk_thresh = (LB_vals[0]+ UB_vals[0])/2;
  L_onblk_thresh = (LB_vals[SENSOR_NUM -1]+ UB_vals[SENSOR_NUM - 1])/2;
  Serial.println("Rth: " + String(R_onblk_thresh) + " Lth: " + String(L_onblk_thresh));

}


//----------------Function to Get Line Position----------------------
//-->Determines line position after sensor is read
int CloudRunner::get_pos() {
  int raw[SENSOR_NUM];

  for(int i=0; i < SENSOR_NUM;i++){
    raw[i] = read_sensor(pin[i])/32;   //read sensors for raw data     
      
  }
  //Zero out existing Line Position Variables
  mass=0;
  torque=0;
  pos=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < SENSOR_NUM; i++){  
    mass += raw[i];
    torque += (raw[i] * i); //Torque = force + lever arm
  }
  
  //Note this multiplier may need changing based on the board & surface's behavior

  pos = (torque * torque_multiplier / mass);
  
  
  delay(1);

  return pos;
}


//----------------Function to Get Line Position----------------------
//-->Determines line position after sensor is read using the normalized values
//Note: this assumes you have called the calibrate_sensors() method
int CloudRunner::get_norm_pos() {
  int raw[SENSOR_NUM]= {0};   //contains raw values
  int norm_vals[SENSOR_NUM]={0}; //contains normalized values

  for(int i=0; i < SENSOR_NUM;i++){
    raw[i] = read_sensor(pin[i])/32;   //read sensors for raw data     
      
  }

  //Take raw values from sensors and normalize them given the
  //recorded Lower and Upper bounds for each sensor 
  //(from most recent call of calibrate_sensors())
  for(int i=0; i<SENSOR_NUM; i++){
    //Normalize to a 0-100 range of value 
    norm_vals[i] = map(raw[i], LB_vals[i], UB_vals[i], 0, 100);
  }

  //Zero out existing Line Position Variables
  mass=0;
  torque=0;
  pos=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < SENSOR_NUM; i++){  
    mass += norm_vals[i];
    torque += (norm_vals[i] * i); //Torque = force + lever arm
  }
  
  //Note this multiplier may need changing based on the board & surface's behavior
  pos = (torque * torque_multiplier)/ mass;
  delay(1);
  return pos;
}


//-----------------Function to check Left and right turns---------------
//-> Detect turns based on crossing of the threshold set by calibration
void CloudRunner::check_turn(){

  int R_val =0, L_val=0;  //mapped raw values

  R_val = read_sensor(R_TURN_PIN)/32;
  L_val = read_sensor(L_TURN_PIN)/32;

  //turn detection logic
  if(R_val > R_onblk_thresh && L_val < L_onblk_thresh){
    R_turn_detected = true;
    L_turn_detected = false;
    Intersect_detected = false;
  }else if(R_val < R_onblk_thresh && L_val > L_onblk_thresh){
    L_turn_detected = true;
    R_turn_detected = false;
    Intersect_detected = false;
  }else if(R_val> R_onblk_thresh && L_val > L_onblk_thresh){
    Serial.println("turn detected\n");
    Intersect_detected = true;
    R_turn_detected = false;
    L_turn_detected = false;

  } 
  //Set flags if turn detected
  
}


//----------------------Function to reset all turn flags------------------
//-> avoid latching by reseting all turn flags
void CloudRunner::reset_turn_detect(){
  R_turn_detected = false;
  L_turn_detected = false;
  Intersect_detected = false;
}


//======================MOTOR UTILITY FUNCTIONS======================
//-------------Function to steer the motors in a fixed interval-------------
//-> interfaces with motor driver via pwm
void CloudRunner::drive_motor(int p_direction){
  analogWrite(L_SPEED_PIN, 200);
  analogWrite(R_SPEED_PIN, 200);
  switch(p_direction){
    case STOP_MOTORS:
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,LOW);
      break;
    case FORWARD_MOTORS:
      digitalWrite(L_FORWARD,HIGH);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,HIGH);
      digitalWrite(R_BACKWARD,LOW);
      break;
    case REVERSE_MOTORS:
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,HIGH);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,HIGH);
      break;
    case TURNLEFT:
      digitalWrite(L_FORWARD,HIGH);
      digitalWrite(L_BACKWARD,LOW);
      digitalWrite(R_FORWARD,LOW);
      digitalWrite(R_BACKWARD,HIGH);
      break;
    case TURNRIGHT:
      digitalWrite(L_FORWARD,LOW);
      digitalWrite(L_BACKWARD,HIGH);
      digitalWrite(R_FORWARD,HIGH);
      digitalWrite(R_BACKWARD,LOW);
      break;
    default:
      break;
  }
}

//----------------------Function to steer motors using PID------------------
//-> modulates speed based on calculated PID value
void CloudRunner::PID_steer(int p_PID_val) {
  // Calculating the effective motor speed:
  
  float Lspeed = INIT_spd + p_PID_val + L_spd_offset;
  float Rspeed = INIT_spd - p_PID_val + R_spd_offset ;

  // The motor speed should not exceed the max PWM value
  Lspeed = constrain(Lspeed, LB_spd, UB_spd);
  Rspeed = constrain(Rspeed, LB_spd, UB_spd);

  analogWrite(L_SPEED_PIN, Lspeed); //Left Motor Speed
  analogWrite(R_SPEED_PIN, Rspeed); //Right Motor Speed

  //following lines of code are to make the bot move forward
  
  digitalWrite(L_FORWARD, HIGH);
  digitalWrite(L_BACKWARD, LOW);
  digitalWrite(R_FORWARD , HIGH);
  digitalWrite(R_BACKWARD, LOW);
  
}

//----------------------Function to calculate PID------------------
//-> calculates PID value based on error passed
int CloudRunner::PID_calc(int p_error_val){
  P = p_error_val;
  D = p_error_val - old_error;
  I += P;
  I = constrain(I,-60,60);
  old_error = p_error_val;
  int PID_val = P*Kp + I*Ki + D*Kd;
  return PID_val;
}


//--------------------------Function to use DC motors as mini buzzers-------------------
//-> Use this function to provide feedback and make debugging easier
void CloudRunner::beep_motor(){
  
  int amplitude = 255;      //PWM value controls amplitude of beep
  int oscillation_delay_us = 500;  

  //stop motors
  digitalWrite(L_FORWARD,LOW);
  digitalWrite(L_BACKWARD,LOW);
  digitalWrite(R_FORWARD,LOW);
  digitalWrite(R_BACKWARD,LOW);
  analogWrite(L_SPEED_PIN,0);
  analogWrite(R_SPEED_PIN,0);

  
  //oscillate motors to achieve beep
  for( int i = 0; i < 20; i++){

    
    analogWrite(L_SPEED_PIN,amplitude);
    analogWrite(R_SPEED_PIN,amplitude);
    
       //Run pins motor forward
    digitalWrite(L_FORWARD,HIGH);
    digitalWrite(L_BACKWARD,LOW);
    digitalWrite(R_FORWARD,HIGH);
    digitalWrite(R_BACKWARD,LOW);
    delayMicroseconds(oscillation_delay_us);
      //Run pins motor backwards
    digitalWrite(L_FORWARD,LOW);
    digitalWrite(L_BACKWARD,HIGH);
    digitalWrite(R_FORWARD,LOW);
    digitalWrite(R_BACKWARD,HIGH);
    delayMicroseconds(oscillation_delay_us);
  }

  analogWrite(L_SPEED_PIN,0);
  analogWrite(R_SPEED_PIN,0);
}

//----------------Functions to make quarter second beeps----------
//-> Use motors as buzzers for easier debugging
void CloudRunner::motor_quartersec_beep(int p_num_beep){
  for(int i = 0;i < p_num_beep;i++){
    beep_motor();
    delay(250);
  }
}



//======================PID UTILITY FUNCTIONS======================
//-> setter and getter functions 
void CloudRunner::set_Kd(int p_Kd){
  Kp = p_Kd;
}

void CloudRunner::set_Kp(int p_Kp){
  Kp = p_Kp;
}

void CloudRunner::set_Ki(int p_Ki){
  Ki = p_Ki;
}

void CloudRunner::set_count(int val)
{
  count = val;
}

void CloudRunner::set_L_spd_offset(int offset){
  L_spd_offset = offset;
}

void CloudRunner::set_R_spd_offset(int offset){
  R_spd_offset = offset;
}

void CloudRunner::set_LB_spd(int speed){
  LB_spd = speed;
}

void CloudRunner::set_UB_spd(int speed){
  UB_spd = speed;
}

void CloudRunner::set_INIT_spd(int speed){
  INIT_spd = speed;
}

void CloudRunner::set_with_intersection(boolean statement, int count)
{
  with_intersection = statement;
  intersectionCount = count;
}

int CloudRunner::get_Kp(){
  return Kp;
}

int CloudRunner::get_Kd(){
  return Kd;
}

int CloudRunner::get_Ki(){
  return Ki;
}

int CloudRunner::get_count(){
    return count;
}

int CloudRunner::get_L_spd_offset(){
    return L_spd_offset;
}

int CloudRunner::get_R_spd_offset(){
    return R_spd_offset;
}

int CloudRunner::get_LB_spd(){
    return LB_spd;
}

int CloudRunner::get_UB_spd(){
    return UB_spd;
}

int CloudRunner::get_INIT_spd(){
    return INIT_spd;
}

boolean CloudRunner::get_with_intersection()
{
    return with_intersection;
}

int CloudRunner::get_intersectionCount()
{
    return intersectionCount;
}

boolean CloudRunner::intersection_detected()
{
    return Intersect_detected;
}

void CloudRunner::set_target_pos(int p_pos){
  target_pos = p_pos;
}

int CloudRunner::get_target_pos(){
  return target_pos;
}

void CloudRunner::set_torque_multiplier(int p_multiplier){
  torque_multiplier = p_multiplier;
}

int CloudRunner::get_torque_multiplier(){
  return torque_multiplier;
}