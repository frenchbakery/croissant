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

        // settings
        const int max_y = 1350;  // max motor position count
        const float y_cm = 9.6;  // real life measurement
        const int l_initial = 600;
        const int r_initial = 1448;

        /**
         * @brief turn of all motors / servos and block in an infinite loop
         * 
         */
        void shutdownAndBlock();

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
        float getYPerc();

        /**
         * @brief set the arm height in cm (0...down, 9.6...up)
         * 
         * @param cm value between 0 and 9.6
         */
        void setYcm(float cm);
        float getYcm();

        void grab(int percent);

        void tilt(int angle);
};
