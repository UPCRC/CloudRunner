/*
  cloudrunner.h - Library for using the CloudRat board.
  Created by Alfred Jason R. Abanto, April 27, 2024.
  Updated by Neil Anthony D. Marmeto, October 13, 2024
  Released into the public domain.
*/

#ifndef CLOUDRUNNER_H
#define CLOUDRUNNER_H

#include <Arduino.h>
#include "constants.h"
class CloudRunner{
    private:
        //Tresholds and Calibration values for sensors
        int R_onblk_thresh=0;    //threshold value for Right turn detection
        int L_onblk_thresh=0;    //threshold value for Left turn detection

        //Array of normalizing constants for each PID sensor
        int LB_vals[SENSOR_NUM];
        int UB_vals[SENSOR_NUM];

        //Global Variables for Line Tracing
        float mass=0;
        float torque=0;
        float pos=0;
        int target_pos =101;
        int torque_multiplier=100;
        int fivepin[5] = {2,4,5,7,8};
        int threepin[3] = {4,5,7};
        int *pin;
        int count = 0;
        int intersectionCount =0;

        //Global variables for manual calibration
        int L_spd_offset =0;
        int R_spd_offset =0;
        int LB_spd = 0;
        int UB_spd = 255;
        int INIT_spd = 60;

        //Global Variables for PID
        float Ki = 0, Kp = 4 , Kd = 2 ;
        int P = 0 , I = 0, D = 0 , old_D = 0 , old_error = 0;

        //Boolean for maze_end
        boolean maze_end = false;
        boolean L_turn_detected = false;
        boolean R_turn_detected = false;
        boolean Intersect_detected = false;
        boolean with_intersection = false;

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

        //PID related functions
        void PID_steer(int p_PID_val);
        int PID_calc(int p_error_val);
        void check_turn();
        void reset_turn_detect();

        //Motor driver utility functions
        void beep_motor();
        void motor_quartersec_beep(int p_num_beep);
        void drive_motor(int p_direction);


        //PID constants Setter & Getter functions
        void set_Kp(int p_Kp);
        void set_Kd(int p_Kd);
        void set_Ki(int p_Ki);
        void set_count(int val);
        void set_L_spd_offset(int offset);
        void set_R_spd_offset(int offset);
        void set_LB_spd(int speed);
        void set_UB_spd(int speed);
        void set_INIT_spd(int speed);
        void set_with_intersection(boolean statement, int count);

        int get_Kp();
        int get_Kd();
        int get_Ki();
        int get_count();
        int get_L_spd_offset();
        int get_R_spd_offset();
        int get_LB_spd();
        int get_UB_spd();
        int get_INIT_spd();
        boolean get_with_intersection();
        int get_intersectionCount();
        boolean intersection_detected();


        //Utility functions to get the set target position
        //which is used in follow_line() to calculate the error value
        void set_target_pos(int p_pos);
        int get_target_pos();

        //Utility function to tune how we calculate the position of the line
        void set_torque_multiplier(int p_multiplier);
        int get_torque_multiplier();

};

#endif