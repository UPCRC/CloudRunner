//This code is for testing Karit v2 sensor array 
//Written by: Alfred Abanto 14/07/2019

#define sensor_num 3
#define start_sensor_pin 7 //(Right most PID sensor)
#define L_turn_pin 8            // Left sensor for turn detection
#define R_turn_pin 2           // Right sensor for turn detection

int lowest_val = 100, highest_val =0; //normalizing constants
int L_turn_lowest_val = 100, L_turn_highest_val =0; //normalizing constants for Left turn sensor
int R_turn_lowest_val = 100, R_turn_highest_val =0; //normalizing constants for Right turn sensor


int mass = 0, torque=0,centroid = 0; //for line tracking
int L_onblk_thresh =0, R_onblk_thresh=0; //for turn detection

void setup() {
  Serial.begin(9600);  
}

void loop() {
  
  test_PID_sensors();
  //test_elbow_sensors();
}

//-----------------------Function for reading and testing elbow sensors---------------------------
//-> note: currently the threshold setting is set at the same time as calibration which causes problem
//-> it is advisable that you commentout threshold setting portion
//-> also it is advisable to uncomment out value serial.print values
void test_elbow_sensors(){
   int raw[2]= {0};   //contains raw values
  int vals[2]= {0};   //contains normalized data

  //Read Left and right turn sensors
  raw[0] = read_sensor(L_turn_pin);
  raw[1] = read_sensor(R_turn_pin);

  //Callibrate turn sensors
  for(int i=0 ;i<2;i++){
    if(i == 1){
      if(R_turn_lowest_val > raw[i]) R_turn_lowest_val == raw[i];     //get values for Right normalizing
      if(R_turn_highest_val < raw[i]) R_turn_highest_val == raw[i];  
    }else if(i==0){
      if(L_turn_lowest_val > raw[i]) L_turn_lowest_val == raw[i];     //get values for Left normalizing
      if(L_turn_highest_val < raw[i]) L_turn_highest_val == raw[i];    
    }
  }
  
  //Normalize raw values
  for(int i =0; i < 2;i++){
    if(i == 1){
      vals[i]= 100 - map(raw[i],R_turn_lowest_val,R_turn_highest_val,1,100);  //Invert values since white background

      //after mapping set threshold value to 70% of maximum mapped value
      if( (0.7 * vals[i]) > R_onblk_thresh) R_onblk_thresh = 0.7 * vals[i];
    
    }else if( i ==0){
      vals[i]= 100 - map(raw[i],L_turn_lowest_val,L_turn_highest_val,1,100);  //Invert values since white background
      //after mapping set threshold value to 70% of maximum mapped value
      if( (0.7 * vals[i]) > L_onblk_thresh) L_onblk_thresh = 0.7 * vals[i];
    }
  }

  //Portion which prints out turn sensor values
  
  Serial.print(" Left turn:");
  Serial.print(vals[0]);
  //Serial.print(" Thresh L:");
  //Serial.print(vals[0] * 0.6);
  Serial.print(" Right turn:");
  Serial.print(vals[1]);
  //Serial.print(" Thresh R:");
  //Serial.print(vals[1] * 0.6);

  
  
  
  //Turn detection ,uncomment if you only want to print out vals
  /*
  if(vals[1] > R_onblk_thresh && vals[0] < L_onblk_thresh)
    Serial.print(" Right turn detected");
  else if(vals[1] < R_onblk_thresh && vals[0] > L_onblk_thresh)
    Serial.print(" Left turn detected");
  else if(vals[1] > R_onblk_thresh && vals[0] > L_onblk_thresh)
    Serial.print(" intersection detected");
  else
    Serial.print(" no detection");
  */

  Serial.print("\n");
}





//-----------------------------------Function for Reading center 3 PID sensors-------------------
void test_PID_sensors(){
  int raw[sensor_num]= {0};   //contains raw values
  int vals[sensor_num]= {0};   //contains normalized data
  
  
  for(int i=0, pin= start_sensor_pin; i < sensor_num;i++,pin--){
    raw[i] = read_sensor(pin)/32;   //read sensors
    
    if(lowest_val > raw[i]) lowest_val == raw[i];     //get values for normalizing
    if(highest_val < raw[i]) highest_val == raw[i];   
    
    if(pin == 7) pin -= 2;                 // If using all sensors,skip pin D5 and D6 (used for Motor pins) 
      
  }


  //Normalize raw values
  for(int i =0; i < sensor_num;i++)
    vals[i]= 100 -  map(raw[i],lowest_val,highest_val,1,100);  //Invert values since white background
  
  //Zero out Line Position Variables
  mass =0;
  torque=0;
  centroid=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < sensor_num; i++){  //print values
    mass += vals[i];
    torque += (vals[i] * i);
    Serial.print(vals[i]);
    Serial.print(" ");
  }

  centroid = torque*10 / mass;
  
  Serial.print(" Mass: ");
  Serial.print(mass);
  Serial.print(" ");
  Serial.print("Centroid: ");
  Serial.print(centroid);
  Serial.print(" ");
  Serial.print("Torque: ");
  Serial.print(torque);
  test_elbow_sensors();
  Serial.print("\n");
}






//-------------------------Sensor Reading code-----------------------------
//This code works for sensor circuit based on 3pi robot by Pololu 
//For circuit explanation Refer to : https://www.youtube.com/watch?v=9XjSJV5MPc0&t=543s
int read_sensor(int sensor_pin){  
  pinMode(sensor_pin,OUTPUT);
  digitalWrite(sensor_pin,HIGH);
  long val = 0;
  delay(1);
  pinMode(sensor_pin,INPUT);

  while(digitalRead(sensor_pin) == HIGH)
    val++;
    
  return val;
}
