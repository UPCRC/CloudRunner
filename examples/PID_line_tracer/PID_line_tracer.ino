
#include <cloudrunner.h>
CloudRunner board;


void setup() {
  //Initialize board and motors
  board.begin();

  //Set the PID constants here
  board.set_Kp(8);
  board.set_Kd(4);
  board.set_Ki(0); // if mabilis tanggaling si ki 
  

  //Set your ideal center position here (for the line)
  board.set_target_pos(50);

  //Set the other constants here, this should help you
  // solve any problems with the get pos
  board.set_torque_multiplier(50);
  

  Serial.begin(9600);
  Serial.println("Start Calibration");
  board.calibrate_turn_sensors();
  Serial.println("Done Calibration");

  
}

void loop() {
  board.drive_motor(FORWARD_MOTORS);
  delay(30);
  board.follow_line();


  
}
