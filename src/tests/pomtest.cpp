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
#include <kiprplus/pid_motor.hpp>
#include <kiprplus/aggregation_engine.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"

namespace go
{
    Navigation *nav = nullptr;
}

int main()
{
    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    // wait_for_side_button();

    /*auto lmotor = std::make_shared<kp::PIDMotor>(0);
    auto rmotor = std::make_shared<kp::PIDMotor>(1);
    kp::AggregationEngine myengine({rmotor, lmotor});
    myengine.setMovementModifiers({1, -1});

    lmotor->clearPositionCounter();
    rmotor->clearPositionCounter();
    lmotor->setAbsoluteTarget(0);
    rmotor->setAbsoluteTarget(0);
    lmotor->enablePositionControl();
    rmotor->enablePositionControl();

    for (int i = 0; i < 2; i++)
    {
        myengine.moveRelativePosition(1000, 4000);
        std::cout << "positive done" << std::endl;
        myengine.moveRelativePosition(1000, -4000);
        std::cout << "negative done" << std::endl;
    }
    //refmotor.moveRelativePosition(300, 10000);
    /*while (true)
    {
        int target = potpos.value();
        mymotor.setAbsoluteTarget(target);
        msleep(2);
    }

    return 0;*/

    go::nav = new CRNav;

    go::pom_container.initialize();
    go::nav->initialize();
    go::nav->setMotorSpeed(1200);

    /*go::pom_container.setColorSelector(PomContainer::pos_t::red);

    go::nav.driveDistance(35);
    go::nav.awaitTargetReached();
    go::nav.rotateBy(-M_PI_2);
    go::nav.awaitTargetReached();*/

    /*go::nav->driveDistance(100);
    msleep(10000);
    go::nav->driveDistance(-100);*/
    go::pom_container.open();

    /*
    test vid 1
    go::nav->driveDistance(40);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(100);
    go::nav->awaitTargetReached();
    msleep(1000);*/

    /*for (int i = 0; i < 5; i++)
    {
        go::pom_container.close();
        go::nav->driveDistance(25);
        go::pom_container.open();
        go::nav->driveDistance(15);
    }*/

    /*
    test vid 1
    go::nav->rotateBy(M_PI);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(100);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(-40);
    go::nav->awaitTargetReached();
    msleep(1000);
    */

    
    //test vid 2
    go::nav->driveDistance(50);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(60);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(50);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(-M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(40);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(-M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(50);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(-M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(100);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->driveDistance(-50);
    go::nav->awaitTargetReached();
    msleep(1000);
    
    
    
    /*go::nav->rotateBy(M_PI * 10);
    go::nav->awaitTargetReached();
    msleep(8000);
    go::nav->rotateBy(M_PI * -10);
    go::nav->awaitTargetReached();*/

    go::pom_container.terminate();
    go::nav->terminate();
}