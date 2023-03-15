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
#include <kipr/digital/digital.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"


#define D2R(deg) ((deg)*M_PI/180)

#define CALIB_SPEED 400 // TPS


kipr::digital::Digital back_button_left(0);
kipr::digital::Digital back_button_right(1);


namespace go
{
    CRNav *nav = nullptr;
}


void Straight_line_sorter()
{
    go::nav->driveDistance(17.5);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    msleep(200);
    go::pom_container.open();
    msleep(200);
    go::nav->driveDistance(23.5);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
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

    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(-100);
    go::nav->driveRightSpeed(-100);

    for (;;)
    {
        if (back_button_left.value())
            go::nav->driveLeftSpeed(0);
        else 
            go::nav->driveLeftSpeed(-CALIB_SPEED);
        
        if (back_button_right.value())
            go::nav->driveRightSpeed(0);
        else 
            go::nav->driveRightSpeed(-CALIB_SPEED);

        if (back_button_left.value() && back_button_right.value())
            break;
        
        msleep(10);
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();

    msleep(1000);

    go::nav->driveDistance(2);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    msleep(1000);


    //First pom is collected
    go::pom_container.open();
    go::nav->driveVector(el::cart_t(20,-20));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(45));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveDistance(31);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.close();

    //Align Croissant parallel to balck gametable line
    go::nav->rotateBy(D2R(90));
    go::nav->driveDistance(13);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    
    //Takes red poms from the line until end is reached
    for (int i = 0; i < 3; i++)
    {
        Straight_line_sorter();
    }
    
    //Gets to position where it can collect the other red pompoms
    go::nav->rotateBy(D2R(-180));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveVector(el::cart_t(-23.5,-23.5));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(40));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveDistance(105);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    
    go::pom_container.open();
    go::nav->driveDistance(30);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.close();



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

    msleep(2000);
    go::pom_container.terminate();
    go::nav->terminate();
}