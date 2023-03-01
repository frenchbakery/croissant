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
    // reset servo positions
    int l_speed = l_servo.getSpeed();
    int r_speed = r_servo.getSpeed();

    l_servo.setPosition(l_initial, 2048);
    r_servo.setPosition(r_initial, 2048);

    // wait for motor to reach end
    y_motor.moveAtVelocity(-100);
    while (!esw.value()) { msleep(10); };
    y_motor.off();

    // wait a little bit longer so the motor actually stops
    msleep(200);

    // reset the position counters
    y_motor.clearPositionCounter();
    y_motor.setAbsoluteTarget(0);

    // wait for servos to travel and rollback their speed
    l_servo.waitUntilComleted();
    r_servo.waitUntilComleted();
    l_servo.setSpeed(l_speed);
    r_servo.setSpeed(r_speed);
}


void Arm::setYPerc(float percent)
{
    if (percent > 100 || percent < 0)
    {
        std::cout << "Invalid percent value: " << percent << "% when max height is 100%\n";
        y_motor.off();
        shutdownAndBlock();
    }
    y_motor.setAbsoluteTarget(max_y * (percent / 100));
}

float Arm::getYPerc()
{
    return 100 * (y_motor.getPosition() / max_y);
}

void Arm::setYcm(float cm)
{
    if (cm > y_cm || cm < 0)
    {
        std::cout << "Invalid cm value: " << cm << "cm when max height is " << y_cm << "cm\n";
        shutdownAndBlock();
    }
    y_motor.setAbsoluteTarget((cm / y_cm) * max_y);
}

float Arm::getYcm()
{
    return y_cm * (y_motor.getPosition() / max_y);
}


void Arm::shutdownAndBlock()
{
    y_motor.off();
    l_servo.disable();
    r_servo.disable();
    for (;;) { msleep(1000); }
}


void Arm::grab(int percent)
{
    // move cross
}


void Arm::tilt(int angle)
{
    // move straight
}
