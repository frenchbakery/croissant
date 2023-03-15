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
#include <kipr/analog/analog.hpp>

#include "drivers/navigation/croissant/crnav.hpp"
#include "drivers/croissant/pom_sorter/pom_container.hpp"


#define D2R(deg) ((deg)*M_PI/180)

#define CALIB_SPEED 500 // TPS
#define LIGHT_THRESHOLD 1500


kipr::digital::Digital back_button_left(0);
kipr::digital::Digital back_button_right(1);
kipr::analog::Analog line_left(0);
kipr::analog::Analog line_right(1);



namespace go
{
    CRNav *nav = nullptr;
}

/*
void correctAlignment()
{
    if (onLine())
    {
        go::nav->disablePositionControl();

        go::nav->driveLeftSpeed(100);
        go::nav->driveRightSpeed(-100);

        while (onLine())
            msleep(10);

        go::nav->driveLeftSpeed(0);
        go::nav->driveRightSpeed(0);
        go::nav->resetPositionControllers();
        go::nav->enablePositionControl();
    }
    else
    {
        go::nav->disablePositionControl();

        go::nav->driveLeftSpeed(-100);
        go::nav->driveRightSpeed(100);

        while (!onLine())
            msleep(10);

        go::nav->driveLeftSpeed(0);
        go::nav->driveRightSpeed(0);
        go::nav->resetPositionControllers();
        go::nav->enablePositionControl();
    }
}
*/

void straightLineSorter(double offset = 0)
{
    double first_distance = 15;
    go::nav->driveDistance(first_distance + offset);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.open();
    go::nav->driveDistance(41 - first_distance);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.close();
    //go::pom_container.hold();
    //correctAlignment();
}

void alignBack()
{
    // Find zero point in X direction and align robot to game table
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
}

void alignFront()
{
    // Find zero point in X direction and align robot to game table
    go::nav->disablePositionControl();

    go::nav->driveLeftSpeed(CALIB_SPEED);
    go::nav->driveRightSpeed(CALIB_SPEED);

    for (;;)
    {
        if (line_left.value() > LIGHT_THRESHOLD)
            go::nav->driveLeftSpeed(0);
        else 
            go::nav->driveLeftSpeed(CALIB_SPEED);
        
        if (line_right.value() > LIGHT_THRESHOLD)
            go::nav->driveRightSpeed(0);
        else 
            go::nav->driveRightSpeed(CALIB_SPEED);

        if (line_left.value() > LIGHT_THRESHOLD && line_right.value() > LIGHT_THRESHOLD)
            break;
        
        msleep(10);
    }

    go::nav->driveLeftSpeed(0);
    go::nav->driveRightSpeed(0);
    go::nav->resetPositionControllers();
    go::nav->enablePositionControl();
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

    alignBack();
    msleep(1000);

    // drive to home positoin
    go::nav->driveDistance(5);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    msleep(1000);

    //First pom is collected
    go::pom_container.open();
    go::nav->driveVector(el::cart_t(15, -15));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(45));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveDistance(33);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::pom_container.close();

    //Align Croissant parallel to balck gametable line
    go::nav->rotateBy(D2R(90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    
    //Takes red poms from the line until end is reached
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
            straightLineSorter(13);
        else
            straightLineSorter();
    }
    
    // Realign at the end of the table
    go::nav->driveDistance(-20);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveDistance(-10);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    alignBack();
    go::nav->driveDistance(20);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(-90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    alignFront();

    // Gets to position where it can collect the other red pompoms
    go::nav->driveDistance(35);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->rotateBy(D2R(90));
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();
    go::nav->driveDistance(130);
    go::nav->startSequence();
    go::nav->awaitSequenceComplete();

    // collect first double stack
    go::pom_container.open();
    go::nav->driveDistance(20);
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