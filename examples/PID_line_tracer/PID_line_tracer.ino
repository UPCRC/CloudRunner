
#include <CloudRunner.h>
CloudRunner board;
void setup() {
  //Initialize board and motors
  board.begin();
  Serial.begin(9600);
  Serial.println("Start Calibration");
  //Manually Calibrate for 10s
  for(int i = 0; i<5; i++){
    board.calibrate_sensors();
    delay(500);
    Serial.println(i);
  }
  Serial.println("Done Calibration");
  
  while(1){
    Serial.println(board.getpos());
    
  }
}

void loop() {
  board.follow_line();
}
