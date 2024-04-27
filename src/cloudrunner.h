/*
  cloudrunner.h - Library for using the CloudRat board.
  Created by Alfred Jason R. Abanto, April 27, 2024.
  Released into the public domain.
*/

#ifndef CLOUDRUNNER_H
#define CLOUDRUNNER_H

#include <Arduino.h>
#include "constants.h"

class CloudRat{
    private:
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

        //Global Variables for PID
        int Ki = 0, Kp = 20 , Kd = 18 ;
        int P = 0 , I = 0, D = 0 , old_D = 0 , old_error = 0;

        //Boolean for maze_end
        boolean maze_end = false;
        boolean L_turn_detected = false;
        boolean R_turn_detected = false;
        boolean Intersect_detected = false;

        //PID related functions
        void PID_steer(int p_PID_val);
        int PID_calc(int p_error_val);
        void check_turn();
        void reset_turn_detect();
    public:

        //Initialize board function
        void begin();

        //Sensor utility functions
        int read_sensor(int p_sensor_pin);
        void calibrate_sensors();
        void calibrate_turn_sensors();
        int getpos();

        //Motor driver utility functions
        void beep_motor();
        void motor_quartersec_beep(int p_num_beep);
        void drive_motor(int p_direction);
        void follow_line();

        //PID constants Setter & Getter functions
        void set_Kp(int p_Kp);
        void set_Kd(int p_Kd);
        void set_Ki(int p_Ki);
        int get_Kp();
        int get_Kd();
        int get_Ki();


}

#endif