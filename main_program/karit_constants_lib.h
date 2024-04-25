//GPIO assignment
#define L_speed_pin 5 //D5
#define R_speed_pin 6 //D6
#define L_forward 14  //A0
#define L_backward 15 //A1
#define R_forward 16  //A2
#define R_backward 17 //A3

//Constants for driveMotor functions
#define stop_motors 0
#define forward_motors 1
#define reverse_motors 2
#define turnleft  21
#define turnright 22
#define turnback 23 

//Constants for PID functions
#define init_speed 50


//Constants for sensor functions/routines
#define sensor_num 3        //Number of PID sensors (3)
#define start_sensor_pin 7  //Right most PID Sensor pin

#define L_turn_pin 8        //Left turn sensor pin
#define R_turn_pin 2        //Right turn sensor pin

//Tresholds and Calibration values for sensors

int L_turn_lowest_val = 100, L_turn_highest_val=0; //normalizing constants for Left turn sensor
int R_turn_lowest_val = 100, R_turn_highest_val=0; //normalizing constants for Right turn sensors
int R_onblk_thresh=0;    //threshold value for Right turn detection
int L_onblk_thresh=0;    //threshold value for Left turn detection

int lowest_val = 100, highest_val =0; //normalizing constants for PID sensors


//Global Variables for Line Tracing
int mass=0;
int torque=0;
int centroid=0;   

//Boolean for maze_end
boolean maze_end = false;
boolean L_turn_detected = false;
boolean R_turn_detected = false;
boolean Intersect_detected = false;
