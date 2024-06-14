
#include <cloudrunner.h>
CloudRunner board;
 //to stop code
int x = 1;

void setup() {
  //Initialize board and motors
  board.begin();

  //Set the PID constants here
  //board.set_Kp(120);
  board.set_Kp(140);
  //board.set_Kd(45);
  board.set_Kd(100);
  board.set_Ki(0);

  //Set your ideal center position here (for the line)
  board.set_target_pos(55);

  //Set the other constants here, this should help you
  // solve any problems with the get pos
  board.set_torque_multiplier(50);
  

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
  while(x){
    board.follow_line();
    x = 0;
  }
}
