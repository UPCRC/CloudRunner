
#include <cloudrunner.h>
CloudRunner board;

void setup() {
  Serial.begin(9600);

  //Initialize board and motors
  board.begin();
  
  //Print out default configuration
  Serial.println("===== Start of configuration =====");
  Serial.print("Kp: ");
  Serial.print(board.get_Kp());
  Serial.print(" Kd: ");
  Serial.print(board.get_Kd()); 
  Serial.print(" Ki: ");
  Serial.print(board.get_Ki());
  Serial.print("\n");
  Serial.print("Torque multiplier: ");
  Serial.print(board.get_torque_multiplier());
  Serial.print(" Target position: ");
  Serial.print(board.get_target_pos());
  Serial.print("\n");
  Serial.println("===== END of configuration =====");
  delay(5000); //Delay for 5s

/*If you want to change the default configuration
  you can put it here
  board.set_Kp(int p_Kp);
  board.set_Kd(int p_Kd);
  board.set_Ki(int p_Ki);
  board.set_torque_multiplier(int p_multiplier);
  board.set_target_pos(int p_pos);
  */
    
  //This portion of the code starts the test

  /*This portion is skipped until further checking of functionalities

  Serial.println("===== Start of calibration =====");
  //Manually Calibrate for 5s
  board.calibrate_PID_sensors();

  Serial.println("===== END of calibration =====");
  delay(3000); // wait 3s to start the test
  */

  Serial.println(" -- STARTING TEST -- ");
}

//Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s

void loop() {
  //Instructions: 
  //Use a black strip of paper with a 1cm thick line to simulate a competition surface
  //then move the black line back and forth and observe if the value for the position changes as expected
  //i.e what value is it on the left most and right mose

  test_get_pos(); 
  print_raw_values();

  //Uncomment this to use normalizing feature , note this assumes you had 
  //sucessfully called calibrate_PID_sensor()
  // test_get_normalize_pos();
}


//Use this test to see if you're getting correct values for the line position caluclation. 
// for this version of the get_pos(), you dont need to use calibrate_PID_sensors()
// this is helpful for debugging follow_line() function
void test_get_pos(){
  Serial.print("Position: ");
  Serial.println(board.get_pos());  
}

//Use this test to retrieve raw values from the sensors
//for debugging purposes
void print_raw_values(){
  board.test_read_sensor();
}

//Use this test to see if you're getting correct values for the line position caluclation USING the normalized values
// this implies that you successfully used the calibrate_PID_sensors() method 
// this is helpful for debugging follow_line() function
void test_get_normalize_pos(){
  Serial.println(board.get_norm_pos());  
}


