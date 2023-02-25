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

#include <kipr/time/time.h>
#include <kipr/wait_for/wait_for.h>
#include <kipr/motor/motor.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"

CRNav __crnav;
namespace go
{
    Navigation &nav = __crnav;
};

/*
kp::RampedMotor testmotor(0);
kp::RampedMotor testmotor2(1);
*/

int main()
{
    //wait_for_side_button();
    /*
    testmotor.clearPositionCounter();
    testmotor.moveRelativePosition(500, 1500);
    testmotor2.clearPositionCounter();
    testmotor2.moveRelativePosition(500, 1500);

    testmotor.blockMotorDone();
    testmotor2.blockMotorDone();
*/

    //return 0;


    go::pom_container.initialize();
    go::nav.initialize();
    go::nav.setMotorSpeed(500);
    
    /*go::pom_container.setColorSelector(PomContainer::pos_t::red);

    go::nav.driveDistance(35);
    go::nav.awaitTargetReached();
    go::nav.rotateBy(-M_PI_2);
    go::nav.awaitTargetReached();*/
    
    int n = 5;
    go::nav.driveDistance(20 * n);

    for (int i = 0; i < 5; i++)
    {
        go::pom_container.open();
        go::nav.awaitTargetPercentage((100/n) * i);
        go::pom_container.close();
    }

    go::pom_container.terminate();
    go::nav.terminate();
}