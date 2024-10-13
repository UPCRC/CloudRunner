

#include <cloudrunner.h>
CloudRunner board;

void setup() {
  Serial.begin(9600);
  //Initialize board and motors
  board.begin();
  //Set the intersection boolean and intersection count
  board.set_with_intersection(true, 3);

  //Set your manual configuration for the motors
  board.set_L_spd_offset(0); // Set an offset in the motor's speed 
  board.set_R_spd_offset(0); // if their actual rpm does not match
  board.set_INIT_spd(100);    // Initial speed
  board.set_LB_spd(0);       // Lower Bound speed
  board.set_UB_spd(255);      // Upper Bound speed

  //Set the PID constants here
  board.set_Kp(4);
  board.set_Kd(3);
  board.set_Ki(0); 
  
  //Set your ideal center position here (for the line)
  board.set_target_pos(101);
  
  //Set the other constants here, this should help you
  // solve any problems with the get pos
  board.set_torque_multiplier(100);
  
  delay(1000);

  //sensor calibration if with intersection 
  if(board.get_with_intersection()){

    Serial.println("Start Calibration");
    board.calibrate_PID_sensors();
    delay(10);
    board.calibrate_turn_sensors();
    Serial.println("Done Calibration");
    delay(5000);

  }

}

void loop() {

  int pos =0 , error =0 , PID_val =0;
  pos = board.get_pos();                  //find the current position

  error = pos - board.get_target_pos();   //find the error
  PID_val = board.PID_calc(error);        //find the PID value


  if(board.get_with_intersection()){

    board.check_turn();                   //check if intersection detected

    if(board.intersection_detected()){    // if there's an intersection drive for xxx ms and increment count

      board.drive_motor(FORWARD_MOTORS);
      delay(100);
      board.set_count(board.get_count() + 1);

    }

    board.reset_turn_detect();            //reset detection booleans

    if(board.get_count() >= board.get_intersectionCount()){   // check if the robot should stop after x intersection detected

      board.drive_motor(STOP_MOTORS);
      delay(10000000);

    }
  }
  
  board.PID_steer(PID_val);               //adjust motor speed and runs the robot

}
