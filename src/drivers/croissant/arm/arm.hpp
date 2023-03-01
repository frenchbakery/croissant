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
        const int max_y = 1350;


    public:
        Arm(
            int motor_port,
            int left_servo_port,
            int right_servo_port,
            int end_switch_port
        );

        void calibrate();
        void setYPerc(float percent);
};
