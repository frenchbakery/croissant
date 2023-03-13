/**
 * @file main.cpp
 * @author LiloZz
 * @brief croissant robot main file
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright FrenchBakery(c) 2023
 * 
 */

#include <iostream>

#include <kipr/time/time.h>
#include <kipr/wait_for/wait_for.h>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"

#define D2R(deg) ((deg)*M_PI/180)

namespace go
{
    Navigation *nav = nullptr;
}

void Straight_line_sorter()
{
    go::nav->driveDistance(20.5);
    go::nav->awaitTargetReached();
    go::pom_container.open();
    go::nav->driveDistance(20.5);
    go::nav->awaitTargetReached();
    go::pom_container.close();
}


int main()
{
    auto dmotor = std::make_shared<kp::PIDMotor>(5);
    go::nav = new CRNav;

    //Press button on Wombat before programm starts
    //wait_for_side_button();

    go::pom_container.initialize();
    go::nav->initialize();
    go::nav->setMotorSpeed(1200);

    //First pom is collected
    go::nav->driveVector(el::cart_t(20,-20.7));
    msleep(1000);
    go::nav->rotateBy(D2R(45));
    go::nav->awaitTargetReached();
    go::pom_container.open();
    go::nav->driveDistance(30);
    go::nav->awaitTargetReached();
    go::pom_container.close();

    //Align Croissant parallel to balck gametable line
    go::nav->rotateBy(D2R(90));
    go::nav->awaitTargetReached();
    msleep(500);
    go::nav->driveDistance(13);
    
    //Takes red poms from the line until end is reached
    for (int i = 0; i < 2; i++)
    {
        Straight_line_sorter();
    }
    
    /*
    go::pom_container.open();
    go::pom_container.close();

    go::nav->driveDistance(50);
    go::nav->awaitTargetReached();
    msleep(1000);
    go::nav->rotateBy(M_PI_2);
    go::nav->awaitTargetReached();
    msleep(1000);
    */

    go::pom_container.terminate();
    go::nav->terminate();
}