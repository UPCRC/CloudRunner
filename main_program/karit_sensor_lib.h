//This code is for testing Karit v2 sensor array 
//Written by: Alfred Abanto 14/07/2019




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


//------------------Calibration function for PID sensors----------------------
//-->Records lowest and highest values of sensor values during initiation phase (only for PID sensors)
void calibrate_sensors(){
  
  for(int i=0, raw_val=0, pin= start_sensor_pin; i < sensor_num;i++,pin--){
    raw_val = read_sensor(pin)/32;   //read sensors for raw data     

    if(lowest_val > raw_val) lowest_val == raw_val;     //get values for normalizing
    if(highest_val < raw_val) highest_val == raw_val;

    if(pin == 7)                  // skip pin D5 and D6 (used) 
      pin -= 2;
  }
}


//--------------Calibration function for turn sensors-----------------------
//-> separate calibration function for turn sensors
//->this function sets new thresh values as calibration is underway
void calibrate_turn_sensors(){
  int L_thresh = 0, R_thresh = 0;
  int L_raw_val = 0;
  int R_raw_val = 0;
  float thresh_percent = 0.5;

  //Reading raw_values of sensor pins
  L_raw_val = read_sensor(L_turn_pin)/32;   //read sensors for raw data 
  R_raw_val = read_sensor(R_turn_pin)/32;   //read sensors for raw data 
  

  //----------------------Setting calibration constants for Left turn sensor------------------- 
  if(L_turn_lowest_val > L_raw_val) L_turn_lowest_val == L_raw_val;     //get values for normalizing Left turn sensor
  if(L_turn_highest_val < L_raw_val) L_turn_highest_val == R_raw_val;   

  //Setting thresholds values based on 70% of highest mapped value
  L_thresh = thresh_percent *  map(L_raw_val,L_turn_lowest_val,L_turn_highest_val,1,100);  
  //if new threshold if bigger than existing
  if( L_thresh > L_onblk_thresh) L_onblk_thresh = L_thresh;
      
  //----------------------Setting calibration constants for Right turn sensor---------------
  if(R_turn_lowest_val > R_raw_val) R_turn_lowest_val == R_raw_val;     //get values for normalizing Right turn sensor
  if(R_turn_highest_val < R_raw_val) R_turn_highest_val == R_raw_val;

  //Setting thresholds values based on 70% of highest mapped value
  R_thresh = thresh_percent *  map(R_raw_val,R_turn_lowest_val,R_turn_highest_val,1,100);  
  //if new threshold if bigger than existing
  if( R_thresh > R_onblk_thresh) R_onblk_thresh = R_thresh;
    
  
}


//---------------Calibration Routine (Initiation Phase)---------------
//--> Robot Movement routine to calibrate sensor values
void start_calibrate(){
  
  analogWrite(L_speed_pin, 50);
  analogWrite(R_speed_pin, 50);  
  
  for(int i = 0 ; i < 85 ; i++){
    
    if( i <= 25 || i > 65 ){ 
      digitalWrite(L_forward , HIGH);
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);    
    }else if(i > 25 && i <= 65){
      digitalWrite(L_forward,LOW);
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
    }

    
    calibrate_sensors();
    calibrate_turn_sensors();
    delay(11);
  }

  
  digitalWrite(L_forward,LOW);
  digitalWrite(L_backward,LOW);
  digitalWrite(R_forward,LOW);
  digitalWrite(R_backward,LOW);

}




//----------------Function to Get Line Position----------------------
//-->Determines line position after sensor is read
int getpos() {
  int raw[sensor_num]= {0};   //contains raw values
  int vals[sensor_num]= {0};   //contains normalized data
  
  for(int i=0, pin= start_sensor_pin; i < sensor_num;i++,pin--){
    raw[i] = read_sensor(pin)/32;   //read sensors for raw data     
   
    if(pin == 7)pin -= 2;      // If using all sensors, skip pin D5 and D6 (used for motors) 
      
  }


  //Normalize raw values using calibration constants
  for(int i =0; i < sensor_num;i++)
    vals[i]= 100 - map(raw[i],lowest_val,highest_val,1,100);
  
  //Zero out existing Line Position Variables
  mass=0;
  torque=0;
  centroid=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < sensor_num; i++){  //print values
    mass += vals[i];
    torque += (vals[i] * i);
  }
  
  centroid = torque*10 / mass;

  return centroid;
}
