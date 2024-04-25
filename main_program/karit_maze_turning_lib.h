// all delays based on 8.0V input to robot
// all functions are not yet tested


//-------------General function to operate the motors-------------
//-> interfaces with motor driver via pwm
void driveMotor(int d){
  analogWrite(L_speed_pin, 100);
  analogWrite(R_speed_pin, 100);
  switch(d){
    case stop_motors:
      digitalWrite(L_forward,LOW);
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,LOW);
      break;
    case forward_motors:
      digitalWrite(L_forward,HIGH);
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
      break;
    case reverse_motors:
      digitalWrite(L_forward,LOW);
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);
      break;
    case turnleft:  
      digitalWrite(L_forward,LOW);    //reverse left motor to face 90 degrees left
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
      delay(300);
      digitalWrite(L_forward,HIGH);   //break by momentarilly reversing right motor
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);
      delay(20);
      digitalWrite(L_forward,LOW);    //stop motors
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,LOW);
      break;
    case turnright:
      digitalWrite(L_forward,HIGH);   //reverse right motor to face 90 degrees right
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);
      delay(300);
      digitalWrite(L_forward,LOW);    //break by momentarily reversing left moror
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
      delay(20);
      digitalWrite(L_forward,LOW);    //stop motors
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,LOW);
      break;
    case turnback:
      digitalWrite(L_forward,LOW);    //rotate 180 degrees by turning right ( reversing left motor)
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
      delay(700);
      digitalWrite(L_forward,HIGH);   //break by momentarily reversing right motor
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);
      delay(50);
      digitalWrite(L_forward,LOW);    //stop motors
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,LOW);
    default:
      break;
  }
}



void slowturn(char t){
  switch(t){
    case turnright:
      digitalWrite(L_forward,HIGH);   //reverse right motor
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);
      delay(50);
      digitalWrite(L_forward,LOW);    //break by momentarily turning right
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
      delay(20);
      digitalWrite(L_forward,LOW);    //stop motors
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,LOW);
      break;
    case turnleft:
      digitalWrite(L_forward,LOW);    //reverse left motor
      digitalWrite(L_backward,HIGH);
      digitalWrite(R_forward,HIGH);
      digitalWrite(R_backward,LOW);
      delay(50);
      digitalWrite(L_forward,HIGH);   //break by momentarily turning left 
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,HIGH);
      delay(20);
      digitalWrite(L_forward,LOW);    //stop motors
      digitalWrite(L_backward,LOW);
      digitalWrite(R_forward,LOW);
      digitalWrite(R_backward,LOW);
      break;
  }
}


//---------------Function to Re-center robot on line-------
//-> Note: this is on the assumption that atleast 1 of the 3 PID line sensors
//-> is on top of the line, if not callibrate start_calibration function as required
void reposition(){

  int pos = getpos();
  while(1){
    if(pos > 10  || (mass < 35 && torque < 31))slowturn(turnright); 
    else if(pos < 9  )slowturn(turnleft);
    
    pos = getpos(); 
    if(pos >= 9 && pos <= 10 && mass > 35 && torque > 31) break;
  }
}


//--------------------------Function to use DC motors as mini buzzers-------------------
//-> Use this function to provide feedback and make debugging easier
void beep(){
  
  int amplitude = 255;      //PWM value controls amplitude of beep
  int oscillation_delay_us = 500;  

  //stop motors
  digitalWrite(L_forward,LOW);
  digitalWrite(L_backward,LOW);
  digitalWrite(R_forward,LOW);
  digitalWrite(R_backward,LOW);
  analogWrite(L_speed_pin,0);
  analogWrite(R_speed_pin,0);

  
  //oscillate motors to achieve beep
  for( int i = 0; i < 20; i++){

    
    analogWrite(L_speed_pin,amplitude);
    analogWrite(R_speed_pin,amplitude);
    
       //Run pins motor forward
    digitalWrite(L_forward,HIGH);
    digitalWrite(L_backward,LOW);
    digitalWrite(R_forward,HIGH);
    digitalWrite(R_backward,LOW);
    delayMicroseconds(oscillation_delay_us);
      //Run pins motor backwards
    digitalWrite(L_forward,LOW);
    digitalWrite(L_backward,HIGH);
    digitalWrite(R_forward,LOW);
    digitalWrite(R_backward,HIGH);
    delayMicroseconds(oscillation_delay_us);
  }

  analogWrite(L_speed_pin,0);
  analogWrite(R_speed_pin,0);
}


//----------------Some Functions to make half second beeps----------
//-> Use motors as buzzers for easier debugging
void motor_halfsec_beep(int num_beep){
  for(int i = 0;i < num_beep;i++){
    beep();
    delay(500);
  }
}

void motor_quartersec_beep(int num_beep){
  for(int i = 0;i < num_beep;i++){
    beep();
    delay(250);
  }
}

//-----------------Post maze learning turning----------------
void select_turn(char turn){
    switch(turn){
      case 'L':
        driveMotor(turnleft);
        break;
      case 'R':
        driveMotor(turnright);
        break;
      case 'B':
        driveMotor(turnback);
        break;
      case 'S':
        driveMotor(forward_motors);
        break;
      default:
        break;
    }
}
