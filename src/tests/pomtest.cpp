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
#include <kiprplus/sync_pid.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"

CRNav __crnav;
namespace go
{
    Navigation &nav = __crnav;
};


kipr::motor::Motor testmotor(0);
kipr::motor::BackEMF motorpos(0);
kipr::motor::BackEMF potpos(1);
kipr::motor::Motor refmotor(1);
kp::SyncPID mypid(20, 0, 0, 0, -1500, 1500);
int delay = 1;
/*
kp::RampedMotor testmotor2(1);
*/

int main()
{
    //wait_for_side_button();
    
    bool threxit = false;
    testmotor.clearPositionCounter();
    refmotor.clearPositionCounter();
    //mypid.setOutputDeadband(8, 9);

    std::thread pidthread([&threxit](){
        while (!threxit)
        {
            double output = mypid.update(delay, motorpos.value());
            //std::cout << output << std::endl;
            testmotor.moveAtVelocity(output);
            msleep(delay);
        }
    });

    /*for (int i = 0; i < 2; i++)
    {
        mypid.setSetpoint(2000);
        msleep(5000);
        mypid.setSetpoint(-2000);
        msleep(5000);
    }*/
    //refmotor.moveRelativePosition(300, 10000);
    while (true)
    {
        int target = potpos.value();
        mypid.setSetpoint(target);
        msleep(2);
    }
    
    threxit = true;

    if (pidthread.joinable())
        pidthread.join();

    return 0;


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