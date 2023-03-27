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
};

Arm::~Arm()
{
    calibrate_abort = true;
    
    if (calib_thread.joinable())
        calib_thread.join();
}

el::retcode Arm::initialize()
{
    l_servo.enable();
    r_servo.enable();

    // reset servo positions
    int l_speed = l_servo.getSpeed();
    int r_speed = r_servo.getSpeed();
    
    l_servo.setPosition(l_initial, 1024);
    r_servo.setPosition(r_initial, 1024);

    l_servo.waitUntilComleted();
    r_servo.waitUntilComleted();
    l_servo.setSpeed(l_speed);
    r_servo.setSpeed(r_speed);

    y_motor.setAbsoluteTarget(0);
    y_motor.clearPositionCounter();
    y_motor.enablePositionControl();

    return el::retcode::ok;
}

el::retcode Arm::terminate()
{
    y_motor.off();

    l_servo.disable();
    r_servo.disable();

    return el::retcode::ok;
}

void Arm::calibrateY()
{
    if (calibrate_in_progress) return;

    // join thread if it is not joined already
    if (calib_thread.joinable())
        calib_thread.join();
    
    calibrate_in_progress = true;

    calib_thread = std::thread([this](){
        this->y_motor.disablePositionControl();
        msleep(20);

        // wait for motor to reach end
        this->y_motor.moveAtVelocity(-200);
        while (!esw.value()) { 
            if ((bool)(this->calibrate_abort))
            {
                this->y_motor.off();
                this->calibrate_in_progress = false;
                return;
            }
            msleep(10); 
        };
        this->y_motor.off();

        // wait a little bit longer so the motor actually stops
        msleep(200);

        // reset the position counters
        this->y_motor.clearPositionCounter();
        this->y_motor.setAbsoluteTarget(0);
        this->y_motor.enablePositionControl();

        this->calibrate_in_progress = false;
    });
}

void Arm::waitForCalibrate()
{
    if (calib_thread.joinable())
        calib_thread.join();
}

void Arm::setYPerc(float percent)
{
    if (percent > 100 || percent < 0)
    {
        std::cout << "Invalid percent value: " << percent << "% when max height is 100%\n";
        y_motor.off();
        shutdownAndBlock();
    }
    y_motor.enablePositionControl();
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
    y_motor.enablePositionControl();
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
