
#include <cloudrunner.h>
CloudRunner board;


void setup() {
  Serial.begin(9600);
  //Initialize board and motors
  board.begin();

  //Set the PID constants here
  board.set_Kp(16);
  board.set_Kd(4);
  board.set_Ki(0); 
  
  //Set your ideal center position here (for the line)
  board.set_target_pos(49);

  //Set the other constants here, this should help you
  // solve any problems with the get pos
  board.set_torque_multiplier(50);
  
  //This portion is used for detecting intersection
  /*This portion is skipped until further checking of functionalities

  Serial.println("Start Calibration");
  //board.calibrate_turn_sensors();
  Serial.println("Done Calibration");
  */
}

void loop() {
  board.follow_line();
}
