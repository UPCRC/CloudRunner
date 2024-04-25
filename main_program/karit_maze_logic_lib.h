
//-----------------Function to check Left and right turns---------------
//-> Detect turns based on crossing of the threshold set by calibration
void check_turn(){
  int R_val =0, L_val=0;  //mapped raw values

  R_val = 100-map(read_sensor(R_turn_pin)/32,R_turn_lowest_val,R_turn_highest_val,1,100);
  L_val = 100-map(read_sensor(L_turn_pin)/32,L_turn_lowest_val,L_turn_highest_val,1,100);


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
void reset_turn_detect(){
  R_turn_detected = false;
  L_turn_detected = false;
  Intersect_detected = false;
}

//------------------function to reduce maze-------





//------------------function to solve------------
