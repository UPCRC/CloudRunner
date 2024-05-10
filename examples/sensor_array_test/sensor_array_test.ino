
#include <CloudRunner.h>
#include <constants.h>
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

 //If you want to change the default configuration
 //you can put it here


  //This portion of the code starts the test
  Serial.println("===== Start of calibration =====");
  //Manually Calibrate for 5s
  for(int i = 0; i<5; i++){
    board.calibrate_PID_sensors();
    delay(500);
    Serial.println(i);
  }
  Serial.println("===== END of calibration =====");
  delay(3000); // wait 3s to start the test
  Serial.println(" -- STARTING TEST -- ");
}

void loop() {
  //Instructions: 
  //Use a white strip of paper with a 1cm thick line to simulate a competition surface
  //then move the black line back and forth and observe if the value for the position changes as expected
  //i.e what value is it on the left most and right mose
  test_get_pos(); 

  //Uncomment this to use normalizing feature , note this assumes you had 
  //sucessfully called calibrate_PID_sensor()
  // test_get_normalize_pos();
}



//Use this test to see if you're getting correct values for the line position caluclation. 
// for this version of the get_pos(), you dont need to use calibrate_PID_sensors()
// this is helpful for debugging follow_line() function
void test_get_pos(){
  Serial.println(board.get_pos());  
}

//Use this test to see if you're getting correct values for the line position caluclation USING the normalized values
// this implies that you successfully used the calibrate_PID_sensors() method 
// this is helpful for debugging follow_line() function
void test_get_normalize_pos(){
  Serial.println(board.get_norm_pos());  
}

//Use this test to print out raw values and diagnose any problems with getpos() function
void test_read_sensor(){
  //Get sensor values
  int raw[SENSOR_NUM]= {0};   //contains normalized data
  
  for(int i=0, pin= START_SENSOR_PIN; i < SENSOR_NUM;i++,pin++){
    raw[i] = board.read_sensor(pin)/32;   //read sensors for raw data     
   
    if(pin == 5)pin += 2;      // If using all sensors, skip D6 (used) 
    Serial.print(raw[i]);
    Serial.print(" ");      
  }

  
  //Zero out existing Line Position Variables
  int mass=0;
  int torque=0;
  int pos=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < SENSOR_NUM; i++){  //print values
    mass += raw[i];
    torque += (raw[i] * i); //Torque = force + lever arm
  }
  
  pos = (torque * board.get_torque_multiplier())/ mass;

  Serial.print("Centroid: ");
  Serial.print(pos);
  Serial.print(" Torque: ");
  Serial.print(torque);
  Serial.print(" Mass ");
  Serial.print(mass);
  Serial.print("\n");
  
  delay(100);
}
