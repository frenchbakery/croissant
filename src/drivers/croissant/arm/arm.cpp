#include <kipr/time/time.h>
#include <iostream>
#include "arm.hpp"


Arm::Arm(int motor_port, int left_servo_port, int right_servo_port, int end_switch_port)
    : 
        y_motor(motor_port), 
        l_servo(left_servo_port, 600),
        r_servo(right_servo_port, 2048-600),
        esw(end_switch_port)
{
    l_servo.enable();
    r_servo.enable();
};


void Arm::calibrate()
{
    // wait for motor to reach end
    y_motor.moveAtVelocity(-100);
    while (!esw.value()) { msleep(10); };
    y_motor.off();

    y_motor.clearPositionCounter();
    y_motor.setAbsoluteTarget(0);
}
