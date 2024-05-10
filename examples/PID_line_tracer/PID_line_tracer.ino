
#include <CloudRunner.h>
CloudRunner board;
void setup() {
  //Initialize board and motors
  board.begin();

  //Set the PID constants here
  board.set_Kp(0);
  board.set_Kd(0);
  board.set_Ki(0);

  //Set your ideal center position here (for the line)
  board.set_target_pos(50);

  //Set the other constants here, this should help you
  // solve any problems with the get pos
  board.set_torque_multiplier(100);
  

  Serial.begin(9600);
  Serial.println("Start Calibration");
  //Manually Calibrate for 5s
  for(int i = 0; i<5; i++){
    board.calibrate_PID_sensors();
    delay(500);
    Serial.println(i);
  }
  Serial.println("Done Calibration");
  
}

void loop() {
  board.follow_line();
}
