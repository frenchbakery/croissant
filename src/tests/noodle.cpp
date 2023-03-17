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


kipr::analog::Analog line_left(5);


void prepare_grab() 
{
    go::arm->setYcm(7);
    go::arm->waitForY();
    go::arm->tilt(0);
    go::arm->waitForTilt();
    go::arm->grab(0);
    go::arm->waitForGrab();
}


void grab_noodle() 
{
    go::arm->grab(100);
    go::arm->waitForGrab();
    go::nav->driveDistance(-30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::arm->tilt(90);
    go::arm->waitForTilt();
}


int main()
{
    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    go::nav = new CRNav;
    go::arm = new Arm(MOTOR_PORT, LEFT_SERVO, RIGHT_SERVO, END_SWITCH);

    go::arm->calibrate();
    go::nav->initialize();
    go::nav->setMotorSpeed(1000);

    int t = 0;
    int g = 0;

    for (;;) {
        std::cout << "Tilt";
        std::cin >> t;
        std::cout << "Grab";
        std::cin >> g;

        go::arm->tilt(t);
        go::arm->waitForTilt();

        go::arm->grab(g);
        go::arm->waitForGrab();

    }


    

    return 0;

    prepare_grab();

    go::nav->driveDistance(30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    grab_noodle();

    // for (;;)
    // {
    //     go::arm->tilt(line_left.value() / 54.4);
    //     go::arm->waitForTilt();
    //     msleep(100);
    // }

    msleep(2000);

    go::nav->terminate();
}


