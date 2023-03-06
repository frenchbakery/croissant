/**
 * @file arm.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <kiprplus/smooth_servo.hpp>
#include <kipr/digital/digital.hpp>
#include <kiprplus/pid_motor.hpp>


class Arm
{
    protected:
        kp::PIDMotor y_motor;
        kp::SmoothServo l_servo;
        kp::SmoothServo r_servo;
        kipr::digital::Digital esw;
        float current_angle = 90;
        int grab_current = 0;

        // settings
        const int max_y = 1350;  // max motor position count
        const float y_cm = 9.6;  // real life measurement
        const int l_initial = 1125;
        const int r_initial = 725;
        const int max_grab = 300;

        /**
         * @brief turn of all motors / servos and block in an infinite loop
         * 
         */
        void shutdownAndBlock(char *reason = "");

    public:
        Arm(
            int motor_port,
            int left_servo_port,
            int right_servo_port,
            int end_switch_port
        );

        /**
         * @brief set initial motor position and calibrate range
         * 
         */
        void calibrate();

        /**
         * @brief set the arm height in percent (0...down, 100...up)
         * 
         * @param percent value between 0 and 100
         */
        void setYPerc(float percent);

        /**
         * @brief current y position in percent
         * 
         * @return float - 0..100
         */
        float getYPerc();

        /**
         * @brief set the arm height in cm (0...down, 9.6...up)
         * 
         * @param cm value between 0 and 9.6
         */
        void setYcm(float cm);

        /**
         * @brief get current y position in cm
         * 
         * @return float - 0..9.6
         */
        float getYcm();

        void waitForY();

        /**
         * @brief close the grabber (0...open 100...closed)
         * 
         * @param percent 0..100
         */
        void grab(int percent);

        void waitForGrab();

        /**
         * @brief tilt the grabber in degrees
         * 
         * @param angle 0...90
         */
        void tilt(int angle);

        void waitForTilt();

        /**
         * @brief get the current grabber tilt in degrees
         * 
         * @return float - 0..90
         */
        float getTilt();

        // void setYSpeed(int speed);  // melektron hasn't speeed for PIDMotors yet

        /**
         * @brief set the servo speeds
         * 
         * @param speed in servo ticks per second
         */
        void setServoSpeed(int speed);
};
