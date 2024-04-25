


void PID_steer(int PID_val) {
  // Calculating the effective motor speed:
  float Lspeed = init_speed + PID_val;
  float Rspeed = init_speed - PID_val;

  // The motor speed should not exceed the max PWM value
  Lspeed = constrain(Lspeed, 0, 100);
  Rspeed = constrain(Rspeed, 0, 100);

  analogWrite(L_speed_pin, Lspeed); //Left Motor Speed
  analogWrite(R_speed_pin, Rspeed); //Right Motor Speed
  //following lines of code are to make the bot move forward
  
  digitalWrite(L_forward, HIGH);
  digitalWrite(L_backward, LOW);
  digitalWrite(R_forward , HIGH);
  digitalWrite(R_backward, LOW);
}



void follow_line(){

  //Initiate PID constants to 0
  int pos=0,P = 0 , I=0, D= 0 , old_D=0  , error=0 , old_error=0 , PID_val=0;
  float Ki = 0, Kp =20 , Kd =18 ;

  //Endlessly follow line until Intersection, Turn or End of Maze
  while(1){
    pos = getpos();
   
    if(mass > 500){
      maze_end = true;
      return;    //Reached end of maze
    }
    
    
    error = pos-10;
    P = error;
    D = error - old_error;
    I += P;
    old_error = error;
  
    PID_val = P*Kp + I*Ki + D*Kd;


    //check for turns 
    check_turn();

    //if turns are detected perform mazelogic
    if(Intersect_detected ){
      driveMotor(stop_motors);
      delay(500);
      motor_quartersec_beep(4);
      break;
    }else if(L_turn_detected){
      select_turn('L');
      //driveMotor(stop_motors);
      //delay(500);
      //motor_quartersec_beep(3);
    }else if(R_turn_detected){
      driveMotor(stop_motors);
      delay(500);
      motor_quartersec_beep(2);
    }

    //after checking for turns  reset all flags
    reset_turn_detect();
    //if turn isnt detected continue following line
    PID_steer(PID_val);
  }
  
}
