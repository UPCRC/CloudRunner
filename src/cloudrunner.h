/*
  cloudrunner.h - Library for using the CloudRat board.
  Created by Alfred Jason R. Abanto, April 27, 2024.
  Released into the public domain.
*/

#ifndef CLOUDRUNNER_H
#define CLOUDRUNNER_H

#include <Arduino.h>
#include "constants.h"
class CloudRunner{
    private:
        //Tresholds and Calibration values for sensors
        int L_turn_lowest_val = 100, L_turn_highest_val=0; //normalizing constants for Left turn sensor
        int R_turn_lowest_val = 100, R_turn_highest_val=0; //normalizing constants for Right turn sensors
        int R_onblk_thresh=0;    //threshold value for Right turn detection
        int L_onblk_thresh=0;    //threshold value for Left turn detection

        //Array of normalizing constants for each PID sensor
        int LB_vals[SENSOR_NUM] = {100, 100, 100};
        int UB_vals[SENSOR_NUM] = {0, 0, 0};

        //Global Variables for Line Tracing
        int mass=0;
        int torque=0;
        int pos=0;
        int target_pos =52;
        int torque_multiplier=50;

        int count = 0;

        //Global Variables for PID
        float Ki = 0, Kp = 4 , Kd = 2 ;
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
        CloudRunner();

        //Initialize board function
        void begin();

        //Sensor utility functions
        int read_sensor(int p_sensor_pin);
        void test_read_sensor();
        void calibrate_PID_sensors();
        void calibrate_turn_sensors();
        int get_pos();
        int get_norm_pos();

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
        int get_count();

        //Utility functions to get the set target position
        //which is used in follow_line() to calculate the error value
        void set_target_pos(int p_pos);
        int get_target_pos();

        //Utility function to tune how we calculate the position of the line
        void set_torque_multiplier(int p_multiplier);
        int get_torque_multiplier();

};

#endif