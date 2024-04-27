
#include "CloudRunner.h"
CloudRunner board;
void setup() {
  //Initialize board and motors
  board.begin();

  //Beep motors twice
  board.motor_quartersec_beep(2);
  //Manually Calibrate for 10s
  for(int i = 0; i<10; i++){
    board.calibrate_sensors();
    board.calibrate_turn_sensors();
    delay(1000);
  }

  //Beep motors thrice
  board.motor_quartersec_beep(3);

  //Wait 5s and beep every second
  for(int i=0; i<5; i++){
    board.beep_motor();
    delay(1000);
  }
}

void loop() {
  board.follow_line();
}
