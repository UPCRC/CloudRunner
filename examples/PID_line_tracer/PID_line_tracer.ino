
#include <cloudrunner.h>
CloudRunner board;

void setup() {
  Serial.begin(9600);
  //Initialize board and motors
  board.begin();

  //Set your manual configuration for the motors
  board.set_L_spd_offset(0); // Set an offset in the motor's speed 
  board.set_R_spd_offset(0); // if their actual rpm does not match
  board.set_INIT_spd(100);    // Initial speed
  board.set_LB_spd(0);       // Lower Bound speed
  board.set_UB_spd(255);      // Upper Bound speed

  //Set the PID constants here
  board.set_Kp(4);
  board.set_Kd(3);
  board.set_Ki(0.01); 
  
  //Set your ideal center position here (for the line)
  board.set_target_pos(101);

  //Set the other constants here, this should help you
  // solve any problems with the get pos
  board.set_torque_multiplier(100);
  
  //This portion is used for detecting intersection
  //This portion is skipped until further checking of functionalities

  Serial.println("Start Calibration");
  board.calibrate_PID_sensors();
  delay(10);
  board.calibrate_turn_sensors();
  Serial.println("Done Calibration");
  delay(5000);
  
}

void loop() {
  board.follow_line();
}
