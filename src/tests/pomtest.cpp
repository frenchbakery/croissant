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

/*CRNav __crnav;
namespace go
{
    Navigation &nav = __crnav;
};*/





int main()
{
    //wait_for_side_button();
    msleep(1000);

    std::thread mythread([](){
        msleep(5000);
    });

    auto lmotor = std::make_shared<kp::PIDMotor>(0);
    auto rmotor = std::make_shared<kp::PIDMotor>(1);
    auto dmotor = std::make_shared<kp::PIDMotor>(2);
    kp::AggregationEngine myengine({dmotor, lmotor, rmotor});

    lmotor->clearPositionCounter();
    rmotor->clearPositionCounter();
    dmotor->clearPositionCounter();
    lmotor->setAbsoluteTarget(0);
    rmotor->setAbsoluteTarget(0);
    dmotor->setAbsoluteTarget(0);
    lmotor->enablePositionControl();
    rmotor->enablePositionControl();
    dmotor->enablePositionControl();

    std::cout << "enabled, waiting" << std::endl;
    msleep(3000);
    std::cout << "go!" << std::endl;

    for (int i = 0; i < 2; i++)
    {
        myengine.moveRelativePosition(500, 2000);
        std::cout << "positive done" << std::endl;
        msleep(5000);
        myengine.moveRelativePosition(500, -2000);
        std::cout << "negative done" << std::endl;
        msleep(5000);
    }
    //refmotor.moveRelativePosition(300, 10000);
    /*while (true)
    {
        int target = potpos.value();
        mymotor.setAbsoluteTarget(target);
        msleep(2);
    }*/

    if (mythread.joinable())
        mythread.join();

    return 0;


    /*go::pom_container.initialize();
    go::nav.initialize();
    go::nav.setMotorSpeed(500);*/
    
    /*go::pom_container.setColorSelector(PomContainer::pos_t::red);

    go::nav.driveDistance(35);
    go::nav.awaitTargetReached();
    go::nav.rotateBy(-M_PI_2);
    go::nav.awaitTargetReached();*/
    
    /*int n = 5;
    go::nav.driveDistance(20 * n);

    for (int i = 0; i < 5; i++)
    {
        go::pom_container.open();
        go::nav.awaitTargetPercentage((100/n) * i);
        go::pom_container.close();
    }

    go::pom_container.terminate();
    go::nav.terminate();*/
}