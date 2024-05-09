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

  // In ver2 of the CloudRat board we found out that
  // there is a bug in the circuit so we need to set 
  // A6 and A7 as input
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}


//======================SENSOR UTILITY FUNCTIONS======================
//-------------------------Sensor Reading code-----------------------------
//This code works for sensor circuit based on 3pi robot by Pololu 
//For circuit explanation Refer to : https://www.youtube.com/watch?v=9XjSJV5MPc0&t=543s
int CloudRunner::read_sensor(int p_sensor_pin){  
  pinMode(p_sensor_pin,OUTPUT);
  digitalWrite(p_sensor_pin,HIGH);
  long val = 0;
  delay(1);
  pinMode(p_sensor_pin,INPUT);

  //Wait for the voltage to fall bellow threshold of HIGH, if this takes too long 
  //then output maximum value
  while(digitalRead(p_sensor_pin) == HIGH && val < MAX_LIMIT_SENSE)
    val++;
    
  return val;
}

//------------------Calibration function for PID sensors----------------------
//-->Records lowest and highest values of sensor values during initiation phase (only for PID sensors)
void CloudRunner::calibrate_sensors(){
  
  for(int i=0, raw_val=0, pin= START_SENSOR_PIN; i < SENSOR_NUM;i++,pin++){
    raw_val = read_sensor(pin)/32;   //read sensors for raw data     

    if(lowest_val > raw_val) lowest_val = raw_val;     //get values for normalizing
    if(highest_val < raw_val) highest_val = raw_val;

    if(pin == 5)                  // skip pin D6 (used) 
      pin += 2;
  }
}

//--------------Calibration function for turn sensors-----------------------
//-> separate calibration function for turn sensors
//->this function sets new thresh values as calibration is underway
void CloudRunner::calibrate_turn_sensors(){
  int L_thresh = 0, R_thresh = 0;
  int L_raw_val = 0;
  int R_raw_val = 0;
  float thresh_percent = 0.5;

  //Reading raw_values of sensor pins
  L_raw_val = read_sensor(L_TURN_PIN)/32;   //read sensors for raw data 
  R_raw_val = read_sensor(R_TURN_PIN)/32;   //read sensors for raw data 
  

  //----------------------Setting calibration constants for Left turn sensor------------------- 
  if(L_turn_lowest_val > L_raw_val) L_turn_lowest_val = L_raw_val;     //get values for normalizing Left turn sensor
  if(L_turn_highest_val < L_raw_val) L_turn_highest_val = R_raw_val;   

  //Setting thresholds values based on 70% of highest mapped value
  L_thresh = thresh_percent *  map(L_raw_val,L_turn_lowest_val,L_turn_highest_val,1,100);  
  //if new threshold if bigger than existing
  if( L_thresh > L_onblk_thresh) L_onblk_thresh = L_thresh;
      
  //----------------------Setting calibration constants for Right turn sensor---------------
  if(R_turn_lowest_val > R_raw_val) R_turn_lowest_val = R_raw_val;     //get values for normalizing Right turn sensor
  if(R_turn_highest_val < R_raw_val) R_turn_highest_val = R_raw_val;

  //Setting thresholds values based on 70% of highest mapped value
  R_thresh = thresh_percent *  map(R_raw_val,R_turn_lowest_val,R_turn_highest_val,1,100);  
  //if new threshold if bigger than existing
  if( R_thresh > R_onblk_thresh) R_onblk_thresh = R_thresh;
    
  
}

//----------------Function to Get Line Position----------------------
//-->Determines line position after sensor is read
int CloudRunner::getpos() {
  int raw[SENSOR_NUM]= {0};   //contains raw values
  
  for(int i=0, pin= START_SENSOR_PIN; i < SENSOR_NUM;i++,pin++){
    raw[i] = read_sensor(pin)/32;   //read sensors for raw data     
   
    if(pin == 5)pin += 2;      // If using all sensors, skip D6 (used) 
      
  }
  //Zero out existing Line Position Variables
  mass=0;
  torque=0;
  centroid=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < SENSOR_NUM; i++){  //print values
    mass += raw[i];
    torque += (raw[i] * i); //Torque = force + lever arm
  }
  
  //Note this multiplier may need changing based on the board & surface's behavior
  centroid = (torque * 100)/ mass;

  return centroid;
}


//-----------------Function to check Left and right turns---------------
//-> Detect turns based on crossing of the threshold set by calibration
void CloudRunner::check_turn(){
  int R_val =0, L_val=0;  //mapped raw values

  R_val = 100-map(read_sensor(R_TURN_PIN)/32,R_turn_lowest_val,R_turn_highest_val,1,100);
  L_val = 100-map(read_sensor(L_TURN_PIN)/32,L_turn_lowest_val,L_turn_highest_val,1,100);


  //turn detection logic
  if(R_val > R_onblk_thresh && L_val < L_onblk_thresh){
    R_turn_detected = true;
    L_turn_detected = false;
    Intersect_detected = false;
  }else if(R_val < R_onblk_thresh && L_val > L_onblk_thresh){
    L_turn_detected = true;
    R_turn_detected = false;
    Intersect_detected = false;
  }else if(R_val > R_onblk_thresh && L_val > L_onblk_thresh && mass > 35){
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
  analogWrite(L_SPEED_PIN, 100);
  analogWrite(R_SPEED_PIN, 100);
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
    default:
      break;
  }
}

//----------------------Function to steer motors using PID------------------
//-> modulates speed based on calculated PID value
void CloudRunner::PID_steer(int p_PID_val) {
  // Calculating the effective motor speed:
  float Lspeed = INIT_SPEED + p_PID_val;
  float Rspeed = INIT_SPEED - p_PID_val;

  // The motor speed should not exceed the max PWM value
  Lspeed = constrain(Lspeed, 0, 100);
  Rspeed = constrain(Rspeed, 0, 100);

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
  old_error = p_error_val;
  int PID_val = P*Kp + I*Ki + D*Kd;
  return PID_val;
}

//----------------------Function to follow line using PID steering------------------
//-> avoid latching by reseting all turn flags
void CloudRunner::follow_line(){
  //Initialize local PID variables to 0
  int pos=0, error = 0, PID_val = 0;

  //Endlessly follow line until Intersection, Turn or End of Maze
  while(1){
    //Read motors and get the position of the line
    pos = getpos();
    
    if(mass > 500){
      return;    //Reached end of maze
    }

    //Calculate error based on current position 
    error = pos-10;

    //Calculate PID value based on error
    PID_val = PID_calc(error);

    //check for turns & intersections
    check_turn();

    //if junction detected, just move forward
    //and skip PID steer
    if(Intersect_detected){
      drive_motor(FORWARD_MOTORS);
      delay(500);
    }else{
      //if turn isnt detected continue following line
      //Use calculated PID value
      PID_steer(PID_val);
    }

    //after checking for turns reset all flags
    reset_turn_detect();
  }
  
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

int CloudRunner::get_Kp(){
  return Kp;
}

int CloudRunner::get_Kd(){
  return Kd;
}

int CloudRunner::get_Ki(){
  return Ki;
}
