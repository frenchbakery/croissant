/**
 * @file pomtest.cpp
 * @author test program for pom sorting
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright FrenchBakery (c) 2023
 * 
 */

#include <iostream>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/arm/arm.hpp"
#include <kipr/time/time.h>
#include <kipr/analog/analog.hpp>

namespace go
{
    Navigation *nav = nullptr;
    Arm *arm = nullptr;
}

#define MOTOR_PORT 2
#define LEFT_SERVO 1
#define RIGHT_SERVO 2
#define END_SWITCH 2


void prepare_grab() 
{
    go::arm->setYcm(8);
    go::arm->waitForY();
    go::arm->tilt(0);
    go::arm->waitForTilt();
    go::arm->tilt(90);
    go::arm->waitForTilt();
    go::arm->grab(0);
    go::arm->waitForGrab();
}


void grab_noodle() 
{
    go::arm->grab(100);
    go::nav->driveDistance(-30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::arm->tilt(0);
}

kipr::analog::Analog line_left(5);

int main()
{
    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    go::nav = new CRNav;
    go::arm = new Arm(MOTOR_PORT, LEFT_SERVO, RIGHT_SERVO, END_SWITCH);

    go::arm->calibrate();
    go::nav->initialize();
    go::nav->setMotorSpeed(1000);

    //prepare_grab();

    go::arm->tilt(90);
    go::arm->waitForTilt();
    go::arm->grab(100);
    go::arm->waitForGrab();

    // for (;;)
    // {
    //     go::arm->tilt(line_left.value() / 54.4);
    //     go::arm->waitForTilt();
    //     msleep(100);
    // }

    // for (;;)
    // {
    //     go::arm->tilt(0);
    //     go::arm->waitForTilt();
    //     msleep(100);
    //     go::arm->tilt(90);
    //     go::arm->waitForTilt();
    //     msleep(100);
    // }

    msleep(2000);

    go::nav->terminate();
}