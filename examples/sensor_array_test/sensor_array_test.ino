
#include <CloudRunner.h>
#include <constants.h>
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
  delay(3000); // wait 3s to start the test
  Serial.println("STARTING TEST");
}

void loop() {
  //Instructions: 
  //Use a white strip of paper with a 1cm thick line to simulate a competition surface
  //then move the black line back and forth and observe if the value for the position changes as expected
  //i.e what value is it on the left most and right mose
  test_get_pos();  
}


//Use this test to see if you're getting correct values for 
// the line position caluclation. This is helpful for debugging follow_line() function
void test_get_pos(){
  Serial.println(board.getpos());  
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
  int centroid=0;
  
  //Calculate Position of Line using Centroid method by Kirk Charles
  //For explanation refer to: https://www.youtube.com/watch?v=RFYB0wO9ZSQ&t=1217s
  for(int i=0; i < SENSOR_NUM; i++){  //print values
    mass += raw[i];
    torque += (raw[i] * i); //Torque = force + lever arm
  }
  
  centroid = torque * 100/ mass;

  Serial.print("Centroid: ");
  Serial.print(centroid);
  Serial.print(" Torque: ");
  Serial.print(torque);
  Serial.print(" Mass ");
  Serial.print(mass);
  Serial.print("\n");
  
  delay(100);
}
