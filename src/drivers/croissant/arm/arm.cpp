#include <kipr/time/time.h>
#include <iostream>
#include "arm.hpp"


Arm::Arm(int motor_port, int left_servo_port, int right_servo_port, int end_switch_port)
    : 
        y_motor(motor_port, 500), 
        l_servo(left_servo_port, l_initial),
        r_servo(right_servo_port, r_initial),
        esw(end_switch_port)
{
    l_servo_current = l_initial;
    r_servo_current = r_initial;

    l_servo.enable();
    r_servo.enable();
};


void Arm::calibrate()
{
    // reset servo positions
    int l_speed = l_servo.getSpeed();
    int r_speed = r_servo.getSpeed();
    
    l_servo.setPosition(l_initial, 1024);
    r_servo.setPosition(r_initial, 1024);

    // wait for motor to reach end
    y_motor.moveAtVelocity(-100);
    while (!esw.value()) { msleep(10); };
    y_motor.off();

    // wait a little bit longer so the motor actually stops
    msleep(200);

    // reset the position counters
    y_motor.enablePositionControl();
    y_motor.clearPositionCounter();
    y_motor.setAbsoluteTarget(0);

    // wait for servos to travel and rollback their speed
    l_servo.waitUntilComleted();
    r_servo.waitUntilComleted();
    l_servo.setSpeed(l_speed);
    r_servo.setSpeed(r_speed);
    std::cout << "servos done\n";
    y_motor.enablePositionControl();
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


void Arm::shutdownAndBlock(const char *reason)
{
    if (reason != "")
        std::cout << reason;

    y_motor.off();
    l_servo.disable();
    r_servo.disable();
    for (;;) { msleep(1000); }
}


void Arm::grab(int percent)
{
    // 
    int grab_delta = grab_current - percent;

    // add / subtract closed delta from current angle
    int r_pos = r_servo.getSetPosition() - max_grab * (grab_delta / 100.0);
    int l_pos = l_servo.getSetPosition() - max_grab * (grab_delta / 100.0);

    // update servo positions
    r_servo.setPosition(r_pos);
    l_servo.setPosition(l_pos);

    grab_current = percent;
}

void Arm::tilt(int angle)
{
    if (!(angle <= 90))
        shutdownAndBlock("invalid angle!");

    // move straight
    int angle_delta = current_angle - angle;
    float angle_perc = angle_delta / 90.0;

    int add = 1024 * angle_perc;
    
    std::cout << "is: " << l_servo.getSetPosition() << ", " << r_servo.getSetPosition() << std::endl;

    // add / subtract angle delta from current angle
    int l_pos = l_servo.getSetPosition() - add;
    int r_pos = r_servo.getSetPosition() + add;

    // update servo positions
    r_servo.setPosition(r_pos);
    l_servo.setPosition(l_pos);

    current_angle = angle;
}

float Arm::getTilt()
{
    return current_angle;
}

void Arm::waitForGrab()
{
    l_servo.waitUntilComleted();
    r_servo.waitUntilComleted();
}


void Arm::waitForTilt()
{
    waitForGrab();
}

void Arm::waitForY()
{
    y_motor.blockMotorDone();
}


void Arm::setServoSpeed(int speed)
{
    l_servo.setSpeed(speed);
    r_servo.setSpeed(speed);
}
