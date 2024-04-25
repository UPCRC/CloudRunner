
#include "karit_constants_lib.h"
#include "karit_sensor_lib.h"
#include "karit_maze_logic_lib.h"
#include "karit_maze_turning_lib.h"
#include "karit_PID_steer_lib.h"

void setup() {

  //setup Motor control pins as output
  for(int i = 5; i<18; i++){
    pinMode(i,OUTPUT);
    if(i == 6) i+= 7;
  }

  //Calibrate sensors before initial run
  start_calibrate();
  reposition();
  motor_halfsec_beep(3); //beep 3 times
  motor_quartersec_beep(2);
  
}

void loop() {

  while(1){
    if(maze_end){
        driveMotor(stop_motors);
        
      //End of maze
    }else{
      follow_line();
    }
    
  }

}
