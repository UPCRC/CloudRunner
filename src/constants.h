#ifndef CLOUDRUNNER_CONSTANTS_H
#define CLOUDRUNNER_CONSTANTS_H

//GPIO assignment
#define L_SPEED_PIN 10 //D10
#define R_SPEED_PIN 9 //D9
#define L_FORWARD A0  //A0
#define L_BACKWARD A1 //A1
#define R_FORWARD A2  //A2
#define R_BACKWARD A3 //A3

//Constants for driveMotor functions
#define STOP_MOTORS 0
#define FORWARD_MOTORS 1
#define REVERSE_MOTORS 2
#define TURNLEFT  21
#define TURNRIGHT 22
#define TURNBACK 23 

//Constants for PID functions
#define INIT_SPEED 50


//Constants for sensor functions/routines
#define SENSOR_NUM 3        //Number of PID sensors (3)
#define START_SENSOR_PIN 4  //Right most PID Sensor pin

#define L_TURN_PIN 8        //Left turn sensor pin
#define R_TURN_PIN 2        //Right turn sensor pin

#endif